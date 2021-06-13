#pragma once

#include <type_traits>
#include <utility>

#include "utility.h"

template <typename T, typename... Args>
struct LibCppIsConstructible;

template <typename Derived, typename Base>
struct IsInvalidBaseToDerivedCast {
  static_assert(std::is_reference_v<Derived>, "Wrong specialization");
  using rawDerived = Uncvref<Derived>;
  using rawBase = Uncvref<Base>;

  static const bool kValue = !std::is_same_v<rawBase, rawDerived> && std::is_base_of_v<rawBase, rawDerived>
      && !LibCppIsConstructible<rawDerived, rawBase>::type::value;
};

template <typename To, typename From>
struct IsInvalidLvalueToRvalueCast : std::false_type {
  static_assert(std::is_reference<To>::value, "Wrong specialization");
};

template <typename RefTo, typename RefFrom>
struct IsInvalidLvalueToRvalueCast<RefTo&&, RefFrom&> {
  using raw_to = Uncvref<RefTo>;
  using raw_from = Uncvref<RefFrom>;
  static const bool
      kValue = !std::is_function_v<raw_to> && (std::is_same_v<raw_from, raw_to> || std::is_base_of_v<raw_to, raw_from>);
};

template <typename T>
T&& Declval(int);

template <typename T>
T Declval(int64_t);

template <typename T>
decltype(Declval<T>(0)) Declval() noexcept;

struct IsConstructibleHelper {

  template <typename T, typename... Args, typename = decltype(T(Declval<Args>()...))>
  static std::true_type TestNary(int);

  template <typename T, typename...>
  static std::false_type TestNary(...);

  template <typename T, typename A, typename = decltype(::new T(Declval<A>()))>
  static std::is_destructible<T> TestUnary(int);

  template <typename, typename>
  static std::false_type TestUnary(...);

  template <typename T>
  static void eat(T);

  template <typename To, typename From, typename = decltype(eat<To>(Declval<From>()))>
  static std::true_type TestCast(int);

  template <typename To, typename From, typename = decltype(static_cast<To>(Declval<From>()))>
  static std::integral_constant<bool,
                                !IsInvalidBaseToDerivedCast<To, From>::kValue
                                    && !IsInvalidLvalueToRvalueCast<To, From>::kValue
  > TestCast(long);

  template <typename, typename>
  static std::false_type TestCast(...);
};

template <typename T, typename... Args>
struct LibCppIsConstructible {
  static_assert(sizeof...(Args) > 1, "Wrong specialization");
  using type = decltype(IsConstructibleHelper::TestNary<T, Args...>(0));
};

template <typename T, typename A>
struct LibCppIsConstructible<T, A> : public decltype(IsConstructibleHelper::TestUnary<T, A>(0)) {};

template <typename T, typename A>
struct LibCppIsConstructible<T&, A> : public decltype(IsConstructibleHelper::TestCast<T&, A>(0)) {};

template <typename T, typename A>
struct LibCppIsConstructible<T&&, A> : public decltype(IsConstructibleHelper::TestCast<T&&, A>(0)) {};

template <typename T, typename... Args>
struct IsConstructible : public LibCppIsConstructible<T, Args...>::type {};

template <typename T>
struct IsCopyConstructible : public IsConstructible<T, typename AddLvalueReference<typename AddConst<T>::type>::type> {};