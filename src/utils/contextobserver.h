#pragma once

#include <qobject.h>
#include <qpointer.h>

#include "../global.h"

#include "invokableobserver.h"

EVENT_BUS_BEGIN_NAMESPACE

template<typename T>
class ContextObserver : public InvokableObserver {
public:
    explicit ContextObserver(T *context) : contextPtr(context) {}

    bool canBeReference() override {
        return !contextPtr.isNull();
    }

    QObject* context() const override {
        return contextPtr.data();
    }

    QByteArray targetClassName() const override {
        return T::staticMetaObject.className();
    }

    QString targetObjectName() const override {
        return contextPtr->objectName();
    }

private:
    QPointer<T> contextPtr;
};

EVENT_BUS_END_NAMESPACE