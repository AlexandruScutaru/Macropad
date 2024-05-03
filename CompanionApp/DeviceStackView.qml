import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15


Item {
    id: root

    anchors.fill: parent
    anchors.margins: 16

    required property DeviceController controller

    StackView {
        id: stack

        anchors.fill: parent

        initialItem: connectView
        clip: true

        Connections {
            target: controller
            function onDeviceConnected() {
                stack.push(dashboardView)
            }
        }

        Component {
            id: connectView
            DeviceConnectView {}
        }

        Component {
            id: dashboardView
            DeviceDashboardView {}
        }
    }
}
