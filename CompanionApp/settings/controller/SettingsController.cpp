#include "SettingsController.h"
#include "theming/Theme.h"
#include "theming/ThemeLoader.h"

#include <QDebug>
#include <QVariantMap>

using namespace settings;


SettingsController::SettingsController(QObject* parent)
    : QObject(parent)
{
    qDebug() << "SettingsController::SettingsController";

    mTabsListModel = createTabsListModel();
    mThemesListModel = createThemesListModel();
}

SettingsController::~SettingsController() {
    qDebug() << "SettingsController::~SettingsController";
}


void SettingsController::changeTheme(const QString& name) {
    emit changeThemeRequested(name);
}

QString SettingsController::currentTheme() {
    return mCurrentTheme;
}

void SettingsController::setCurrentTheme(const QString& name) {
    if (mCurrentTheme == name) {
        return;
    }

    mCurrentTheme = name;
    emit currentThemeChanged(mCurrentTheme);
}

TabsListModel* SettingsController::tabsListModel() {
    return mTabsListModel;
}

ThemesListModel* SettingsController::themesListModel() {
    return mThemesListModel;
}

TabsListModel* SettingsController::createTabsListModel() {
    QString urlTemplate = QString("/qt/qml/MacropadCompanion/%1.qml");

    using TabEntry = std::tuple<QString, QString>;
    std::vector<TabEntry> availableTabs = {
        { QObject::tr("Appearance"), urlTemplate.arg("Appearance") },
    };

    QList<QMap<int, QVariant>> tabsModel;
    for (auto& [name, url]: availableTabs) {
        QMap<int, QVariant> tabRow;
        tabRow[TabsListModel::Name] = name;
        tabRow[TabsListModel::Url] = url;
        tabsModel.push_back(tabRow);
    }

    const auto tabsListModel = new TabsListModel(this);
    tabsListModel->setData(tabsModel);
    
    return tabsListModel;
}

ThemesListModel* SettingsController::createThemesListModel() {
    using ThemeEntry = std::tuple<QString, theme::Theme*>;

    std::vector<ThemeEntry> availableThemes = {
        { "dark", theme::Loader::Load(theme::Type::Dark) },
        { "light", theme::Loader::Load(theme::Type::Light) },
    };

    QList<QMap<int, QVariant>> themesModel;
    for (auto& [name, colors]: availableThemes) {
        QMap<int, QVariant> themeRow;
        themeRow[ThemesListModel::Name] = name;
        themeRow[ThemesListModel::Colors] = QVariant::fromValue(colors);
        themesModel.push_back(themeRow);
    }

    const auto themesListModel = new ThemesListModel(this);
    themesListModel->setData(themesModel);
    
    return themesListModel;
}