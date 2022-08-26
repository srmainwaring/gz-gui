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
import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import gz.gui 1.0
import "qrc:/GzTestVector_qml"

Column {
  id: gzTestVector
  anchors.fill: parent

  spacing: 2

  GzVector2 {
    id: gzVector2
    xValue: 1.0
    yValue: 2.0
    onGzVectorSet: {
      xValue = _x
      yValue = _y
      console.log(xValue, yValue)
    }
  }

}
