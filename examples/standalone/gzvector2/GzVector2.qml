import QtQuick 2.9
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3

Item {
  id: gzVectorRoot

  // Read-only / write
  property bool readOnly: true

  // User input value
  property double xValue
  property double yValue

  // Names for XYZ
  property string xName: "X"
  property string yName: "Y"

  // Units, defaults to meters.
  // Set to "" to omit units & the parentheses.
  property string gzUnit: "m"

  // Expand/Collapse of this widget
  property bool expand: true

  /*** The following are private variables: ***/
  // height: gzVectorContent.height

  Rectangle {
    id: gzVectorContent
    width: parent.width
    // height: expand ? gzVectorGrid.height : 0
    // clip: true
    color: "transparent"

    // Behavior on height {
    //   NumberAnimation {
    //     duration: 200;
    //     easing.type: Easing.InOutQuad
    //   }
    // }

    GridLayout {
      id: gzVectorGrid
      width: parent.width
      columns: 2

      Text {
        text: gzUnit == "" ? xName : xName + ' (' + gzUnit + ')'
        // leftPadding: 5
        // color: Material.theme == Material.Light ? "#444444" : "#bbbbbb"
        // font.pointSize: 12
      }

      // Item {
      //   Layout.fillWidth: true
      //   height: 40
      //   Loader {
      //     id: xLoader
      //     anchors.fill: parent
      //     property double numberValue: xValue
      //     sourceComponent: readOnly ? readOnlyNumber : writableNumber
      //     onLoaded: {
      //       xItem = xLoader.item
      //     }
      //   }
      // }

      Text {
        text: gzUnit == "" ? yName : yName + ' (' + gzUnit + ')'
        // leftPadding: 5
        // color: Material.theme == Material.Light ? "#444444" : "#bbbbbb"
        // font.pointSize: 12
      }

      // Item {
      //   Layout.fillWidth: true
      //   height: 40
      //   Loader {
      //     id: yLoader
      //     anchors.fill: parent
      //     property double numberValue: yValue
      //     sourceComponent: readOnly ? readOnlyNumber : writableNumber
      //     onLoaded: {
      //       yItem = yLoader.item
      //     }
      //   }
      // }
    }
  }
}
