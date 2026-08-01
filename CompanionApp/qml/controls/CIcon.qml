import QtQuick
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
    property int iconSize: 32
    property int iconWidth: iconSize
    property int iconHeight: iconSize

    property int animationType: CIcon.AnimationType.None
    property bool toggleAnimation: false
    property bool flipIcon: false
    property bool isAnimationForward

    width: iconWidth
    height: iconHeight

    rotation: icon.flipIcon ? 180 : 0

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
        duration: 100
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
                scaleAnimation.from = icon.scale;
                scaleAnimation.to = 0.9;
                scaleAnimation.start();
                break;
            case CIcon.AnimationType.Rotate:
                rotateAnimation.stop();
                rotateAnimation.from = icon.rotation;
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
                scaleAnimation.from = icon.scale;
                scaleAnimation.to = 1.0;
                scaleAnimation.start();
                break;
            case CIcon.AnimationType.Rotate:
                rotateAnimation.stop();
                rotateAnimation.from = icon.rotation;
                rotateAnimation.to = 0;
                rotateAnimation.start();
                break;
        }
    }
}
