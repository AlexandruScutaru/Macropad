#pragma once

#include "../model/ActionConfigListModel.h"

#include <QObject>
#include <QPointer>
#include <QQmlEngine>

namespace Keypad {
    struct Action;
    enum OptionType;
}


class ActionConfigController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Not intended to be created from QML directly")
    Q_PROPERTY(QString keyActionName READ keyActionName NOTIFY keyActionNameChanged)
    Q_PROPERTY(QString keyActionDisplayName READ keyActionDisplayName NOTIFY keyActionDisplayNameChanged)
    Q_PROPERTY(ActionConfigListModel* model READ model NOTIFY modelChanged)

public:
    explicit ActionConfigController(QObject* parent = nullptr);
    ~ActionConfigController();

    Q_INVOKABLE void optionChanged(const QString& name, const QVariant& value);

    QString keyActionName();
    QString keyActionDisplayName();
    ActionConfigListModel* model();

signals:
    void keyActionNameChanged(const QString& name);
    void keyActionDisplayNameChanged(const QString& displayName);
    void modelChanged(ActionConfigListModel* model);
    void configOptionChanged(int layer, int key, const QString& name, const QVariant& value);

public slots:
    void onActionConfigChanged(int layer, int key, const Keypad::Action& action);

private:
    QString mActionName;
    QString mActionDisplayName;
    QPointer<ActionConfigListModel> mActionConfigListModel{ nullptr };
    int mKeyActionLayer{ 0 };
    int mKeyAction{ 0 };

};
