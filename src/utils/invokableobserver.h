#pragma once

#include "../global.h"

EVENT_BUS_BEGIN_NAMESPACE

class InvokableObserver {
public:
    virtual ~InvokableObserver() = default;
    virtual bool canBeReference() = 0;
    virtual QObject* context() const = 0;
    virtual QByteArray targetClassName() const = 0;
    virtual QString targetObjectName() const = 0;
};

EVENT_BUS_END_NAMESPACE