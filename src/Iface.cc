/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <tinyxml2.h>

#include <ignition/common/Console.hh>
#include <ignition/common/PluginLoader.hh>
#include <ignition/common/SystemPaths.hh>

#include "ignition/gui/qt.h"
#include "ignition/gui/Iface.hh"
#include "ignition/gui/MainWindow.hh"
#include "ignition/gui/Plugin.hh"

// These are needed by QT. They need to stay valid during the entire
// lifetime of the application, and argc > 0 and argv must contain one valid
// character string
int g_argc = 1;
char **g_argv;

using namespace ignition;
using namespace gui;

QApplication *g_app;
MainWindow *g_main_win = nullptr;
std::vector<std::unique_ptr<Plugin>> g_plugins;
std::string g_pluginPathEnv = "IGN_GUI_PLUGIN_PATH";

/////////////////////////////////////////////////
// Check whether the app has been initialized
bool checkApp()
{
  if (!g_app)
  {
    ignerr <<
        "Application not initialized. Have you called initApp() yet?"
        << std::endl;
  }

  return g_app;
}

/////////////////////////////////////////////////
// Install a signal handler to allow graceful shutdown on Ctrl-C.
bool installSignalHandler()
{
  if (!checkApp())
    return false;

#ifndef _WIN32
  auto handler = [](int)
      {
        // Note: Don't call stop() for the main window, we close it and let the
        // program pick it up from there
        if (g_main_win)
          g_main_win->close();
        else
          stop();
      };

  struct sigaction sigact;
  sigact.sa_flags = 0;
  sigact.sa_handler = handler;

  if (sigemptyset(&sigact.sa_mask) != 0)
  {
    ignerr << "sigemptyset failed while setting up for SIGINT"
              << std::endl;
    return false;
  }

  if (sigaction(SIGINT, &sigact, nullptr))
  {
    ignerr << "signal(2) failed while setting up for SIGINT"
              << std::endl;
    return false;
  }
#endif

  return true;
}

/////////////////////////////////////////////////
bool ignition::gui::runConfig(const std::string &_config)
{
  ignmsg << "Loading config file [" << _config << "]" << std::endl;

  if (_config.empty())
  {
    ignerr << "Missing config filename" << std::endl;
    return false;
  }

  initApp();
  loadConfig(_config);
  createMainWindow();
  runMainWindow();
  stop();

  return true;
}

/////////////////////////////////////////////////
bool ignition::gui::runStandalone(const std::string &_filename)
{
  ignmsg << "Loading standalone plugin [" << _filename << "]" << std::endl;

  if (_filename.empty())
  {
    ignerr << "Missing plugin filename" << std::endl;
    return false;
  }

  initApp();
  loadPlugin(_filename);
  runDialogs();
  stop();

  return true;
}

/////////////////////////////////////////////////
bool ignition::gui::initApp()
{
  // Configure console
  ignition::common::Console::SetPrefix("[GUI] ");

  ignmsg << "Init app" << std::endl;

  // Create app
  g_app = new QApplication(g_argc, g_argv);

  // Set style
  QFile file(":/style.qss");
  file.open(QFile::ReadOnly);
  QString styleSheet = QLatin1String(file.readAll());
  g_app->setStyleSheet(styleSheet);

  // Install signal handler for graceful shutdown
  installSignalHandler();

  return true;
}

/////////////////////////////////////////////////
bool ignition::gui::stop()
{
  ignmsg << "Stop" << std::endl;

  if (g_main_win)
  {
    g_main_win->close();
    delete g_main_win;
    g_main_win = nullptr;
  }

  if (g_app)
  {
    g_app->quit();
    delete g_app;
    g_app = nullptr;
  }

  g_plugins.clear();

  return true;
}

/////////////////////////////////////////////////
bool ignition::gui::loadConfig(const std::string &_config)
{
  if (!checkApp())
    return false;

  if (_config.empty())
  {
    ignerr << "Missing config file" << std::endl;
    return false;
  }

  // Use tinyxml to read config
  tinyxml2::XMLDocument doc;
  auto success = !doc.LoadFile(_config.c_str());
  if (!success)
  {
    ignmsg << "Failed to load file [" << _config << "]: XMLError"
              << std::endl;
    return false;
  }

  // Process each plugin
  for(auto pluginElem = doc.FirstChildElement("plugin"); pluginElem != nullptr;
      pluginElem = pluginElem->NextSiblingElement("plugin"))
  {
    auto filename = pluginElem->Attribute("filename");
    loadPlugin(filename);
  }

  return true;
}

/////////////////////////////////////////////////
bool ignition::gui::loadPlugin(const std::string &_filename)
{
  if (!checkApp())
    return false;

  // Get full path
  char *homePath = getenv("HOME");
  std::string home;
  if (homePath)
    home = homePath;

  ignition::common::SystemPaths systemPaths;
  systemPaths.SetPluginPathEnv(g_pluginPathEnv);
  systemPaths.AddPluginPaths(home + "/.ignition/gui/plugins");

  auto pathToLib = systemPaths.FindSharedLibrary(_filename);

  // Load plugin
  ignition::common::PluginLoader pluginLoader;
  auto pluginName = pluginLoader.LoadLibrary(pathToLib);
  auto plugin = pluginLoader.Instantiate<ignition::gui::Plugin>(pluginName);

  if (!plugin)
  {
    ignerr << "Failed to load plugin [" << _filename << "]" << std::endl;
    return false;
  }

  // Store plugin in list
  g_plugins.push_back(std::move(plugin));

  return true;
}

/////////////////////////////////////////////////
bool ignition::gui::createMainWindow()
{
  if (!checkApp())
    return false;

  ignmsg << "Create main window" << std::endl;

  g_main_win = new MainWindow();

  // Create a widget for each plugin
  for (auto &plugin : g_plugins)
  {
    auto title = QString::fromStdString(plugin->Title());
    auto dock = new QDockWidget(title, g_main_win);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea |
                          Qt::RightDockWidgetArea);
    dock->setWidget(&*plugin);
    g_main_win->addDockWidget(Qt::LeftDockWidgetArea, dock);

    // Qt steals the ownership of the plugin (QWidget)
    // Remove it from the smart pointer without calling the destructor
    plugin.release();
  }
  g_plugins.clear();

  return true;
}

/////////////////////////////////////////////////
ignition::gui::MainWindow *ignition::gui::mainWindow()
{
  return g_main_win;
}

/////////////////////////////////////////////////
bool ignition::gui::runMainWindow()
{
  if (!checkApp())
    return false;

  if (!mainWindow())
    return false;

  ignmsg << "Run main window" << std::endl;

  g_main_win->show();

  // Execute app
  g_app->exec();

  return true;
}

/////////////////////////////////////////////////
bool ignition::gui::runDialogs()
{
  if (!checkApp())
    return false;

  ignmsg << "Run dialogs" << std::endl;

  for (auto &plugin : g_plugins)
  {
    auto layout = new QVBoxLayout();
    layout->addWidget(plugin.get());

    // Qt steals the ownership of the plugin (QWidget)
    // Remove it from the smart pointer without calling the destructor
    plugin.release();

    auto dialog = new QDialog();
    dialog->setLayout(layout);

    dialog->exec();
  }
  g_plugins.clear();

  return true;
}

/////////////////////////////////////////////////
void ignition::gui::setPluginPathEnv(const std::string &_path)
{
  g_pluginPathEnv = _path;
}

/////////////////////////////////////////////////
void ignition::gui::setVerbosity(const unsigned int _verbosity)
{
  ignition::common::Console::SetVerbosity(_verbosity);
}
