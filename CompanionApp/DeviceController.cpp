#include "DeviceController.h"
#include "HidHelper.h"

#include <QDebug>


DeviceController::DeviceController(QObject* parent)
    : QObject(parent)
    , mModel(new DeviceInfoModel(this))
{
    qDebug() << "DeviceController::DeviceController";

    mHidHelper = new HidHelper(this);
    mHidInitted = mHidHelper->init();
}

DeviceController::~DeviceController() {
    qDebug() << "DeviceController::~DeviceController";
}

DeviceInfoModel* DeviceController::getModel() {
    return mModel;
}

void DeviceController::search(const QString& vid, const QString& pid) {
    const auto info = mHidHelper->enumerateDevices(convertToInt(vid), convertToInt(pid));

    mModel->reset();

    std::vector<DeviceInfoModel::Row> model;
    model.reserve(info.size());

    for(int i = 0; i < info.size(); i++) {
        DeviceInfoModel::Row row;
        row[DeviceInfoModel::Vid] = QString("0x%1").arg(info[i].vid, 4, 16, QLatin1Char('0'));
        row[DeviceInfoModel::Pid] = QString("0x%1").arg(info[i].pid, 4, 16, QLatin1Char('0'));
        row[DeviceInfoModel::UsagePage] = QString("0x%1").arg(info[i].usagePage, 4, 16, QLatin1Char('0'));
        row[DeviceInfoModel::UsageId] = QString("0x%1").arg(info[i].usageId, 4, 16, QLatin1Char('0'));
        row[DeviceInfoModel::Product] = QString::fromStdString(info[i].product);
        row[DeviceInfoModel::Manufacturer] = QString::fromStdString(info[i].manufacturer);
        row[DeviceInfoModel::Serial] = QString::fromStdString(info[i].serial);
        row[DeviceInfoModel::Path] = QString::fromStdString(info[i].path);
        model.push_back(row);
    }

    mModel->setData(model);
}

//device->usage_page == 0xFF60 && device->usage == 0x61)
void DeviceController::openDevice(const QString& path) {
    if (auto device = mHidHelper->openDevice(path.toStdString()); device) {
        emit deviceOpened(device);
        emit deviceConnected();
    }
}

int DeviceController::convertToInt(const QString& hexStr) {
    if (hexStr.isEmpty()) {
        return 0;
    }

    bool res = false;
    int value = hexStr.toInt(&res, 16);
    assert(res && "couldn't convert hex str to int");

    return value;
}
