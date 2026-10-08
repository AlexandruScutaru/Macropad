import QtQuick
import QtQuick.Layouts

import Macropad.AppContext

Item {
    id: keypadLayers

    KeypadController {
        id: controller
        appContext: AppContext
    }

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

            model: controller.model
            outlineColor: controller.layerColor ?? "transparent"

            onActionAssigned: (key, actionId) => {
                controller.assignAction(key, actionId);
            }

            onKeySelected: (key) => {
                controller.onKeySelected(key);
            }

            onKeyTriggered: (key) => {
                controller.onKeyTriggered(key);
            }
        }

        LayerPagination {
            id: layerPagination

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter

            pageCount: controller.layerCount
            currentPage: controller.currentLayer

            onPageChanged: (page) => {
                controller.currentLayer = page;
            }
        }
    }
}
