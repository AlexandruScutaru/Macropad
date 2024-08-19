#include "MainController.h"
#include "Config.h"

#include "os/windows/WinApiWrapper.h"
#include "../hid/HidHelper.h"
#include "../hid/PotentiometersReader.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>


MainController::MainController(PotentiometersReader* potentiometersReader, Config* config, QObject* parent)
    : QObject(parent)
    , mConfig(config)
    , mDeviceInfoModel(new DeviceInfoModel(this))
    , mSlidersModel(new SlidersModel(this))
    , mPotentiometersReader(potentiometersReader)
{
    qDebug() << "MainController::MainController";

    assert(mConfig && "Invalid config");

    mHidHelper = new HidHelper(this);
    mHidInitted = mHidHelper->init();

    QObject::connect(this, &MainController::deviceOpened, mPotentiometersReader, &PotentiometersReader::startReading);
    QObject::connect(mPotentiometersReader, &PotentiometersReader::potentiometersUpdated, this, &MainController::onPotentiometersChanged);

    for (const auto& [action, key]: mConfig->hotkeyActionMap()) {
        if (key != Qt::Key_unknown) {
            WinApiWrapper::RegisterGlobalHotkey(static_cast<int>(action), key);
        }
    }
}

MainController::~MainController() {
    qDebug() << "MainController::~MainController";
}


Config* MainController::getConfig() {
    return mConfig;
}

DeviceInfoModel* MainController::getDeviceInfoModel() {
    return mDeviceInfoModel;
}

SlidersModel* MainController::getSlidersModel() {
    return mSlidersModel;
}

void MainController::search(const QString& vid, const QString& pid) {
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

void MainController::openDevice(const QString& path) {
    if (auto device = mHidHelper->openDevice(path.toStdString()); device) {
        mConfig->saveLastDevicePath(path);
        emit deviceOpened(device);
        emit deviceConnected();
        return;
    }

    mConfig->clearLastDevicePath();
    emit noDeviceSaved();
}

void MainController::openLastDevice() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

    if (const auto path = mConfig->lastDevicePath(); !path.isEmpty()) {
        openDevice(path);
        return;
    }

    emit noDeviceSaved();
}

void MainController::onSwitchOutputClicked() {
    emit switchOutputRequested();
}

void MainController::onSliderMoved(int sliderId, int value) {
    qDebug() << "Slider " << sliderId << " moved to " << value;
}

void MainController::setIsCalibrating(bool isCalibrating) {
    mIsCalibrating = isCalibrating;
}

void MainController::onPotentiometersChanged(const std::vector<int>& values) {
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

        std::vector<SlidersModel::Row> model;
        model.reserve(normalizedValues.size());

        for(int i = 0; i < normalizedValues.size(); i++) {
            SlidersModel::Row row;
            row[SlidersModel::IdRole] = i;
            row[SlidersModel::ValueRole] = normalizedValues[i];
            model.push_back(row);
        }

        mSlidersModel->updateData(model);
    }
}

int MainController::convertToInt(const QString& hexStr) {
    if (hexStr.isEmpty()) {
        return 0;
    }

    bool res = false;
    int value = hexStr.toInt(&res, 16);
    assert(res && "couldn't convert hex str to int");

    return value;
}
