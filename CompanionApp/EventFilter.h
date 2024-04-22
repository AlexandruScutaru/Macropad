#pragma once

#include <QObject>


class EventFilter : public QObject {
    Q_OBJECT
public:
    explicit EventFilter(QObject* parent = nullptr);
    ~EventFilter();

    void setCloseRequested();

protected:
    bool eventFilter(QObject* obj, QEvent* event);

signals:
    void windowCloseRequested();

private:
    bool mCloseRequested{ false };

};
