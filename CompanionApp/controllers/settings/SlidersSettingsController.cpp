#include "controllers/settings/SlidersSettingsController.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>

#include <QtQml/qqmlregistration.h>


SlidersSettingsController::SlidersSettingsController(QObject *parent)
    : QObject(parent)
    , mModel(new SlidersSettingsModel(this))
    , mPotentiometersInfo(std::vector<PotentiometerInfo>(4))
{
    qDebug() << "SlidersSettingsController::SlidersSettingsController";

    qRegisterMetaType<std::vector<int>>();
}

SlidersSettingsController::~SlidersSettingsController() {
    qDebug() << "SlidersSettingsController::~SlidersSettingsController";
}

void SlidersSettingsController::init(AppSettings* appSettings) {
    mAppSettings = appSettings;

    mPotentiometersInfo = mAppSettings->potentiometersInfo();
    mCurrentValues = std::vector<int>(mPotentiometersInfo.size());
    updateModel();
}

SlidersSettingsModel* SlidersSettingsController::getModel() {
    return mModel;
}

void SlidersSettingsController::setIsCalibrating(bool isCalibrating) {
    if (mIsCalibrating != isCalibrating) {
        mIsCalibrating = isCalibrating;
        emit isCalibratingChanged(mIsCalibrating);
    }

    if (mIsCalibrating) {
        for (int i = 0; i < mPotentiometersInfo.size(); i++) {
            mPotentiometersInfo[i].min = 1023;
            mPotentiometersInfo[i].max = 0;
        }
    } else {
        mAppSettings->savePotentiometersInfo(mPotentiometersInfo);
    }
}

bool SlidersSettingsController::getIsCalibrating() {
    return mIsCalibrating;
}

void SlidersSettingsController::onPotentiometersChanged(const std::vector<int>& values) {
    if (values.size() != mPotentiometersInfo.size()) {
        mPotentiometersInfo = std::vector<PotentiometerInfo>(values.size());
    }

    mCurrentValues = values;

    if (mIsCalibrating) {
        handleCalibration(values);
    }

    updateModel();
}

void SlidersSettingsController::updateModel() {
    std::vector<SlidersSettingsModel::Row> model;
    model.reserve(mPotentiometersInfo.size());

    for (int i = 0; i < mPotentiometersInfo.size(); i++) {
        SlidersSettingsModel::Row row;
        row[SlidersSettingsModel::IdRole] = i;
        row[SlidersSettingsModel::MinRole] = mPotentiometersInfo[i].min;
        row[SlidersSettingsModel::MaxRole] = mPotentiometersInfo[i].max;
        row[SlidersSettingsModel::ValueRole] = mCurrentValues[i];
        model.push_back(row);
    }

    mModel->updateData(model);
}

void SlidersSettingsController::handleCalibration(const std::vector<int>& values) {
    for (int i = 0; i < mPotentiometersInfo.size(); i++) {
        if (values[i] >= 0 && values[i] < mPotentiometersInfo[i].min) mPotentiometersInfo[i].min = values[i];
        if (values[i] >= 0 && values[i] > mPotentiometersInfo[i].max) mPotentiometersInfo[i].max = values[i];
    }
}
