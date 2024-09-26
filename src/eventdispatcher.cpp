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
    QMutexLocker lock(&observerMutex);
    observers.append(observer);
}

void EventDispatcher::dispatchEvent(const QString &eventName, const QVariantList &data) {
    int index = 0;
    QMutexLocker lock(&observerMutex);
    while (index < observers.size()) {
        if (!observers.at(index)->canBeReference()) {
            methodCache.remove(observers.at(index));
            observers.removeAt(index);
            continue;
        }
        invokeObserver(observers.at(index++), eventName, data);
    }
}

void EventDispatcher::invokeObserver(InvokableObserver *observer, const QString& eventName, const QVariantList &data) {
    auto context = observer->context();
    if (!methodCache.contains(observer)) {
        QPair<QByteArray, QByteArray> methods;
        auto syncMethod = getMethodName(eventName, false);
        if (methodExist(context, syncMethod)) {
            methods.first = syncMethod;
        }
        auto asyncMethod = getMethodName(eventName, true);
        if (methodExist(context, asyncMethod)) {
            methods.second = asyncMethod;
        }
        methodCache.insert(observer, methods);
    }

    auto methods = methodCache.value(observer);
    if (!methods.first.isEmpty()) {
        callMethod(context, methods.first, data);
    }

    if (!methods.second.isEmpty()) {
        auto invokeWork = QThread::create([=] {
            callMethod(context, methods.second, data);
        });
        connect(invokeWork, &QThread::finished, invokeWork, &QObject::deleteLater);
        invokeWork->start();
    }
}

QByteArray EventDispatcher::getMethodName(const QString &eventName, bool async) {
    if (async) {
        return (QLatin1String("onAsyncEvent") + eventName).toLatin1();
    } else {
        return (QLatin1String("onEvent") + eventName).toLatin1();
    }

}

QGenericArgument EventDispatcher::toArgument(const QVariantList &args, int index) {
    if (index < args.size()) {
        return { args[index].typeName(), args[index].data() };
    }
    return { nullptr };
}

bool EventDispatcher::methodExist(const QObject *object, const QByteArray &methodName) {
    return object->metaObject()->indexOfMethod(methodName) != -1;
}

void EventDispatcher::callMethod(QObject *object, const QByteArray &methodName, const QVariantList &data) {
    QMetaObject::invokeMethod(object, methodName.constData(), Qt::AutoConnection,
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