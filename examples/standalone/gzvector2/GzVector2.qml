import QtQuick 2.9
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3
import gz.gui 1.0

/**
 *  Item displaying a 3D vector
 *
 *  Users can set values to xValue, yValue, and zValue.
 *  If readOnly == False,
 *  users can read from signal parameters of GzVectorSet: _x, _y, and _z
 *
 *  Usage example:
 *  GzVector3 {
 *    id: gzVector
 *    xName: "Red"
 *    yName: "Green"
 *    gzUnit: ""
 *    readOnly: false
 *    xValue: xValueFromCPP
 *    yValue: yValueFromCPP
 *    onGzVectorSet: {
 *      myFunc(_x, _y)
 *    }
 *  }
**/
Item {
  id: gzVectorRoot

  // Read-only / write
  property bool readOnly: true

  // User input value
  property double xValue
  property double yValue

  /**
   * Used to read spinbox values
   * @params: _x, _y: corresponding spinBoxes values
   * @note: When readOnly == false, user should read spinbox value from its
   *        parameters.
   *        When readOnly == true, this signal is unused.
   */
  signal gzVectorSet(double _x, double _y)

  // Names for XYZ
  property string xName: "X"
  property string yName: "Y"

  // Units, defaults to meters.
  // Set to "" to omit units & the parentheses.
  property string gzUnit: "m"

  // Expand/Collapse of this widget
  property bool expand: true

  // Maximum spinbox value
  property double spinMax: Number.MAX_VALUE

  /*** The following are private variables: ***/
  height: gzVectorContent.height

  // local variables to store spinbox values
  property var xItem: {}
  property var yItem: {}

  // Dummy component to use its functions.
  GzHelpers {
    id: gzHelper
  }
  /*** Private variables end: ***/

  /**
   * Used to create a spin box
   */
  Component {
    id: writableNumber
    GzSpinBox {
      id: writableSpin
      value: numberValue
      minimumValue: -spinMax
      maximumValue: spinMax
      decimals: gzHelper.getDecimals(writableSpin.width)
      onEditingFinished: {
        gzVectorRoot.gzVectorSet(xItem.value, yItem.value)
      }
    }
  }

  /**
   * Used to create a read-only number
   */
  Component {
    id: readOnlyNumber
    Text {
      id: numberText
      anchors.fill: parent
      horizontalAlignment: Text.AlignRight
      verticalAlignment: Text.AlignVCenter
      text: {
        var decimals = gzHelper.getDecimals(numberText.width)
        return numberValue.toFixed(decimals)
      }
    }
  }

  Rectangle {
    id: gzVectorContent
    width: parent.width
    height: expand ? gzVectorGrid.height : 0
    // clip: true
    color: "transparent"

    Behavior on height {
      NumberAnimation {
        duration: 200;
        easing.type: Easing.InOutQuad
      }
    }

    GridLayout {
      id: gzVectorGrid
      width: parent.width
      columns: 2

      Text {
        text: gzUnit == "" ? xName : xName + ' (' + gzUnit + ')'
        leftPadding: 5
        color: Material.theme == Material.Light ? "#444444" : "#bbbbbb"
        font.pointSize: 12
      }

      Item {
        Layout.fillWidth: true
        height: 40
        Loader {
          id: xLoader
          anchors.fill: parent
          property double numberValue: xValue
          sourceComponent: readOnly ? readOnlyNumber : writableNumber
          onLoaded: {
            xItem = xLoader.item
          }
        }
      }

      Text {
        text: gzUnit == "" ? yName : yName + ' (' + gzUnit + ')'
        leftPadding: 5
        color: Material.theme == Material.Light ? "#444444" : "#bbbbbb"
        font.pointSize: 12
      }

      Item {
        Layout.fillWidth: true
        height: 40
        Loader {
          id: yLoader
          anchors.fill: parent
          property double numberValue: yValue
          sourceComponent: readOnly ? readOnlyNumber : writableNumber
          onLoaded: {
            yItem = yLoader.item
          }
        }
      }
    }
  }
}
