import QtQuick

Item {
    id: focusOutline

    property var target: null
    // to be added from theme side
    property color color: "#5DADE2"
    property int borderWidth: 2
    property bool visibleWhenFocused: true

    property int radius: 6
    property var topLeft
    property var topRight
    property var bottomLeft
    property var bottomRight

    anchors.fill: target
    visible: target && visibleWhenFocused && target.activeFocus && isKeyboardNavigationFocus(target.focusReason)
    z: 999

    function isKeyboardNavigationFocus(reason) {
        return reason === Qt.TabFocusReason ||
               reason === Qt.BacktabFocusReason ||
               reason === Qt.ShortcutFocusReason;
    }

    Rectangle {
        anchors.fill: parent

        color: "transparent"
        border.color: focusOutline.color
        border.width: focusOutline.borderWidth
        radius: focusOutline.radius

        topLeftRadius: focusOutline.topLeft ? focusOutline.topLeft : focusOutline.radius
        topRightRadius: focusOutline.topRight ? focusOutline.topRight : focusOutline.radius
        bottomLeftRadius: focusOutline.bottomLeft ? focusOutline.bottomLeft : focusOutline.radius
        bottomRightRadius: focusOutline.bottomRight ? focusOutline.bottomRight : focusOutline.radius
    }
}
