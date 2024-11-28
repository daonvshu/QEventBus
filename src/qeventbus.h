#pragma once

#include <qobject.h>
#include <qthread.h>
#include <qsharedpointer.h>

#include "global.h"

#include "utils/valuecastutil.h"
#include "utils/contextobserver.h"
#include "utils/eventpackagebuilder.h"

#include "eventdispatcher.h"

class QEventBus : public QObject {
public:
    explicit QEventBus(QObject *parent = nullptr);

    template<typename T>
    void registerObserver(T* context) const {
        static_assert(std::is_base_of<QObject, T>::value, "context must be derived from QObject");
        registerTarget(new EventBus::ContextObserver<T>(context));
    }

    template<typename... Args>
    QSharedPointer<EventBus::EventPackageBuilder> publish(const QString& eventName, Args&&... args) const {
        auto builder = QSharedPointer<EventBus::EventPackageBuilder>::create(dispatcher);
        builder->event(eventName)->arg(EventBus::ValueCastUtil::toVariantList(args...));
        return builder;
    }

    static const QEventBus& globalBus();

private:
    QThread* dispatchThread;
    EventBus::EventDispatcher* dispatcher;

private:
    void registerTarget(EventBus::InvokableObserver* observer) const;
};
