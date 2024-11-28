#include "qeventbus.h"

#include "eventdispatcher.h"

#include <qcoreapplication.h>

EVENT_BUS_USE_NAMESPACE

QEventBus::QEventBus(QObject *parent)
    : QObject(parent)
{
    dispatchThread = new QThread;
    dispatcher = new EventDispatcher;
    dispatcher->moveToThread(dispatchThread);

    connect(qApp, &QCoreApplication::aboutToQuit, this, [&] {
        dispatchThread->quit();
    });

    connect(dispatchThread, &QThread::finished, [=] {
        dispatcher->deleteLater();
        dispatchThread->deleteLater();
    });

    dispatchThread->start();
}

void QEventBus::registerTarget(EventBus::InvokableObserver *observer) const {
    dispatcher->addObserver(observer);
}

const QEventBus &QEventBus::globalBus() {
    static QEventBus bus;
    return bus;
}

