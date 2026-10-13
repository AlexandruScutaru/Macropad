#include "AppController.h"
#include "NavigationBarListModel.h"
#include "modules/settings/service/SettingsService.h"
#include "shared/hid/DeviceService.h"

#include <QDebug>
#include <QVariantMap>

#include <cassert>
#include <tuple>


static constexpr auto TAB_ICON_URL_TEMPLATE = "qrc:///resources/icons/%1.svg";
static constexpr auto KEYPAD_TAB_NAME = "keypad";
static constexpr auto SLIDERS_TAB_NAME = "sliders";
static constexpr auto SETTINGS_TAB_NAME = "settings";

using namespace settings;

enum Animation {
    None = 0,
    Scale,
    Rotate
};

AppController::AppController(QObject* parent)
    : QObject(parent)
{
    qDebug() << "AppController::AppController";

    createNavBarModel();
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

NavigationBarListModel* AppController::navBarModel() {
    return mNavBarModel;
}

void AppController::connectToDevice() {
    if (!mHidDeviceService) {
        return;
    }

    mHidDeviceService->connectToDevice();
}

void AppController::navigationRequested(const QString& tabName) {
    if (tabName == SETTINGS_TAB_NAME) {
        emit openSettings();
        return;
    }

    if (tabName == KEYPAD_TAB_NAME) {
        emit switchTab(0, KEYPAD_TAB_NAME);
        return;
    }

    if (tabName == SLIDERS_TAB_NAME) {
        emit switchTab(1, SLIDERS_TAB_NAME);
        return;
    }
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
    assert(mHidDeviceService && "DeviceService is null");

    QObject::connect(mHidDeviceService, &hid::DeviceService::deviceConnected, this, [this]() {
        setTabButtonState(KEYPAD_TAB_NAME, true);
        emit deviceConnected();
        emit switchTab(0, KEYPAD_TAB_NAME);
    });

    QObject::connect(mHidDeviceService, &hid::DeviceService::deviceNotFound, this, [this]() {
        emit deviceNotFound();
        setTabButtonState(KEYPAD_TAB_NAME, false);
        setTabButtonState(SLIDERS_TAB_NAME, false);
    });

    QObject::connect(mHidDeviceService, &hid::DeviceService::slidersModuleStateChanged, this, [this](auto connected) {
        setTabButtonState(SLIDERS_TAB_NAME, connected);
        emit switchTab(0, KEYPAD_TAB_NAME);
    });
}

void AppController::setTabButtonState(const QString& name, bool enabled) {
    if (!mNavBarModel) {
        return;
    }

    QModelIndex modelIndex;
    size_t i = 0;
    for (; i < mNavBarModel->getCount(); i++) {
        auto index = mNavBarModel->index(i, 0);
        if (mNavBarModel->data(index, NavigationBarListModel::Name).toString() == name) {
            modelIndex = index;
            break;
        }
    }

    if (!modelIndex.isValid()) {
        return;
    }

    QMap<int, QVariant> row;
    row[NavigationBarListModel::Type] = mNavBarModel->data(modelIndex, NavigationBarListModel::Type);
    row[NavigationBarListModel::Name] = QString(name);
    row[NavigationBarListModel::DisplayName] = mNavBarModel->data(modelIndex, NavigationBarListModel::DisplayName);
    row[NavigationBarListModel::IconSource] = mNavBarModel->data(modelIndex, NavigationBarListModel::IconSource);
    row[NavigationBarListModel::Enabled] = enabled;
    row[NavigationBarListModel::Checkable] = true;
    row[NavigationBarListModel::Animation] = mNavBarModel->data(modelIndex, NavigationBarListModel::Animation);

    mNavBarModel->updateRow(i, row);
}

void AppController::createNavBarModel() {
    mNavBarModel = new NavigationBarListModel(this);

    using TabInfo = std::tuple<QString, QString, QString, QString, bool, bool, int>;
    const std::vector<TabInfo> tabs = {
        {"button", KEYPAD_TAB_NAME, tr("Keypad"), "keys", false, true, Animation::Scale},
        {"button", SLIDERS_TAB_NAME, tr("Sliders"), "sliders", false, true, Animation::Scale},
        {"spacer", "", "", "", false, false, Animation::None},
        {"button", SETTINGS_TAB_NAME, tr("Settings"), "settings", true, false, Animation::Rotate},
    };

    QList<QMap<int, QVariant>> model;
    model.reserve(tabs.size());

    for (const auto& [type, name, displayName, icon, enabled, checkable, animation]: tabs) {
        QMap<int, QVariant> row;
        row[NavigationBarListModel::Type] = type;
        row[NavigationBarListModel::Name] = name;
        row[NavigationBarListModel::DisplayName] = displayName;
        row[NavigationBarListModel::IconSource] = QString(TAB_ICON_URL_TEMPLATE).arg(icon);
        row[NavigationBarListModel::Enabled] = enabled;
        row[NavigationBarListModel::Checkable] = checkable;
        row[NavigationBarListModel::Animation] = animation;
        model.push_back(row);
    }

    mNavBarModel->setData(model);
    emit navBarModelChanged(mNavBarModel);
}
