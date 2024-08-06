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

        clip: true

        Component.onCompleted: {
            controller.openLastDevice();
        }

        Connections {
            target: controller
            function onDeviceConnected() {
                stack.replace(dashboardView);
            }

            function onNoDeviceSaved() {
                stack.replace(connectView);
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
