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

#include <tinyxml2.h>

#include <ignition/common/Console.hh>
#include "ignition/gui/Iface.hh"
#include "ignition/gui/MainWindow.hh"

namespace ignition
{
  namespace gui
  {
    class MainWindowPrivate
    {
    };
  }
}

using namespace ignition;
using namespace gui;

/////////////////////////////////////////////////
MainWindow::MainWindow()
  : dataPtr(new MainWindowPrivate)
{
  this->setObjectName("mainWindow");

  // Title
  std::string title = "Ignition GUI";
  this->setWindowIconText(tr(title.c_str()));
  this->setWindowTitle(tr(title.c_str()));

  // Main widget

  // Menu
  auto fileMenu = this->menuBar()->addMenu(tr("&File"));

  auto loadConfigAct = new QAction(tr("&Load configuration"), this);
  loadConfigAct->setStatusTip(tr("Quit"));
  this->connect(loadConfigAct, SIGNAL(triggered()), this, SLOT(OnLoadConfig()));
  fileMenu->addAction(loadConfigAct);

  auto saveConfigAct = new QAction(tr("&Save configuration"), this);
  saveConfigAct->setStatusTip(tr("Quit"));
  this->connect(saveConfigAct, SIGNAL(triggered()), this, SLOT(OnSaveConfig()));
  fileMenu->addAction(saveConfigAct);

  fileMenu->addSeparator();

  auto quitAct = new QAction(tr("&Quit"), this);
  quitAct->setStatusTip(tr("Quit"));
  this->connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
  fileMenu->addAction(quitAct);

  // Docking
  this->setDockOptions(QMainWindow::AnimatedDocks |
                       QMainWindow::AllowTabbedDocks |
                       QMainWindow::AllowNestedDocks);
}

/////////////////////////////////////////////////
MainWindow::~MainWindow()
{
}

/////////////////////////////////////////////////
bool MainWindow::CloseAllDocks()
{
  ignmsg << "Closing all docks" << std::endl;

  auto docks = this->findChildren<QDockWidget *>();
  for (auto dock : docks)
    dock->close();

  return true;
}

/////////////////////////////////////////////////
void MainWindow::OnLoadConfig()
{
  QFileDialog fileDialog(this, tr("Load configuration"), QDir::homePath(),
      tr("*.config"));
  fileDialog.setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint |
      Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);

  if (fileDialog.exec() != QDialog::Accepted)
    return;

  auto selected = fileDialog.selectedFiles();
  if (selected.empty())
    return;

  if (!loadConfig(selected[0].toStdString()))
    return;

  if (!this->CloseAllDocks())
   return;

  addPluginsToWindow();
  applyConfig();
}

/////////////////////////////////////////////////
void MainWindow::OnSaveConfig()
{
  QFileDialog fileDialog(this, tr("Save configuration"), QDir::homePath(),
      tr("*.config"));
  fileDialog.setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint |
      Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);
  fileDialog.setAcceptMode(QFileDialog::AcceptSave);

  if (fileDialog.exec() != QDialog::Accepted)
    return;

  auto selected = fileDialog.selectedFiles();
  if (selected.empty())
    return;

  std::string config = "<?xml version=\"1.0\"?>";
  config += "<window>\n";
  config += "  <state>\n";
  config += this->saveState().toBase64().toStdString();
  config += "\n  </state>\n";
  config += "  <position_x>";
  config += std::to_string(this->pos().x());
  config += "</position_x>\n";
  config += "  <position_y>";
  config += std::to_string(this->pos().y());
  config += "</position_y>\n";
  config += "  <width>";
  config += std::to_string(this->width());
  config += "</width>\n";
  config += "  <height>";
  config += std::to_string(this->height());
  config += "</height>\n";
  config += "</window>\n";
  config += "<plugin filename=\"libhello_plugin.so\">\n";
  config += "  <title>1</title>\n";
  config += "</plugin>\n";
  config += "<plugin filename=\"libhello_plugin.so\">\n";
  config += "  <title>2</title>\n";
  config += "</plugin>\n";
  config += "<plugin filename=\"libhello_plugin.so\">\n";
  config += "  <title>3</title>\n";
  config += "</plugin>\n";
  config += "<plugin filename=\"libhello_plugin.so\">\n";
  config += "  <title>4</title>\n";
  config += "</plugin>\n";
  config += "<plugin filename=\"libhello_plugin.so\">\n";
  config += "  <title>5</title>\n";
  config += "</plugin>";

  auto ini = selected[0] + "_ini";

  auto settings = new QSettings(ini, QSettings::IniFormat);
  settings->setValue("state", this->saveState());
  settings->setValue("pos", this->pos());
  settings->setValue("size", this->size());
  settings->sync();

  // Open the file
  std::ofstream out(selected[0].toStdString().c_str(), std::ios::out);

  if (!out)
  {
    QMessageBox msgBox;
    std::string str = "Unable to open file: " + selected[0].toStdString();
    str += ".\nCheck file permissions.";
    msgBox.setText(str.c_str());
    msgBox.exec();
  }
  else
    out << config;

  out.close();
}

