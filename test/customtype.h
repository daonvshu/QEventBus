#pragma once

#include <qobject.h>

struct CustomType {
    QString data1;
    int data2;
};

Q_DECLARE_METATYPE(CustomType)