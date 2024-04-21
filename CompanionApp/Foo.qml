import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic


Item {
    height: content.height

    ColumnLayout {
        id: content
        width: parent.width

        ComboBox {
            Layout.fillWidth: true
            editable: false

            model: ListModel {
                id: model
                ListElement { text: "Device 1" }
                ListElement { text: "Device 2" }
                ListElement { text: "Device 3" }
            }

            onActivated: (index) => {}
        }
    }
}
