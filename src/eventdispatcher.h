#pragma once

#include <qobject.h>

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

    static QString getMethodName(const QString& eventName, bool async);
    static QGenericArgument toArgument(const QVariantList& args, int index);
    static bool methodExist(const QObject* object, const QString& methodName);
    static void callMethod(QObject* object, const QString& methodName, const QVariantList& data);

private:
    QList<InvokableObserver*> observers;
};

EVENT_BUS_END_NAMESPACE