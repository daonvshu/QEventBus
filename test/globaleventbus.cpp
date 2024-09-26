#include "globaleventbus.h"

GlobalEventBus::GlobalEventBus()
{
}

GlobalEventBus &GlobalEventBus::instance() {
    static GlobalEventBus instance;
    return instance;
}
