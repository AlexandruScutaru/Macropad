#include "DevHelperController.h"

#include <QDebug>


DevHelperController::DevHelperController(QObject* parent)
    : QObject(parent)
    , mModel(new DevHelperModel(this))
{
    qDebug() << "DevHelperController::DevHelperController";
}

DevHelperController::~DevHelperController() {
    qDebug() << "DevHelperController::~DevHelperController";
}


DevHelperModel* DevHelperController::getModel() {
    return mModel;
}

void DevHelperController::onSwitchOutputClicked() {
    emit switchOutputRequested();
}

void DevHelperController::onSliderMoved(int sliderId, int value) {
    qDebug() << "Slider " << sliderId << " moved to " << value;
}

void DevHelperController::onPotentiometersUpdated(const std::vector<int>& values) {
    mModel->reset();

    std::vector<DevHelperModel::Row> model;
    model.reserve(values.size());

    for(int i = 0; i < values.size(); i++) {
        DevHelperModel::Row row;
        row[DevHelperModel::IdRole] = i;
        row[DevHelperModel::ValueRole] = values[i];
        model.push_back(row);
    }

    mModel->setData(model);
}
