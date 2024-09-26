#pragma once

#include <qobject.h>

#include <qeventbus.h>

class GlobalEventBus : public QObject {
public:
    static const QEventBus& getBus() {
        return instance().eventBus;
    }

private:
    GlobalEventBus();

    static GlobalEventBus& instance();

    QEventBus eventBus;
};
