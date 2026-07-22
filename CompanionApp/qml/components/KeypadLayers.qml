import QtQuick
import QtQuick.Layouts

Item {
    id: keypadLayers

    required property KeypadController controller

    ColumnLayout {
        id: actionAssignLayout

        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        KeysGrid {
            id: keypadKeys

            readonly property int minSize: 150
            readonly property int maxSize: 350

            Layout.minimumHeight: minSize
            Layout.minimumWidth: minSize
            Layout.maximumHeight: maxSize
            Layout.maximumWidth: maxSize
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter

            model: keypadLayers.controller.model
            outlineColor: keypadLayers.controller.layerColor ?? "transparent"

            onActionAssigned: (key, actionId) => {
                keypadLayers.controller.assignAction(key, actionId);
            }

            onKeySelected: (key) => {
                keypadLayers.controller.onKeySelected(key);
            }

            onKeyTriggered: (key) => {
                keypadLayers.controller.onKeyTriggered(key);
            }
        }

        LayerPagination {
            id: layerPagination

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter

            pageCount: keypadLayers.controller.layerCount
            currentPage: keypadLayers.controller.currentLayer

            onPageChanged: (page) => {
                keypadLayers.controller.currentLayer = page;
            }
        }
    }
}
