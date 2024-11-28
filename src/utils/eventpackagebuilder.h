#pragma once

#include "../global.h"
#include "../eventdispatcher.h"

EVENT_BUS_BEGIN_NAMESPACE

class EventPackageBuilder {
public:
    explicit EventPackageBuilder(EventDispatcher* dispatcher)
        : mDispatcher(dispatcher)
    {}

    ~EventPackageBuilder() {
        mDispatcher->sendEvent(mEventName, mArgs, mPrintTarget);
    }

    EventPackageBuilder* event(const QString& eventName) {
        mEventName = eventName;
        return this;
    }

    EventPackageBuilder* arg(const QVariantList& args) {
        mArgs << args;
        return this;
    }

    EventPackageBuilder* arg(const QVariant& arg) {
        mArgs << arg;
        return this;
    }

    EventPackageBuilder* printTarget() {
        mPrintTarget = true;
        return this;
    }

private:
    EventDispatcher* mDispatcher;
    QString mEventName;
    QVariantList mArgs;
    bool mPrintTarget = false;
};

EVENT_BUS_END_NAMESPACE