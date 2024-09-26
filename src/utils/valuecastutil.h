#pragma once

#include <qvariant.h>

#include "../global.h"

EVENT_BUS_BEGIN_NAMESPACE

namespace ValueCastUtil {

    template<typename E>
    static QVariant castToVariant(const typename std::enable_if<std::is_constructible<QVariant, E>::value, E>::type& v) {
        return v;
    }

    template<typename E>
    static QVariant castToVariant(const typename std::enable_if<!std::is_constructible<QVariant, E>::value, E>::type& v) {
        return QVariant::fromValue(v);
    }

    static QVariantList toVariantList() {
        return {};
    }

    template<typename T, typename... Args>
    static QVariantList toVariantList(const T& value, const Args&... args) {
        return QVariantList{ castToVariant<T>(value) } + toVariantList(args...);
    }
}

EVENT_BUS_END_NAMESPACE