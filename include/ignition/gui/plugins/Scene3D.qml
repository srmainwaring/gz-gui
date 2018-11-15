import QtQuick 2.0
import QtQuick.Controls 2.0
import RenderWindow 1.0

Rectangle {
  width: 1000
  height: 800

  RenderWindow {
    id: renderWindow
    objectName: "rw"
    anchors.fill: parent
  }

  onParentChanged: {
    if (undefined === parent)
      return;

      width = Qt.binding(function() {return parent.parent.width})
      height = Qt.binding(function() {return parent.parent.height})
  }
}

