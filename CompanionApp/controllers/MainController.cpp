#include "MainController.h"
#include "AppSettings.h"

//#include "os/windows/WinApiWrapper.h"
#include "../hid/HidHelper.h"
#include "../hid/PotentiometersReader.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>

static constexpr auto VID = 0xFEED;
static constexpr auto PID = 0xB00B;
static constexpr auto USAGE_PAGE = 0xFF60;
static constexpr auto USAGE_ID = 0x61;


MainController::MainController(PotentiometersReader* potentiometersReader, AppSettings* appSettings, bool skipPhysicalDevice, QObject* parent)
    : QObject(parent)
    , mAppSettings(appSettings)
    , mDeviceInfoModel(new DeviceInfoModel(this))
    , mSlidersModel(new SlidersModel(this))
    , mPotentiometersReader(potentiometersReader)
    , mSkipPhysicalDevice(skipPhysicalDevice)
{
    qDebug() << "MainController::MainController";

    assert(mAppSettings && "Invalid appSettings");

    mHidHelper = new HidHelper(VID, PID, USAGE_PAGE, USAGE_ID, this);
    mHidInitted = mHidHelper->init();

    QObject::connect(mHidHelper, &HidHelper::deviceOpened, this, &MainController::onDeviceConnected);
    QObject::connect(mPotentiometersReader, &PotentiometersReader::potentiometersUpdated, this, &MainController::onPotentiometersChanged);

    for (const auto& [action, key]: mAppSettings->hotkeyActionMap()) {
        if (key != Qt::Key_unknown) {
            //WinApiWrapper::RegisterGlobalHotkey(static_cast<int>(action), key);
        }
    }
}

MainController::~MainController() {
    qDebug() << "MainController::~MainController";
}


AppSettings* MainController::getAppSettings() {
    return mAppSettings;
}

DeviceInfoModel* MainController::getDeviceInfoModel() {
    return mDeviceInfoModel;
}

SlidersModel* MainController::getSlidersModel() {
    return mSlidersModel;
}

void MainController::connectToDevice() {
    if (mSkipPhysicalDevice) {
        emit deviceConnected();
        onPotentiometersChanged({ 20, 20, 20, 20 });
        return;
    }

    if (mHidHelper) {
        mHidHelper->openDevice();
    }
}

void MainController::setIsCalibrating(bool isCalibrating) {
    mIsCalibrating = isCalibrating;
}

void MainController::onSwitchOutputClicked() {
    emit switchOutputRequested();
}

void MainController::onSliderMoved(int sliderId, int value) {
    qDebug() << "Slider " << sliderId << " moved to " << value;
}

void MainController::onDeviceConnected(hid_device* device) {
    if (device) {
        emit deviceConnected();
        mPotentiometersReader->startReading(device);
        return;
    }

    emit deviceNotFound();
}

void MainController::onPotentiometersChanged(const std::vector<int>& values) {
    emit potentiometersChanged(values);

    if (!mIsCalibrating) {
        const auto potentiometersInfo = mAppSettings->potentiometersInfo();

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
