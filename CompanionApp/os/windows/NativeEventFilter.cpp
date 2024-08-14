#include "NativeEventFilter.h"
#include "os/windows/WinApiWrapper.h"

#include <QDebug>


NativeEventFilter::NativeEventFilter(QObject* parent)
    : QObject(parent)
{
    qDebug() << "NativeEventFilter::NativeEventFilter";
}

NativeEventFilter::~NativeEventFilter() {
    qDebug() << "NativeEventFilter::~NativeEventFilter";
}


bool NativeEventFilter::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) {
    Q_UNUSED(eventType);
    Q_UNUSED(result);

    MSG* msg = static_cast<MSG*>(message);
    if (msg->message == WM_HOTKEY) {
        if (const auto key = WinApiWrapper::ParseHotkeyMessageParam(msg->lParam)) {
            emit hotkeyTriggered(key);
            return true;
        }
    }

    return false;
}
