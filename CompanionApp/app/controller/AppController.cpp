#include "AppController.h"
#include "modules/settings/service/SettingsService.h"
#include "shared/hid/HidDeviceService.h"

#include <QDebug>
#include <QVariantMap>
#include <cassert>


using namespace settings;

AppController::AppController(QObject* parent)
    : QObject(parent)
{
    qDebug() << "AppController::AppController";   
}

AppController::~AppController() {
    qDebug() << "AppController::~AppController";
}


QSize AppController::windowSize() {
    return mWindowSize;
}

void AppController::setWindowSize(const QSize& size) {
    if (mWindowSize == size) {
        return;
    }

    mWindowSize = size;
    emit windowSizeChanged(mWindowSize);

    if (mSettingsService) mSettingsService->setWindowSize(size);
}

bool AppController::navBarExpanded() {
    return mNavBarExpanded;
}

void AppController::setNavBarExpanded(bool expanded) {
    if (mNavBarExpanded == expanded) {
        return;
    }

    mNavBarExpanded = expanded;
    emit navBarExpandedChanged(mNavBarExpanded);

    if (mSettingsService) mSettingsService->setNavBarExpanded(expanded);
}

void AppController::connectToDevice() {
    mHidDeviceService->connectToDevice();
}

void AppController::setAppContext(IAppContext* appContext) {
    mAppContext = appContext;
    mSettingsService = mAppContext->settingsService();
    
    QObject::connect(mSettingsService, &settings::SettingsService::showWindowRequested, this, &AppController::showWindowRequested);
    
    mWindowSize = mSettingsService->getWindowSize();
    emit windowSizeChanged(mWindowSize);
    
    mNavBarExpanded = mSettingsService->getNavBarExpanded();
    emit navBarExpandedChanged(mNavBarExpanded);

    mHidDeviceService = mAppContext->hidDeviceService();
    assert(mHidDeviceService && "HidDeviceService is null");

    QObject::connect(mHidDeviceService, &hid::DeviceService::deviceConnected, this, &AppController::deviceConnected);
    QObject::connect(mHidDeviceService, &hid::DeviceService::deviceNotFound, this, &AppController::deviceNotFound);
}
