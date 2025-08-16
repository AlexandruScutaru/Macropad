#include "KeypadModule.h"
#include "actions/SystemActions.h"
#include "controller/KeypadController.h"
#include "service/KeypadService.h"
#include "AppSettings.h"

#include <QDebug>

#include <algorithm>

using namespace Keypad;


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

AvailableActions KeypadModule::loadAvailableActions() {
    ActionsMap actionsMap;
    Sections sections;

    Actions::InsertSystemActions(actionsMap, sections);

    return { actionsMap, sections };
}

// in theory I want this to generate a deterministic UUID based on the fields provided
// but will revisit it at a later time, returning just the concatenation of the fields
QString KeypadModule::Id(const QString& section, const QString& action) {
    return QString("id_%1_%2").arg(section, action);
}
