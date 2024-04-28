#include "DevHelperController.h"

#include <QDebug>


DevHelperController::DevHelperController(QObject *parent)
    : QObject(parent)
{
    qDebug() << "DevHelperController::DevHelperController";
}

DevHelperController::~DevHelperController() {
    qDebug() << "DevHelperController::~DevHelperController";
}


void DevHelperController::onSwitchOutputClicked() {
    qDebug() << "switch output button clicked";
}
