#pragma once

#include <qobject.h>
#include <qhash.h>
#include <qmutex.h>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <qvariantlist.h>
#endif

#include <utility>

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
    static bool methodExist(const QObject* object, QByteArray methodName, const QVariantList& data);
    static void callMethod(QObject* object, const QByteArray& methodName, const QVariantList& data, Qt::ConnectionType connectionType);

private:
    QMutex observerMutex;
    QList<InvokableObserver*> observers;

    struct MethodCacheData {
        QString eventName;
        QByteArray syncMethod;
        QByteArray asyncMethod;

        explicit MethodCacheData(QString eventName): eventName(std::move(eventName)) {}

        bool operator==(const MethodCacheData& other) const {
            return eventName == other.eventName;
        }
    };
    QMultiHash<InvokableObserver*, MethodCacheData> methodCache;
};

EVENT_BUS_END_NAMESPACE