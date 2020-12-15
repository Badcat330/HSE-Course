#pragma once

#include "typelist.h"

template<typename TList, typename TargetType>
struct Erase;

template<typename Head, typename Tail, typename TargetType>
struct Erase<TypeList<Head, Tail>, TargetType> {
    using NewTypeList = TypeList<Head, typename Erase<Tail, TargetType>::NewTypeList>;
};

template<typename Head, typename Tail, typename TargetType>
struct Erase<TypeList<TargetType, TypeList<Head, Tail>>, TargetType> {
    using NewTypeList = TypeList<Head, Tail>;
};

template<typename TargetType>
struct Erase<TypeList<TargetType, NullType>, TargetType> {
    using NewTypeList = NullType;
};

template<typename TargetType>
struct Erase<NullType, TargetType> {
    using NewTypeList = NullType;
};