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
#ifndef IGNITION_GUI_IFACE_HH_
#define IGNITION_GUI_IFACE_HH_

#include <tinyxml2.h>
#include <string>
#include <utility>
#include <vector>

#include "ignition/gui/MainWindow.hh"
#include "ignition/gui/System.hh"

namespace ignition
{
  namespace gui
  {
    /// \brief Initialize the application. This must be run before everything
    /// else.
    /// \return True on success.
    IGNITION_GUI_VISIBLE
    bool initApp();

    /// \brief Stop the graphical interface.
    IGNITION_GUI_VISIBLE
    bool stop();

    /// \brief Load plugins from a configuration file.
    /// \param[in] _config Path to configuration file.
    IGNITION_GUI_VISIBLE
    bool loadConfig(const std::string &_config);

    /// \brief Load a plugin from a file name. The plugin file must be in the
    /// path.
    /// \param[in] _filename Plugin filename.
    /// \param[in[ _pluginElem Element containing plugin configuration
    IGNITION_GUI_VISIBLE
    bool loadPlugin(const std::string &_filename,
        const tinyxml2::XMLElement *_pluginElem = nullptr);

    /// \brief Add previously loaded plugins to the main window.
    /// * Make sure the window is created first
    /// * Be sure to call loadPlugin() for each plugin first
    /// * Plugins previously added must be loaded again
    /// \return True if successful
    IGNITION_GUI_VISIBLE
    bool addPluginsToWindow();

    /// \brief Apply previously loaded config to the main window.
    /// * Make sure the window is created first
    /// * Be sure to call loadConfig() for each plugin first
    /// \return True if successful
    IGNITION_GUI_VISIBLE
    bool applyConfig();

    /// \brief Run a main window using the given configuration file. This is
    /// the main entry point for the command line tool "ign gui -c".
    /// \param[in] _config Full path to configuration file.
    IGNITION_GUI_VISIBLE
    bool runConfig(const std::string &_config);

    /// \brief Run a given plugin as a standalone window. This is the main
    /// entry point for the command line tool "ign gui -s".
    /// \param[in] _filename Plugin file name. The file must be in the path.
    IGNITION_GUI_VISIBLE
    bool runStandalone(const std::string &_filename);

    /// \brief Run previously loaded plugins as individual dialogs.
    /// This has no effect if no plugins have been loaded.
    IGNITION_GUI_VISIBLE
    bool runDialogs();

    /// \brief Run previously loaded plugins on a single main window.
    /// An empty window will be created if no plugins have been loaded.
    IGNITION_GUI_VISIBLE
    bool createMainWindow();

    /// \brief Get the main window, if one has been generated. Use this to
    /// customize the window before running it.
    IGNITION_GUI_VISIBLE
    ignition::gui::MainWindow *mainWindow();

    /// \brief Get the dialogs, if they have been generated.
    IGNITION_GUI_VISIBLE
    std::vector<QDialog *> dialogs();

    /// \brief Run previously created window.
    IGNITION_GUI_VISIBLE
    bool runMainWindow();

    /// \brief Set the environment variable which defines the paths to look for
    /// plugins.
    /// \param[in] _env Name of environment variable.
    IGNITION_GUI_VISIBLE
    void setPluginPathEnv(const std::string &_env);

    /// \brief Add an path to look for plugins.
    /// \param[in] _path Full path.
    IGNITION_GUI_VISIBLE
    void addPluginPath(const std::string &_path);

    /// \brief Set the verbosity level (from 0 to 4).
    IGNITION_GUI_VISIBLE
    void setVerbosity(const unsigned int _verbosity);
  }
}
#endif
