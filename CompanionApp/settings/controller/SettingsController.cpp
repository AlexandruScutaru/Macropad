#include "SettingsController.h"

#include <QDebug>
#include <QVariantMap>
#include <cassert>


static constexpr auto VIEW_URL_TEMPLATE = "/qt/qml/MacropadCompanion/%1.qml";

using namespace settings;

SettingsController::SettingsController(QObject* parent)
    : QObject(parent)
    , mTabsListModel(new TabsListModel(this))
    , mThemesListModel(new ThemesListModel(this))
{
    qDebug() << "SettingsController::SettingsController";
}

SettingsController::~SettingsController() {
    qDebug() << "SettingsController::~SettingsController";
}


QString SettingsController::currentTheme() {
    return mCurrentTheme;
}

TabsListModel* SettingsController::tabsListModel() {
    return mTabsListModel;
}

ThemesListModel* SettingsController::themesListModel() {
    return mThemesListModel;
}

void SettingsController::setAppContext(IAppContext* appContext) {
    mSettingsService = appContext->settingsService();

    // maybe set some signal/slot handling to get service updates
    onCurrentThemeChanged(mSettingsService->getCurrentTheme());
    onTabsListChanged(mSettingsService->getAvailableTabs());
    onThemesListChanged(mSettingsService->getAvailableThemes());
}

void SettingsController::changeTheme(const QString& name) {
    mSettingsService->changeTheme(name);
    onCurrentThemeChanged(name);
}


void SettingsController::onCurrentThemeChanged(const QString& name) {
    if (mCurrentTheme == name) {
        return;
    }

    mCurrentTheme = name;
    emit currentThemeChanged(mCurrentTheme);
}

void SettingsController::onTabsListChanged(const QStringList& tabs) {
    QList<QMap<int, QVariant>> tabsModel;
    tabsModel.reserve(tabs.length());

    for (const auto& tab: tabs) {
        QMap<int, QVariant> tabRow;
        tabRow[TabsListModel::Name] = tab;
        tabRow[TabsListModel::Url] = QString(VIEW_URL_TEMPLATE).arg(tab);
        tabsModel.push_back(tabRow);
    }

    mTabsListModel->updateData(tabsModel);
}

void SettingsController::onThemesListChanged(const std::vector<ThemeEntry>& themes) {
    QList<QMap<int, QVariant>> themesModel;
    themesModel.reserve(themes.size());

    for (const auto& [name, colors]: themes) {
        QMap<int, QVariant> themeRow;
        themeRow[ThemesListModel::Name] = name;
        themeRow[ThemesListModel::Colors] = colors;
        themesModel.push_back(themeRow);
    }

    mThemesListModel->setData(themesModel);
}
