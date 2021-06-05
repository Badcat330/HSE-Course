#include <memory>
#include <type_traits>

template<typename T>
class CustomAllocator {
public:
    template<typename U>
    struct rebind {  // NOLINT
        using other = CustomAllocator<U>;
    };

    using value_type = T;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using size_type = std::size_t;
    using pointer_difference = std::ptrdiff_t;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propogate_on_container_swap = std::false_type;
    using is_always_equal = std::false_type;

    CustomAllocator() {
        arena_ = ::operator new(default_size_ * sizeof(value_type));
        arena_offset_ = new size_type(0);
        num_allocators_ = new size_type(1);
    }

    CustomAllocator(const CustomAllocator &other) noexcept:
            arena_(other.arena_),
            arena_offset_(other.arena_offset_),
            num_allocators_(other.num_allocators_) {
        ++(*num_allocators_);
    }

    ~CustomAllocator() {
        (*num_allocators_)--;
        if (*num_allocators_ == 0) {
            ::operator delete(arena_);
            delete arena_offset_;
            delete num_allocators_;
        }
    }

    template<typename U>
    explicit CustomAllocator(const CustomAllocator<U> &other) noexcept :
            arena_(other.arena_),
            arena_offset_(other.arena_offset_),
            num_allocators_(other.num_allocators_) {
        ++(*num_allocators_);
    }

    T *allocate(size_t n) {  // NOLINT
        pointer next_address = static_cast<pointer>(arena_) + *arena_offset_;
        *arena_offset_ += n;
        return next_address;
    }

    void deallocate(T *p, size_t n) { };

    template<typename... Args>
    void construct(pointer p, Args &&... args) {  // NOLINT
        ::new((void*)p) T(std::forward<Args>(args)...);
    };

    void destroy(pointer p) {  // NOLINT
        delete p;
    };

    template<typename K, typename U>
    friend bool operator==(const CustomAllocator<K> &lhs, const CustomAllocator<U> &rhs) noexcept;

    template<typename K, typename U>
    friend bool operator!=(const CustomAllocator<K> &lhs, const CustomAllocator<U> &rhs) noexcept;

private:
    const size_type default_size_ = 20000;
    size_type *num_allocators_;
    void *arena_ = nullptr;
    size_type *arena_offset_;
};

template<typename T, typename U>
bool operator==(const CustomAllocator<T> &lhs, const CustomAllocator<U> &rhs) noexcept {
    return lhs.arena_ == rhs.arena_;
}

template<typename T, typename U>
bool operator!=(const CustomAllocator<T> &lhs, const CustomAllocator<U> &rhs) noexcept {
    return !(lhs == rhs);
}