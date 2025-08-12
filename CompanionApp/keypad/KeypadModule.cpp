#include "KeypadModule.h"
#include "controller/KeypadController.h"
#include "service/KeypadService.h"
#include "AppSettings.h"

#include <QDebug>

#include <algorithm>


#define ACTION(displayName, tooltip, icon, sectionName, actionName) \
    {\
        .id = KeypadModule::ComputeActionId(sectionName, actionName),\
        .name = displayName,\
        .toolTip = tooltip,\
        .iconName = icon\
    }


KeypadModule::KeypadModule(QPointer<AppSettings> appSettings, QObject* parent)
    : QObject(parent)
{
    qDebug() << "KeypadModule::KeypadModule";

    const auto service = new KeypadService(loadAvailableActions(), appSettings, this);
    mController = new KeypadController(service, this);
}

KeypadModule::~KeypadModule() {
    qDebug() << "KeypadModule::~KeypadModule";
}


KeypadController* KeypadModule::getController() {
    return mController;
}

Keypad::Sections KeypadModule::loadAvailableActions() {
    return {
        {
            .name = tr("System"), .iconName = "sliders_icon.svg", .actions = {
                ACTION(tr("Open website"), tr("Opens a website in the default browser application"), "sliders_icon.svg", "system", "website")
            }
        }
    };
}

// in theory I want this to generate a deterministic UUID based on the fields provided
// but will revisit it at a later time, returning just the concatenation of the fields
QString KeypadModule::ComputeActionId(const QString& section, const QString& action) {
    return QString("id_%1_%2").arg(section, action);
}
