#pragma once

#include <cstdlib>
#include <optional>
#include <type_traits>

namespace task {

struct NullOpt {
    explicit constexpr NullOpt(int) {
    }
};

constexpr NullOpt kNullOpt = NullOpt(0);

struct InPlace {
    explicit InPlace() = default;
};

constexpr InPlace kInPlace = InPlace();

template <typename T, bool = std::is_trivially_destructible_v<T>>
class OptionalDestructBase;

template <typename T>
class OptionalDestructBase<T, false> {
public:
    using value_type = T;

    ~OptionalDestructBase() {
        if (engaged_) {
            val_.~value_type();
        }
    }

    constexpr OptionalDestructBase() noexcept : null_state_(), engaged_(false) {
    }

    constexpr OptionalDestructBase(NullOpt) noexcept : null_state_(), engaged_(false) {
    }

    template <typename... Args>
    constexpr OptionalDestructBase(InPlace, Args &&...args)
        : val_(std::forward<Args>(args)...), engaged_(true) {
    }

    template <typename U = T>
    constexpr OptionalDestructBase(U &&value) : val_(std::forward<U>(value)), engaged_(true) {
    }

protected:
    void Reset() noexcept {
        if (engaged_) {
            val_.~value_type();
            engaged_ = false;
        }
    }

    template <typename U = T>
    void Set(U &&value) {
        if (engaged_) {
            val_.~value_type();
        }
        val_ = std::forward<U>(value);
        engaged_ = true;
    }

    union {
        char null_state_;
        value_type val_;
    };

    bool engaged_;
};

template <typename T>
class OptionalDestructBase<T, true> {
public:
    using value_type = T;

    constexpr OptionalDestructBase() noexcept : null_state_(), engaged_(false) {
    }

    constexpr OptionalDestructBase(NullOpt) noexcept : null_state_(), engaged_(false) {
    }

    template <typename... Args>
    constexpr OptionalDestructBase(InPlace, Args &&...args)
        : val_(std::forward<Args>(args)...), engaged_(true) {
    }

    template <typename U = T>
    constexpr OptionalDestructBase(U &&value) : val_(std::forward<U>(value)), engaged_(true) {
    }

protected:
    void Reset() {
        if (engaged_) {
            engaged_ = false;
        }
    }

    template <typename U = T>
    void Set(U &&value) {
        val_ = std::forward<U>(value);
        engaged_ = true;
    }

    union {
        char null_state_;
        value_type val_;
    };

    bool engaged_;
};

template <typename T>
class Optional : public OptionalDestructBase<T> {
public:
    using base = OptionalDestructBase<T>;
    using value_type = T;

    constexpr Optional() noexcept = default;

    template <typename U = value_type>
    constexpr explicit Optional(U &&value) : base(std::forward<U>(value)) {
    }

    constexpr explicit Optional(NullOpt) noexcept {
    }

    template <typename... Args>
    constexpr explicit Optional(InPlace, Args &&...args)
        : base(kInPlace, std::forward<Args>(args)...) {
    }

    Optional &operator=(NullOpt) noexcept {
        base::Reset();
        return *this;
    }

    template <typename U = T>
    Optional &operator=(U &&value) {
        base::Set(std::forward<U>(value));
        return *this;
    }

    void Reset() noexcept {
        base::Reset();
    }

    template <typename U>
    constexpr T ValueOr(U &&default_value) const & {
        if (base::engaged_) {
            return base::val_;
        }
        return default_value;
    }

    template <typename U>
    constexpr T ValueOr(U &&default_value) && {
        if (base::engaged_) {
            return base::val_;
        }
        return default_value;
    }

    constexpr bool HasValue() const noexcept {
        return base::engaged_;
    }

    constexpr explicit operator bool() const noexcept {
        return HasValue();
    }

    constexpr std::add_pointer_t<const value_type> operator->() const {
        return base::val_;
    }

    constexpr std::add_pointer_t<value_type> operator->() {
        return &(base::val_);
    }

    constexpr const value_type &operator*() const & {
        return base::val_;
    }

    constexpr value_type &operator*() & {
        return base::val_;
    }

    constexpr const value_type &&operator*() const && {
        return std::move(base::val_);
    }

    constexpr value_type &&operator*() && {
        return std::move(base::val_);
    }
};
}  // namespace task