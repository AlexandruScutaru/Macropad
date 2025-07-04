#pragma once

#include "AppSettings.h"
#include "models/settings/SlidersSettingsModel.h"

#include <QObject>
#include <QPointer>
#include <QQmlEngine>

#include <vector>


class SlidersSettingsController : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isCalibrating READ getIsCalibrating NOTIFY isCalibratingChanged)
    QML_ELEMENT
public:
    explicit SlidersSettingsController(QObject *parent = nullptr);
    ~SlidersSettingsController();

    Q_INVOKABLE void init(AppSettings* appSettings);
    Q_INVOKABLE SlidersSettingsModel* getModel();
    Q_INVOKABLE void setIsCalibrating(bool isCalibrating);

    bool getIsCalibrating();

signals:
    void isCalibratingChanged(bool isCalibrating);
    void slidersChanged(const std::vector<int>& values);

public slots:
    void onPotentiometersChanged(const std::vector<int>& values);

private:
    void updateModel();
    void handleCalibration(const std::vector<int>& rawValues);

    QPointer<AppSettings> mAppSettings{ nullptr };
    QPointer<SlidersSettingsModel> mModel{ nullptr };

    std::vector<PotentiometerInfo> mPotentiometersInfo;
    std::vector<int> mCurrentValues;
    bool mIsCalibrating{ false };

};
