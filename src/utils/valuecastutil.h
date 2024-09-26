#pragma once

#include <qvariant.h>

#include "../global.h"

EVENT_BUS_BEGIN_NAMESPACE

struct ValueCastUtil {
    template<typename T, typename... Args>
    static QVariantList toVariantList(T&& value, Args&&... args) {
        return QVariantList{ castToVariant(value) } + toVariantList(args...);
    }

    static QVariantList toVariantList() {
        return {};
    }

    template<typename E>
    static QVariant castToVariant(const typename std::enable_if<std::is_constructible<QVariant, E>::value, E>::type& v) {
        return v;
    }

    template<typename E>
    static QVariant castToVariant(const typename std::enable_if<!std::is_constructible<QVariant, E>::value, E>::type& v) {
        return QVariant::fromValue(v);
    }
};

EVENT_BUS_END_NAMESPACE