#include "DeviceController.h"
#include "../hid/HidHelper.h"
#include "../hid/PotentiometersReader.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>


DeviceController::DeviceController(PotentiometersReader* potentiometersReader, QObject* parent)
    : QObject(parent)
    , mPotentiometersReader(potentiometersReader)
    , mDeviceInfoModel(new DeviceInfoModel(this))
    , mDeviceCalibrationModel(new DeviceCalibrationModel(this))
    , mHotkeysController(new HotkeysController(this))
    , mPotentiometersInfo(std::vector<PotentiometerInfo>(4))
{
    qDebug() << "DeviceController::DeviceController";

    mHidHelper = new HidHelper(this);
    mHidInitted = mHidHelper->init();

    readCalibrationInfo();

    QObject::connect(this, &DeviceController::hotkeyTriggered, mHotkeysController, &HotkeysController::onHotKeyTriggered);
    QObject::connect(mHotkeysController, &HotkeysController::hotkeyActionTriggered, this, &DeviceController::hotkeyActionTriggered);

    QObject::connect(this, &DeviceController::deviceOpened, mPotentiometersReader, &PotentiometersReader::startReading);
    QObject::connect(mPotentiometersReader, &PotentiometersReader::potentiometersUpdated, this, &DeviceController::onPotentiometersUpdated);
}

DeviceController::~DeviceController() {
    qDebug() << "DeviceController::~DeviceController";
}

DeviceInfoModel* DeviceController::getDeviceInfoModel() {
    return mDeviceInfoModel;
}

DeviceCalibrationModel* DeviceController::getDeviceCalibrationModel() {
    return mDeviceCalibrationModel;
}

HotkeysController* DeviceController::getHotkeysController() {
    return mHotkeysController;
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
        saveConnectedDevicePath(path);

        emit deviceOpened(device);
        emit deviceConnected();
        return;
    }

    clearConnectedDevicePath();
    emit noDeviceSaved();
}

void DeviceController::saveConnectedDevicePath(const QString& path) {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
    settings.setValue("device/lastDevicePath", path);
}

void DeviceController::clearConnectedDevicePath() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
    settings.remove("device/lastDevicePath");
}

void DeviceController::saveCalibrationInfo() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
    settings.beginGroup("device");
    settings.beginWriteArray("sliders");
    for (int i = 0; i < mPotentiometersInfo.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("min", mPotentiometersInfo[i].min);
        settings.setValue("max", mPotentiometersInfo[i].max);
    }
    settings.endArray();
    settings.endGroup();
}

void DeviceController::readCalibrationInfo() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
    settings.beginGroup("device");

    auto size = settings.beginReadArray("sliders");
    mPotentiometersInfo.clear();
    mPotentiometersInfo.resize(size);
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        auto min = settings.value("min", 0).toInt();
        auto max = settings.value("max", 1023).toInt();
        mPotentiometersInfo[i].min = min;
        mPotentiometersInfo[i].max = max;
    }
    settings.endArray();
    settings.endGroup();
}

void DeviceController::onPotentiometersUpdated(const std::vector<int>& values) {
    if (values.size() != mPotentiometersInfo.size()) {
        mPotentiometersInfo = std::vector<PotentiometerInfo>(values.size());
    }

    for (int i = 0; i < mPotentiometersInfo.size(); i++) {
        mPotentiometersInfo[i].value = values[i];
    }

    if (mIsCalibrating) {
        handleCalibration(values);
    } else {
        std::vector<int> normalizedValues;
        normalizedValues.reserve(mPotentiometersInfo.size());
        std::transform(mPotentiometersInfo.begin(), mPotentiometersInfo.end(), std::back_inserter(normalizedValues), [](const auto& pot) {
            return std::clamp(static_cast<int>((pot.value - pot.min) / static_cast<double>(pot.max - pot.min) * 100), 0, 100);
        });

        emit slidersChanged(normalizedValues);
    }
}

void DeviceController::setIsCalibrating(bool isCalibrating) {
    mIsCalibrating = isCalibrating;

    if (mIsCalibrating) {
        std::vector<int> currentValues(mPotentiometersInfo.size());
        for (int i = 0; i < mPotentiometersInfo.size(); i++) {
            mPotentiometersInfo[i].min = 1023;
            mPotentiometersInfo[i].max = 0;
            currentValues[i] = mPotentiometersInfo[i].value;
        }
        handleCalibration(currentValues);
    } else {
        saveCalibrationInfo();
    }
}

void DeviceController::openLastDevice() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

    if (const auto path = settings.value("device/lastDevicePath", "").toString(); !path.isEmpty()) {
        openDevice(path);
        return;
    }

    emit noDeviceSaved();
}

void DeviceController::handleCalibration(const std::vector<int>& values) {
    mDeviceCalibrationModel->reset();
    std::vector<DeviceCalibrationModel::Row> model;
    model.reserve(mPotentiometersInfo.size());

    for (int i = 0; i < mPotentiometersInfo.size(); i++) {
        if (values[i] >= 0 && values[i] < mPotentiometersInfo[i].min) mPotentiometersInfo[i].min = values[i];
        if (values[i] >= 0 && values[i] > mPotentiometersInfo[i].max) mPotentiometersInfo[i].max = values[i];

        DeviceCalibrationModel::Row row;
        row[DeviceCalibrationModel::IdRole] = i;
        row[DeviceCalibrationModel::MinRole] = mPotentiometersInfo[i].min;
        row[DeviceCalibrationModel::MaxRole] = mPotentiometersInfo[i].max;
        row[DeviceCalibrationModel::ValueRole] = mPotentiometersInfo[i].value;
        model.push_back(row);
    }

    mDeviceCalibrationModel->setData(model);
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
