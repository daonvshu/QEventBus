#include "eventdispatcher.h"

#include <qvariant.h>
#include <qthread.h>

EVENT_BUS_BEGIN_NAMESPACE

EventDispatcher::EventDispatcher(QObject *parent)
    : QObject(parent)
{
    connect(this, &EventDispatcher::sendEvent, this, &EventDispatcher::dispatchEvent);
}

void EventDispatcher::addObserver(InvokableObserver *observer) {
    observers.append(observer);
}

void EventDispatcher::dispatchEvent(const QString &eventName, const QVariantList &data) {
    int index = 0;
    while (index < observers.size()) {
        if (!observers.at(index)->canBeReference()) {
            observers.removeAt(index);
            continue;
        }
        invokeObserver(observers.at(index++), eventName, data);
    }
}

void EventDispatcher::invokeObserver(InvokableObserver *observer, const QString& eventName, const QVariantList &data) {
    auto syncMethod = getMethodName(eventName, false);
    auto context = observer->context();
    if (methodExist(context, syncMethod)) {
        callMethod(context, syncMethod, data);
    }

    auto asyncMethod = getMethodName(eventName, true);
    if (methodExist(context, asyncMethod)) {
        auto invokeWork = QThread::create([=] {
            callMethod(context, asyncMethod, data);
        });
        connect(invokeWork, &QThread::finished, invokeWork, &QObject::deleteLater);
        invokeWork->start();
    }
}

QString EventDispatcher::getMethodName(const QString &eventName, bool async) {
    if (async) {
        return QLatin1String("onAsyncEvent") + eventName;
    } else {
        return QLatin1String("onEvent") + eventName;
    }

}

QGenericArgument EventDispatcher::toArgument(const QVariantList &args, int index) {
    if (index < args.size()) {
        return { args[index].typeName(), args[index].data() };
    }
    return { nullptr };
}

bool EventDispatcher::methodExist(const QObject *object, const QString &methodName) {
    return object->metaObject()->indexOfMethod(methodName.toLatin1()) != -1;
}

void EventDispatcher::callMethod(QObject *object, const QString &methodName, const QVariantList &data) {
    QMetaObject::invokeMethod(object, methodName.toLatin1().constData(), Qt::AutoConnection,
                              toArgument(data, 0),
                              toArgument(data, 1),
                              toArgument(data, 2),
                              toArgument(data, 3),
                              toArgument(data, 4),
                              toArgument(data, 5),
                              toArgument(data, 6),
                              toArgument(data, 7),
                              toArgument(data, 8),
                              toArgument(data, 9));
}

EVENT_BUS_END_NAMESPACE