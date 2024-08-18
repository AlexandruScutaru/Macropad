#include "DeviceController.h"
#include "Config.h"

#include "os/windows/WinApiWrapper.h"
#include "../hid/HidHelper.h"
#include "../hid/PotentiometersReader.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>


DeviceController::DeviceController(PotentiometersReader* potentiometersReader, Config* config, QObject* parent)
    : QObject(parent)
    , mConfig(config)
    , mDeviceInfoModel(new DeviceInfoModel(this))
    , mPotentiometersReader(potentiometersReader)
{
    qDebug() << "DeviceController::DeviceController";

    assert(mConfig && "Invalid config");

    mHidHelper = new HidHelper(this);
    mHidInitted = mHidHelper->init();

    QObject::connect(this, &DeviceController::deviceOpened, mPotentiometersReader, &PotentiometersReader::startReading);
    QObject::connect(mPotentiometersReader, &PotentiometersReader::potentiometersUpdated, this, &DeviceController::onPotentiometersChanged);

    for (const auto& [action, key]: mConfig->hotkeyActionMap()) {
        if (key != Qt::Key_unknown) {
            WinApiWrapper::RegisterGlobalHotkey(static_cast<int>(action), key);
        }
    }
}

DeviceController::~DeviceController() {
    qDebug() << "DeviceController::~DeviceController";
}

DeviceInfoModel* DeviceController::getDeviceInfoModel() {
    return mDeviceInfoModel;
}

Config* DeviceController::getConfig() {
    return mConfig;
}

void DeviceController::setIsCalibrating(bool isCalibrating) {
    mIsCalibrating = isCalibrating;
}

void DeviceController::search(const QString& vid, const QString& pid) {
    const auto info = mHidHelper->enumerateDevices(convertToInt(vid), convertToInt(pid));

    mDeviceInfoModel->reset();

    std::vector<DeviceInfoModel::Row> model;
    model.reserve(info.size());

    for(int i = 0; i < info.size(); i++) {
        DeviceInfoModel::Row row;
        row[DeviceInfoModel::Vid] = QString("0x%1").arg(info[i].vid, 4, 16, QLatin1Char('0'));
        row[DeviceInfoModel::Pid] = QString("0x%1").arg(info[i].pid, 4, 16, QLatin1Char('0'));
        row[DeviceInfoModel::UsagePage] = QString("0x%1").arg(info[i].usagePage, 4, 16, QLatin1Char('0'));
        row[DeviceInfoModel::UsageId] = QString("0x%1").arg(info[i].usageId, 4, 16, QLatin1Char('0'));
        row[DeviceInfoModel::Product] = QString::fromStdString(info[i].product);
        row[DeviceInfoModel::Manufacturer] = QString::fromStdString(info[i].manufacturer);
        row[DeviceInfoModel::Serial] = QString::fromStdString(info[i].serial);
        row[DeviceInfoModel::Path] = QString::fromStdString(info[i].path);
        model.push_back(row);
    }

    mDeviceInfoModel->setData(model);
}

void DeviceController::openDevice(const QString& path) {
    if (auto device = mHidHelper->openDevice(path.toStdString()); device) {
        mConfig->saveLastDevicePath(path);
        emit deviceOpened(device);
        emit deviceConnected();
        return;
    }

    mConfig->clearLastDevicePath();
    emit noDeviceSaved();
}

void DeviceController::openLastDevice() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

    if (const auto path = mConfig->lastDevicePath(); !path.isEmpty()) {
        openDevice(path);
        return;
    }

    emit noDeviceSaved();
}

void DeviceController::onPotentiometersChanged(const std::vector<int>& values) {
    emit potentiometersChanged(values);

    if (!mIsCalibrating) {
        const auto potentiometersInfo = mConfig->potentiometersInfo();

        if (potentiometersInfo.size() != values.size()) {
            return;
        }

        std::vector<int> normalizedValues(potentiometersInfo.size());
        for(auto i = 0; i < potentiometersInfo.size(); i++) {
            normalizedValues[i] = std::clamp(static_cast<int>((values[i] - potentiometersInfo[i].min) / static_cast<double>(potentiometersInfo[i].max - potentiometersInfo[i].min) * 100), 0, 100);
        }

        emit slidersChanged(normalizedValues);
    }
}

int DeviceController::convertToInt(const QString& hexStr) {
    if (hexStr.isEmpty()) {
        return 0;
    }

    bool res = false;
    int value = hexStr.toInt(&res, 16);
    assert(res && "couldn't convert hex str to int");

    return value;
}
