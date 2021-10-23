/*
 * Copyright (C) 2020 Open Source Robotics Foundation
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

/*
 * GzGammaAdjust.qml is based upon GammaAdjust.qml from the QtGraphicalEffects
 * module in Qt 5.15.2 which has the license listed below.
*/

/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Graphical Effects module.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.9

Item {
  id: rootItem

  property variant source

  property real gamma: 1.0

  property bool cached: false

  // Replaces SourceProxy in the QtGraphicalEffects version
  ShaderEffectSource {
    id: sourceProxy
    sourceItem: rootItem.source
    visible: rootItem.visible
    smooth: true
    live: true
    hideSource: visible
  }

  // Output
  ShaderEffectSource {
    id: cacheItem
    anchors.fill: parent
    visible: rootItem.cached
    smooth: true
    sourceItem: shaderItem
    live: true
    hideSource: visible
  }

  // Apply the gamma adjustment to the source proxy
  ShaderEffect {
    id: shaderItem
    property variant source: sourceProxy
    property real gamma: 1.0 / Math.max(rootItem.gamma, 0.0001)

    anchors.fill: parent

    fragmentShader: "qrc:qml/private/shaders/gammaadjust.frag"
  }
}
