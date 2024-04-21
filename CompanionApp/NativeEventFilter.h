#pragma once

#include <QAbstractNativeEventFilter>
#include <QCoreApplication>
#include <QObject>

#ifdef Q_OS_WIN
#include <QtCore/qt_windows.h>
#endif


class NativeEventFilter : public QObject
                        , public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) Q_DECL_OVERRIDE;

signals:
    void hotKeyTrigerred();

};
