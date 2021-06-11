#include <memory>
#include <type_traits>

template <typename T>
class CustomAllocator {
public:
    template <typename U>
    struct rebind {  // NOLINT
        using other = CustomAllocator<U>;
    };

    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_pointer = const T*;
    using const_reference = const T&;
    using size_type = std::size_t;
    using pointer_difference = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::false_type;

    CustomAllocator() = default;

    CustomAllocator(const CustomAllocator& other) noexcept
        : arena_{other.arena_}, arena_offset_{other.arena_offset_} {
        ++num_allocators_;
    }

    ~CustomAllocator() {
        --num_allocators_;
        if (num_allocators_ == 0) {
            ::operator delete(arena_);
            delete arena_offset_;
        }
    }

    template <typename U>
    explicit CustomAllocator(const CustomAllocator<U>& other) noexcept
        : arena_{other.GetArena()}, arena_offset_{other.GetArenaOffset()} {
        ++num_allocators_;
    }

    void* GetArena() const {
        return arena_;
    }

    size_type* GetArenaOffset() const {
        return arena_offset_;
    }

    T* allocate(size_t n) {  // NOLINT
        pointer next_address = static_cast<pointer>(arena_) + *arena_offset_;
        *arena_offset_ += n;
        return next_address;
    }

    void deallocate(T* p, size_t n){};

    template <typename... Args>
    void construct(pointer p, Args&&... args) {  // NOLINT
        new (p) value_type{std::forward<Args>(args)...};
    };

    void destroy(pointer p) {  // NOLINT
        p->~value_type();
    };

    template <typename K, typename U>
    friend bool operator==(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;

    template <typename K, typename U>
    friend bool operator!=(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;

private:
    static const size_type kDefaultSize{20000};
    static size_type num_allocators_;
    void* arena_{::operator new(kDefaultSize * sizeof(value_type))};
    size_type* arena_offset_{new size_type(0)};
};

template <typename T>
std::size_t CustomAllocator<T>::num_allocators_ = 0;

template <typename T, typename U>
bool operator==(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    return lhs.arena_ == rhs.arena_;
}

template <typename T, typename U>
bool operator!=(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    return !(lhs == rhs);
}