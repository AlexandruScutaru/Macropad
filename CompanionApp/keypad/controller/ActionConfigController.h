#pragma once

#include "../model/ActionConfigListModel.h"

#include <QObject>
#include <QPointer>
#include <QQmlEngine>

namespace Keypad {
    struct Action;
}


class ActionConfigController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Not intended to be created from QML directly")
    Q_PROPERTY(QString keyActionId READ keyActionId NOTIFY keyActionIdChanged)
    Q_PROPERTY(QString keyActionDisplayName READ keyActionDisplayName NOTIFY keyActionDisplayNameChanged)
    Q_PROPERTY(ActionConfigListModel* model READ model NOTIFY modelChanged)

public:
    explicit ActionConfigController(QObject* parent = nullptr);
    ~ActionConfigController();

    Q_INVOKABLE void optionChanged(const QString& name, const QVariant& value);

    QString keyActionId();
    QString keyActionDisplayName();
    ActionConfigListModel* model();

signals:
    void keyActionIdChanged(const QString& id);
    void keyActionDisplayNameChanged(const QString& displayName);
    void modelChanged(ActionConfigListModel* model);
    void configOptionChanged(int layer, int key, const QString& name, const QVariant& value);

public slots:
    void onActionConfigChanged(int layer, int key, const Keypad::Action& action);

private:
    QString mActionId;
    QString mActionDisplayName;
    QPointer<ActionConfigListModel> mActionConfigListModel{ nullptr };
    int mKeyActionLayer{ 0 };
    int mKeyAction{ 0 };

};
