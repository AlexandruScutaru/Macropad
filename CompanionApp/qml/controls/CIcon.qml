import QtQuick 2.15
import Qt5Compat.GraphicalEffects

Item {
    id: icon

    enum AnimationType {
        None,
        Scale,
        Rotate
    }

    property string source
    property color color: Theme.textPrimary
    property int iconSize: 40
    property int iconWidth: iconSize
    property int iconHeight: iconSize

    property int animationType: CIcon.AnimationType.None
    property bool toggleAnimation: false
    property bool flipIcon: false
    property bool isAnimationForward

    width: iconWidth
    height: iconHeight

    property alias iconAngle: rotation.angle
    property int initialAngle: { initialAngle = (icon.flipIcon ? 180 : 0) }

    transform: Rotation {
        id: rotation
        origin.x: icon.width / 2;
        origin.y: icon.height / 2;
        angle: icon.initialAngle
    }

    Component.onCompleted: {
        isAnimationForward = !flipIcon;
        isAnimationForward = isAnimationForward;
    }

    Image {
        id: iconImage
        anchors.fill: parent
        source: icon.source
    }

    ColorOverlay {
        anchors.fill: iconImage
        source: iconImage
        color: icon.color
    }

    PropertyAnimation {
        id: scaleAnimation
        target: icon
        property: "scale"
        duration: 200
        easing.type: Easing.InQuad
    }

    RotationAnimator {
        id: rotateAnimation

        target: icon
        duration: 200
        easing.type: Easing.InQuad

        onFinished: {
            icon.isAnimationForward = !icon.isAnimationForward
        }
    }

    function onPressed() {
        if (toggleAnimation) {
            return;
        }

        animateForward();
    }

    function onReleased() {
        if (toggleAnimation) {
            return;
        }

        animateBackward();
    }

    function onClicked() {
        if (animationType == CIcon.AnimationType.None || !toggleAnimation) {
            return;
        }

        isAnimationForward ? animateForward() : animateBackward();
    }

    function animateForward() {
        switch (animationType) {
            case CIcon.AnimationType.None:
                return;
            case CIcon.AnimationType.Scale:
                scaleAnimation.stop();
                scaleAnimation.to = 0.9;
                scaleAnimation.start();
                break;
            case CIcon.AnimationType.Rotate:
                rotateAnimation.stop();
                icon.iconAngle = 0;
                rotateAnimation.from = 0;
                rotateAnimation.to = 180;
                rotateAnimation.start();
                break;
        }
    }

    function animateBackward() {
        switch (animationType) {
            case CIcon.AnimationType.None:
                return;
            case CIcon.AnimationType.Scale:
                scaleAnimation.stop();
                scaleAnimation.to = 1.0;
                scaleAnimation.start();
                break;
            case CIcon.AnimationType.Rotate:
                rotateAnimation.stop();
                icon.iconAngle = 0;
                rotateAnimation.from = 180;
                rotateAnimation.to = 0;
                rotateAnimation.start();
                break;
        }
    }
}
