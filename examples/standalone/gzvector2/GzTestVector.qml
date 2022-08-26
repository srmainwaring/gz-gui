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
  anchors.fill: parent
  spacing: 2

  Label {
    font.bold: true
    font.pointSize: 18
    text: "Double GzSpinBox"
  }

  // Double spin box
  GzSpinBox {
    minimumValue: -2.5
    maximumValue: 19.3
    decimals: 4
    stepSize: 3.6
    value: 5.8
    width: 300
  }

  Label {
    font.bold: true
    font.pointSize: 18
    text: "Editable GzVector2"
  }

  GzVector2 {
    readOnly: false
    xValue: 1.0
    yValue: 2.0
    onGzVectorSet: {
      xValue = _x
      yValue = _y
      console.log("(", xValue, ", ", yValue, ")")
    }
    width: 300
  }

  Label {
    font.bold: true
    font.pointSize: 18
    text: "Double GzSpinBox"
  }

  // Double spin box
  GzSpinBox {
    minimumValue: 0.0
    maximumValue: 1.0
    decimals: 2
    stepSize: 0.1
    value: 0.5
    width: 300
  }

}
