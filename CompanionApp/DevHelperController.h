#pragma once

#include <QObject>
#include <QPointer>
#include <QQmlEngine>

#include "DevHelperModel.h"


class DevHelperController : public QObject {
    Q_OBJECT
    QML_ELEMENT
public:
    explicit DevHelperController(QObject* parent = nullptr);
    ~DevHelperController();

    Q_INVOKABLE DevHelperModel* getModel();
    Q_INVOKABLE void onSwitchOutputClicked();
    Q_INVOKABLE void onSliderMoved(int sliderId, int value);

public slots:
    void onSlidersUpdated(const std::vector<int>& values);

signals:
    void switchOutputRequested();

private:
    QString mSliderLabel;
    QPointer<DevHelperModel> mModel{ nullptr };

};
