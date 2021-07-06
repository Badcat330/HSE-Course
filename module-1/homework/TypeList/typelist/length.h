#pragma once

#include "typelist.h"

template<typename TList>
struct Length;

template<typename Head, typename Tail>
struct Length<TypeList<Head, Tail>> {
    static const int length = 1 + Length<Tail>::length;
};

template<>
struct Length<NullType> {
    static const int length = 0;
};