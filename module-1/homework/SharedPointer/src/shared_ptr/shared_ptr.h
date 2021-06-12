#pragma once

#include "../control/control.h"

// Default Deleter
template <typename T>
struct DefaultDeleter {
    void operator()(T pointer) noexcept {
        delete pointer;
    }
};

// Remove Types struct
template <typename T>
struct RemoveExtent {
    using type = T;
};

template <typename T>
struct RemoveExtent<T[]> {
    using type = T;
};

template <typename T, size_t N>
struct RemoveExtent<T[N]> {
    using type = T;
};

// SharedPtr
template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
public:
    using element_type = typename RemoveExtent<T>::type;
    using pointer_type = element_type*;

    constexpr SharedPtr() noexcept = default;

    ~SharedPtr() {
        if (control_ != nullptr) {
            control_->ReleaseShared();
        }

        pointer_ = nullptr;
        control_ = nullptr;
    }

    template <typename Y>
    explicit SharedPtr(Y* p) : pointer_{p} {
        control_ = new ControlBlock<pointer_type, DefaultDeleter<pointer_type>>{p};
    }

    template <typename Y, typename Deleter>
    SharedPtr(Y* p, Deleter deleter) noexcept : pointer_{p} {
        control_ = new ControlBlock<pointer_type, Deleter>{p, deleter};
    }

    SharedPtr(const SharedPtr& other) noexcept
        : pointer_{other.pointer_}, control_{other.control_} {
        control_->AddShared();
    }

    SharedPtr(SharedPtr&& other) noexcept : pointer_{other.pointer_}, control_{other.control_} {
        other.control_ = nullptr;
        other.pointer_ = nullptr;
    }

    SharedPtr& operator=(const SharedPtr& r) noexcept {
        Reset();
        pointer_ = r.pointer_;
        control_ = r.control_;
        control_->AddShared();
        return *this;
    }

    template <typename Y>
    SharedPtr& operator=(const SharedPtr<Y>& r) noexcept {
        if (this == &r) {
            return *this;
        }

        Reset();
        pointer_ = r.pointer_;
        control_ = r.control_;
        control_->AddShared();
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& r) noexcept {
        Reset();
        pointer_ = r.pointer_;
        control_ = r.control_;
        r.control_ = nullptr;
        r.pointer_ = nullptr;
        return *this;
    }

    template <typename Y>
    SharedPtr& operator=(SharedPtr<Y>&& r) noexcept {
        Reset();
        pointer_ = r.pointer_;
        control_ = r.control_;
        r.control_ = nullptr;
        r.pointer_ = nullptr;
        return *this;
    }

    // Modifiers
    void Reset() noexcept {
        pointer_ = nullptr;
        if (control_ != nullptr) {
            control_->ReleaseShared();
        }
        control_ = nullptr;
    }

    template <typename Y>
    void Reset(Y* p) noexcept {
        if (control_ != nullptr) {
            control_->ReleaseShared();
        }

        pointer_ = p;
        control_ = new ControlBlock<Y*, DefaultDeleter<pointer_type>>{p};
    }

    template <typename Y, typename Deleter>
    void Reset(Y* p, Deleter deleter) noexcept {
        if (control_ != nullptr) {
            control_->ReleaseShared();
        }

        pointer_ = p;
        control_ = new ControlBlock<Y*, Deleter>{p, deleter};
    }

    void Swap(SharedPtr& other) noexcept {
        std::swap(pointer_, other.pointer_);
        std::swap(control_, other.control_);
    }

    // Observers
    T* Get() const noexcept {
        return pointer_;
    }

    int64_t UseCount() const noexcept {
        if (control_ != nullptr) {
            return control_->UseCount();
        }

        return 0;
    }

    T& operator*() const noexcept {
        return *pointer_;
    }

    T* operator->() const noexcept {
        return pointer_;
    }

    element_type& operator[](std::ptrdiff_t idx) const {
        return pointer_[idx];
    }

    explicit operator bool() const noexcept {
        return control_ != nullptr && pointer_ != nullptr;
    }

    template <typename U>
    friend class WeakPtr;

private:
    pointer_type pointer_ = nullptr;
    SharedWeakCount* control_ = nullptr;
};

template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    return SharedPtr<T>(new T{std::forward<Args>(args)...});
}

// WeakPtr
template <typename T>
class WeakPtr {

public:
    using element_type = typename RemoveExtent<T>::type;
    using poiter_type = element_type*;

    // Special-member functions
    constexpr WeakPtr() noexcept = default;

    template <typename Y>
    explicit WeakPtr(const SharedPtr<Y>& other)
        : pointer_{other.pointer_}, control_{other.control_} {
        control_->AddWeak();
    }

    WeakPtr(const WeakPtr& other) noexcept : pointer_{other.pointer_}, control_{other.control_} {
        control_->AddWeak();
    }

    WeakPtr(WeakPtr&& other) noexcept : pointer_{other.pointer_}, control_{other.control_} {
        other.pointer_ = nullptr;
        other.control_ = nullptr;
    }

    template <typename Y>
    WeakPtr& operator=(const SharedPtr<Y>& other) {
        Reset();

        pointer_ = other.pointer_;
        control_ = other.control_;

        if (control_ != nullptr) {
            control_->AddWeak();
        }

        return *this;
    }

    WeakPtr& operator=(const WeakPtr& other) noexcept {
        if (this == other) {
            return *this;
        }

        Reset();
        pointer_ = other.pointer_;
        control_ = other.control_;

        if (control_ != nullptr) {
            control_->AddWeak();
        }

        return *this;
    }

    WeakPtr& operator=(WeakPtr&& other) noexcept {
        Reset();
        pointer_ = other.pointer_;
        control_ = other.control_;
        other.pointer_ = nullptr;
        other.control_ = nullptr;

        return *this;
    }

    ~WeakPtr() {
        if (control_ != nullptr) {
            control_->ReleaseWeak();
        }
    }

    // Modifiers
    void Reset() noexcept {
        pointer_ = nullptr;
        if (control_ != nullptr) {
            control_->ReleaseWeak();
        }
        control_ = nullptr;
    }

    void Swap(WeakPtr<T>& other) noexcept {
        std::swap(pointer_, other.pointer_);
        std::swap(control_, other.control_);
    }

    // Observers
    bool Expired() noexcept {
        return control_ == nullptr || control_->UseCount() == 0;
    }

    SharedPtr<T> Lock() const noexcept {
        SharedPtr<element_type> shared_ptr;
        shared_ptr.pointer_ = pointer_;

        if (control_ != nullptr) {
            shared_ptr.control_ = control_->Lock();
        }

        return shared_ptr;
    }

    template <typename U>
    friend class SharedPtr;

public:
    poiter_type pointer_{nullptr};
    SharedWeakCount* control_{nullptr};
};