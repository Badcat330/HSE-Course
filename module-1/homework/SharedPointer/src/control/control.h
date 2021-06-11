#pragma once

#include <atomic>

class SharedCount {
public:
    explicit SharedCount(size_t refs = 0) noexcept : shared_owners_{refs} {
    }

    void AddShared() noexcept {
        ++shared_owners_;
    }

    bool ReleaseShared() noexcept {
        if (--shared_owners_ == -1) {
            OnZeroShared();
            return true;
        }

        return false;
    }

    size_t UseCount() const noexcept {
        return shared_owners_.load() + 1;
    }

    virtual void OnZeroShared() = 0;

protected:
    std::atomic<size_t> shared_owners_;
    virtual ~SharedCount() = default;
};

class SharedWeakCount : public SharedCount {
public:
    explicit SharedWeakCount(size_t refs = 0) noexcept
        : SharedCount{refs}, shared_weak_owners_{refs} {
    }

    void AddShared() noexcept {
        SharedCount::AddShared();
    }

    void AddWeak() noexcept {
        ++shared_weak_owners_;
    }

    void ReleaseShared() noexcept {
        if (SharedCount::ReleaseShared()) {
            ReleaseWeak();
        }
    }

    void ReleaseWeak() noexcept {
        --shared_weak_owners_;
    }

    size_t UseCount() const noexcept {
        return SharedCount::UseCount();
    }

    virtual SharedWeakCount* Lock() {
        return nullptr;
    }
    virtual void OnZeroSharedWeak() noexcept {};

private:
    std::atomic<size_t> shared_weak_owners_;

protected:
    virtual ~SharedWeakCount(){};
};

template <typename T, typename Deleter>
class ControlBlock : public SharedWeakCount {
public:
    ControlBlock(T pointer) : pointer_{pointer} {
    }

    ControlBlock(T pointer, Deleter deleter) : pointer_{pointer}, deleter_{deleter} {
    }

    ControlBlock(ControlBlock&) = delete;
    void operator=(ControlBlock&) = delete;

    void OnZeroShared() noexcept override {
        deleter_(pointer_);
    }

    SharedWeakCount* Lock() override {
        if (UseCount() > 0) {
            ControlBlock* newBlock = new ControlBlock{pointer_};
            newBlock->AddShared();
            return newBlock;
        }

        return nullptr;
    }

private:
    T pointer_;
    Deleter deleter_;
};