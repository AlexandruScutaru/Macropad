import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15


Item {
    id: root

    anchors.fill: parent
    anchors.margins: 16

    required property DeviceController controller

    DeviceConnectView {}
}
