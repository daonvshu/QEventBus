#include "qeventbus.h"

#include "eventdispatcher.h"

EVENT_BUS_USE_NAMESPACE

QEventBus::QEventBus(QObject *parent)
    : QObject(parent)
{
    dispatchThread = new QThread;
    dispatcher = new EventDispatcher;
    dispatcher->moveToThread(dispatchThread);

    connect(dispatchThread, &QThread::finished, [=] {
        dispatcher->deleteLater();
        dispatchThread->deleteLater();
    });

    dispatchThread->start();
}

void QEventBus::publishEvent(const QString &eventName, const QVariantList &data) {
    dispatcher->sendEvent(eventName, data);
}

void QEventBus::registerTarget(EventBus::InvokableObserver *observer) {
    dispatcher->addObserver(observer);
}

