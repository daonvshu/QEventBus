#pragma once

#include <qobject.h>
#include <qthread.h>

#include "global.h"

#include "utils/valuecastutil.h"
#include "utils/contextobserver.h"

#include "eventdispatcher.h"

class QEventBus : public QObject {
public:
    explicit QEventBus(QObject *parent = nullptr);

    template<typename T>
    void registerObserver(T* context) {
        static_assert(std::is_base_of<QObject, T>::value, "context must be derived from QObject");
        registerTarget(new EventBus::ContextObserver<T>(context));
    }

    template<typename... Args>
    void publish(const QString& eventName, Args&&... args) {
        publishEvent(eventName, EventBus::ValueCastUtil::toVariantList(args...));
    }

private:
    QThread* dispatchThread;
    EventBus::EventDispatcher* dispatcher;

private:
    void publishEvent(const QString& eventName, const QVariantList& data);
    void registerTarget(EventBus::InvokableObserver* observer);
};
