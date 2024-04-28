#pragma once

#include <QAbstractNativeEventFilter>
#include <QCoreApplication>
#include <QObject>

#ifdef Q_OS_WIN
#include <QtCore/qt_windows.h>
#endif


enum class HotKeys;

class NativeEventFilter : public QObject
                        , public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit NativeEventFilter(QObject* parent = nullptr);
    ~NativeEventFilter();

    virtual bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;

signals:
    void hotKeyTriggered(HotKeys hotKey);

};
