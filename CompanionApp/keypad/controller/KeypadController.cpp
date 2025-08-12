#include "KeypadController.h"

#include "../service/KeypadService.h"

#include <QDebug>


KeypadController::KeypadController(QPointer<KeypadService> service, QObject* parent)
    : QObject(parent)
    , mService(service)
{
    qDebug() << "KeypadController::KeypadController";
}

KeypadController::~KeypadController() {
    qDebug() << "KeypadController::~KeypadController";
}


ActionSectionsListModel* KeypadController::getActionSectionsListModel() {
    return mService->getActionSectionsListModel();
}

LayerListModel* KeypadController::getLayerListModel() {
    return mService->getLayerListModel();
}

void KeypadController::assignAction(int layer, int key, const QString& actionId) {
    mService->assignActionRequested(layer, key, actionId);
}
