#include "eventdispatcher.h"

#include <qvariant.h>
#include <qthread.h>
#include <qdebug.h>
#include <qloggingcategory.h>

EVENT_BUS_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(dispatcher, "qeventbus.dispatcher")

EventDispatcher::EventDispatcher(QObject *parent)
    : QObject(parent)
{
    connect(this, &EventDispatcher::sendEvent, this, &EventDispatcher::dispatchEvent);
}

void EventDispatcher::addObserver(InvokableObserver *observer) {
    QMutexLocker lock(&observerMutex);
    observers.append(observer);
}

void EventDispatcher::dispatchEvent(const QString &eventName, const QVariantList &data, bool printTarget) {
    int index = 0;
    QMutexLocker lock(&observerMutex);
    while (index < observers.size()) {
        if (!observers.at(index)->canBeReference()) {
            methodCache.remove(observers.at(index));
            observers.removeAt(index);
            continue;
        }
        invokeObserver(observers.at(index++), eventName, data, printTarget);
    }
}

void EventDispatcher::invokeObserver(InvokableObserver *observer, const QString& eventName, const QVariantList &data, bool printTarget) {
    auto context = observer->context();
    {
        if (!methodCache.contains(observer) || methodCache.values(observer).indexOf(MethodCacheData(eventName)) == -1) {
            MethodCacheData cacheData(eventName);
            cacheData.objectName = observer->targetObjectName();
            cacheData.className = observer->targetClassName();

            cacheData.autoConnectMethod = getMethodName(eventName, MethodType::AutoConnectMethod);
            auto methodSignature = cacheData.autoConnectMethod;
            if (!methodExist(context, methodSignature, data)) {
                qCInfo(dispatcher) << "ignore the observer" << observer->targetClassName() << "sync event:" << eventName << ", has no sync method:" << methodSignature;
                cacheData.autoConnectMethod.clear();
            }

            cacheData.directCallMethod = getMethodName(eventName, MethodType::DirectCallMethod);
            methodSignature = cacheData.directCallMethod;
            if (!methodExist(context, methodSignature, data)) {
                qCInfo(dispatcher) << "ignore the observer" << observer->targetClassName() << "direct event:" << eventName << ", has no direct method:" << methodSignature;
                cacheData.directCallMethod.clear();
            }

            cacheData.asyncMethod = getMethodName(eventName, MethodType::AsyncCallMethod);
            methodSignature = cacheData.asyncMethod;
            if (!methodExist(context, methodSignature, data)) {
                qCInfo(dispatcher) << "ignore the observer" << observer->targetClassName() << "async event:" << eventName << ", has no async method:" << methodSignature;
                cacheData.asyncMethod.clear();
            }

            methodCache.insert(observer, cacheData);
        }
    }

    {
        auto methods = methodCache.values(observer);
        auto methodCacheIndex = methods.indexOf(MethodCacheData(eventName));
        const auto& cacheData = methods.at(methodCacheIndex);

        if (!cacheData.autoConnectMethod.isEmpty()) {
            if (printTarget) {
                qCInfo(dispatcher).nospace() << "call target: " << cacheData.objectName << "[" << cacheData.className << "]" << " auto connection method '" << cacheData.autoConnectMethod << "'";
            }
            callMethod(context, cacheData.autoConnectMethod, data, Qt::AutoConnection);
        }

        if (!cacheData.directCallMethod.isEmpty()) {
            if (printTarget) {
                qCInfo(dispatcher).nospace() << "call target: " << cacheData.objectName << "[" << cacheData.className << "]" << " direct connection method '" << cacheData.directCallMethod << "'";
            }
            callMethod(context, cacheData.directCallMethod, data, Qt::DirectConnection);
        }

        if (!cacheData.asyncMethod.isEmpty()) {
            if (printTarget) {
                qCInfo(dispatcher).nospace() << "call target: " << cacheData.objectName << "[" << cacheData.className << "]" << " async method '" << cacheData.asyncMethod << "'";
            }
            auto invokeWork = QThread::create([=] {
                callMethod(context, cacheData.asyncMethod, data, Qt::DirectConnection);
            });
            connect(invokeWork, &QThread::finished, invokeWork, &QObject::deleteLater);
            invokeWork->start();
        }
    }
}

QByteArray EventDispatcher::getMethodName(const QString &eventName, MethodType methodType) {
    QByteArray methodName;
    switch (methodType) {
        case MethodType::AutoConnectMethod:
            methodName = "onEvent";
            break;
        case MethodType::DirectCallMethod:
            methodName = "onDirectEvent";
            break;
        case MethodType::AsyncCallMethod:
            methodName = "onAsyncEvent";
            break;
    }
    methodName += toUpperCamelCase(eventName).toLatin1();
    return methodName;
}

QGenericArgument EventDispatcher::toArgument(const QVariantList &args, int index) {
    if (index < args.size()) {
        return { args[index].typeName(), args[index].data() };
    }
    return { nullptr };
}

bool EventDispatcher::methodExist(const QObject *object, QByteArray& methodName, const QVariantList& data) {
    methodName += "(";
    for (const auto &arg : data) {
        methodName += arg.typeName();
        methodName += ",";
    }
    if (!data.isEmpty()) {
        methodName.chop(1);
    }
    methodName += ")";
    return object->metaObject()->indexOfMethod(methodName) != -1;
}

void EventDispatcher::callMethod(QObject *object, const QByteArray &methodName, const QVariantList &data, Qt::ConnectionType connectionType) {
    QMetaObject::invokeMethod(object, methodName.constData(), connectionType,
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

QString EventDispatcher::toUpperCamelCase(const QString &input) {
    QString result;
    bool capitalizeNext = true;

    for (QChar ch : input) {
        if (ch.isLetter()) {
            if (capitalizeNext) {
                result += ch.toUpper();
                capitalizeNext = false;
            } else {
                result += ch;
            }
        } else if (ch.isDigit()) {
            result += ch;
            capitalizeNext = true;
        } else {
            capitalizeNext = true;
        }
    }

    return result;
}

EVENT_BUS_END_NAMESPACE