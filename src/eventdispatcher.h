#pragma once

#include <qobject.h>
#include <qhash.h>
#include <qmutex.h>

#include "global.h"

#include "utils/invokableobserver.h"

EVENT_BUS_BEGIN_NAMESPACE

class EventDispatcher : public QObject {
    Q_OBJECT

public:
    explicit EventDispatcher(QObject *parent = nullptr);

    void addObserver(InvokableObserver* observer);

signals:
    void sendEvent(const QString& eventName, const QVariantList& data);

private:
    void dispatchEvent(const QString& eventName, const QVariantList& data);
    void invokeObserver(InvokableObserver* observer, const QString& eventName, const QVariantList& data);

    static QByteArray getMethodName(const QString& eventName, bool async);
    static QGenericArgument toArgument(const QVariantList& args, int index);
    static bool methodExist(const QObject* object, const QByteArray& methodName);
    static void callMethod(QObject* object, const QByteArray& methodName, const QVariantList& data);

private:
    QMutex observerMutex;
    QList<InvokableObserver*> observers;
    QHash<InvokableObserver*, QPair<QByteArray, QByteArray>> methodCache;
};

EVENT_BUS_END_NAMESPACE