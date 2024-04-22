#include "EventFilter.h"

#include <QEvent>
#include <QDebug>


EventFilter::EventFilter(QObject* parent)
    : QObject(parent)
{
    qDebug() << "EventFilter::EventFilter";
}

EventFilter::~EventFilter() {
    qDebug() << "EventFilter::~EventFilter";
}


bool EventFilter::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::Close && !mCloseRequested) {
        emit windowCloseRequested();
        return true;
    }

    return QObject::eventFilter(obj, event);
}

void EventFilter::setCloseRequested() {
    mCloseRequested = true;
}
