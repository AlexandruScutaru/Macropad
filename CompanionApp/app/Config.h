#pragma once

#include <QObject>
#include <QVariant>


class Config: public QObject {
    Q_OBJECT
public:
    explicit Config(QObject* parent = nullptr);
    ~Config();

    QVariant getProperty(QAnyStringView key, const QVariant& defaultValue = QVariant{}) const;
    void setProperty(QAnyStringView key, const QVariant& value);

};
