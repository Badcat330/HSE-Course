#pragma once

#include <list>
#include <memory>
#include <type_traits>

namespace task {

template <typename T, typename Allocator = std::allocator<T>>
class List {

public:
    // Iterator define and types define
    class Iterator;
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = Iterator;
    using const_iterator = const Iterator;
    using allocator_type = Allocator;

    // Special member functions
    List() : nil_(alloc_.allocate(1)) {
        alloc_.construct(nil_);
        nil_->next = nil_;
        nil_->prev = nil_;
        size_ = 0;
    }

    List(const List& other) {
        // Clear list
        Clear();
        alloc_.destroy(nil_);
        alloc_.deallocate(nil_, 1);

        // Copy alloc
        alloc_ = node_alloc_traits::select_on_container_copy_construction(other.alloc_);

        // Fill list
        nil_ = static_cast<Node*>(alloc_.allocate(1));
        alloc_.construct(nil_);
        nil_->setNext(nil_);
        nil_->setPrev(nil_);

        for (auto iter = other.Begin(); iter != other.End(); ++iter) {
            PushBack(*iter);
        }
    }

    List(const List& other, const Allocator& alloc) {
        // Clear list
        Clear();
        alloc_.destroy(nil_);
        alloc_.deallocate(nil_, 1);

        // Copy alloc
        alloc_ = alloc;

        // Fill list
        nil_ = static_cast<Node*>(alloc_.allocate(1));
        alloc_.construct(nil_);
        nil_->setNext(nil_);
        nil_->setPrev(nil_);

        for (auto iter = other.Begin(); iter != other.End(); ++iter) {
            PushBack(*iter);
        }
    }

    List(List&& other) {
        nil_ = alloc_.allocate(1);
        alloc_.construct(nil_);
        nil_->next = nil_;
        nil_->prev = nil_;
        *this = std::move(other);
    }

    List(List&& other, const Allocator& alloc) : alloc_(alloc), nil_(other.nil_), size_(other.size_)  {
        other.nil_ = alloc_.allocate(1);
        alloc_.construct(other.nil_);
        other.nil_->next = other.nil_;
        other.nil_->prev = other.nil_;
        other.size_ = 0;
    }

    ~List() {
        if (nil_ == nullptr) {
            return;
        }

        Clear();
        alloc_.destroy(nil_);
        alloc_.deallocate(nil_, 1);
    }

    List& operator=(const List& other) {
        if (this == &other) {
            return *this;
        }

        // Clear list
        Clear();
        if (node_alloc_traits::propagate_on_container_copy_assignment::value) {
            alloc_ = other.alloc_;
        }

        for (auto iter = other.Begin(); iter != other.End(); ++iter) {
            PushBack(*iter);
        }

        return *this;
    }

    List& operator=(List&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        if (node_alloc_traits::propagate_on_container_move_assignment::value) {
            alloc_ = std::move(other.alloc_);
            Node* buf = nil_;
            nil_ = other.nil_;
            other.nil_ = buf;
            size_ = other.size_;
            other.size_ = 0;
        } else {
            for (auto iter = other.Begin(); iter != other.End(); ++iter) {
                PushBack(std::move(*iter));
            }
            other.Clear();
        }

        return *this;
    }

    // Element access
    reference Front() {
        if (size_ == 0) {
            throw std::runtime_error("Call from empty list.");
        }

        return nil_->next->value;
    }

    const_reference Front() const {
        if (size_ == 0) {
            throw std::runtime_error("Call from empty list.");
        }

        return nil_->next->value;
    }

    reference Back() {
        if (size_ == 0) {
            throw std::runtime_error("Call from empty list.");
        }

        return nil_->prev->value;
    }

    const_reference Back() const {
        if (size_ == 0) {
            throw std::runtime_error("Call from empty list.");
        }

        return nil_->prev->value;
    }

    // Iterators
    iterator Begin() noexcept {
        return iterator(nil_->next);
    }
    const_iterator Begin() const noexcept {
        return const_iterator(nil_->next);
    }

    iterator End() noexcept {
        return iterator(nil_);
    }
    const_iterator End() const noexcept {
        return const_iterator(nil_);
    }

    // Capacity
    bool Empty() const noexcept {
        return size_ == 0;
    }

    size_type Size() const noexcept {
        return size_;
    }

    //    size_type MaxSize() const noexcept;

    // Modifiers
    void Clear() {
        while (size_ != 0) {
            PopBack();
        }
    }

    void Swap(List& other) noexcept {
        if (&other == this) {
            return;
        }

        if (node_alloc_traits::propagate_on_container_swap::value) {
            std::swap(alloc_, other.alloc_);
            std::swap(nil_, other.nil_);
            std::swap(size_, other.size_);
        } else {
            std::swap(nil_, other.nil_);
        }
    }

    void PushBack(const T& value) {
        Node* current_last = nil_->prev;
        Node* new_node = static_cast<Node*>(alloc_.allocate(1));
        alloc_.construct(new_node, nullptr, nullptr, value);
        new_node->next = nil_;
        new_node->prev = current_last;
        current_last->next = new_node;
        nil_->prev = new_node;
        ++size_;
    }

    void PushBack(T&& value) {
        Node* current_last = nil_->prev;
        Node* new_node = static_cast<Node*>(alloc_.allocate(1));
        alloc_.construct(new_node, nullptr, nullptr, std::forward<T>(value));
        new_node->next = nil_;
        new_node->prev = current_last;
        current_last->next = new_node;
        nil_->prev = new_node;
        ++size_;
    }

    template <typename... Args>
    void EmplaceBack(Args&&... args) {
        Node* current_last = nil_->prev;
        Node* new_node = static_cast<Node*>(alloc_.allocate(1));
        alloc_.construct(new_node, nullptr, nullptr, std::forward<Args>(args)...);
        new_node->next = nil_;
        new_node->prev = current_last;
        current_last->next = new_node;
        nil_->prev = new_node;
        ++size_;
    }

    void PopBack() {
        Node* current_last = nil_->prev;

        if (current_last == nil_) {
            throw std::runtime_error("PopBack from empty list");
        }

        current_last->prev->next = nil_;
        nil_->prev = current_last->prev;
        alloc_.destroy(current_last);
        alloc_.deallocate(current_last, 1);
        --size_;
    }

    void PushFront(const T& value) {
        Node* current_front = nil_->next;
        Node* new_node = static_cast<Node*>(alloc_.allocate(1));
        alloc_.construct(new_node, nullptr, nullptr, value);
        new_node->prev = nil_;
        new_node->next = current_front;
        current_front->prev = new_node;
        nil_->next = new_node;
        ++size_;
    }

    void PushFront(T&& value) {
        Node* current_front = nil_->next;
        Node* new_node = static_cast<Node*>(alloc_.allocate(1));
        alloc_.construct(new_node, nullptr, nullptr, std::forward<T>(value));
        new_node->prev = nil_;
        new_node->next = current_front;
        current_front->prev = new_node;
        nil_->next = new_node;
        ++size_;
    }

    template <typename... Args>
    void EmplaceFront(Args&&... args) {
        Node* current_front = nil_->next;
        Node* new_node = static_cast<Node*>(alloc_.allocate(1));
        alloc_.construct(new_node, nullptr, nullptr, std::forward<Args>(args)...);
        new_node->prev = nil_;
        new_node->next = current_front;
        current_front->prev = new_node;
        nil_->next = new_node;
        ++size_;
    }

    void PopFront() {
        Node* current_front = nil_->next;

        if (current_front == nil_) {
            throw std::runtime_error("PopFront from empty list");
        }

        current_front->next->prev = nil_;
        nil_->next = current_front->next;
        alloc_.destroy(current_front);
        alloc_.deallocate(current_front, 1);
        --size_;
    }

    void Resize(size_type count) {
        while (count > size_) {
            PushBack({});
        }

        while (count < size_) {
            PopBack();
        }
    }

    // Operations
    void Remove(const T& value) {
        Node* current = nil_->next;
        while (current != nil_) {
            current = value == current->value ? DeleteNode(current) : current->next;
        }
    }

    void Unique() {
        Node* current = nil_->next;
        while (current != nil_ && current->next != nil_) {
            if (current->value == current->next->value) {
                DeleteNode(current->next);
            } else {
                current = current->next;
            }
        }
    }

    void Sort() {
        List* answer = Sort(this);
        nil_ = answer->nil_;
        size_ = answer->size_;
    }

    allocator_type GetAllocator() const noexcept {
        return allocator_type(alloc_);
    }

private:
    // List Node
    struct Node {
        Node* next;
        Node* prev;
        value_type value;

        Node() : next(nullptr), prev(nullptr), value() {
        }

        Node(Node* prev, Node* next, value_type value) : next(next), prev(prev), value(value) {
        }

        ~Node() {
            next = nullptr;
            prev = nullptr;
        }
    };

    typedef typename std::allocator_traits<Allocator>::template rebind_alloc<Node> node_allocator;
    typedef typename std::allocator_traits<node_allocator> node_alloc_traits;

    // List variables
    node_allocator alloc_;
    Node* nil_;
    size_type size_ = 0;

    // Helpful methods

    Node* DeleteNode(Node* arg) {
        // Change pointers
        arg->prev->next = arg->next;
        arg->next->prev = arg->prev;
        --size_;
        // Destroy object
        Node* buf = arg->next;
        alloc_.destroy(arg);
        alloc_.deallocate(arg, 1);
        return buf;
    }

    List* Sort(List* collection) {
        if (collection->Size() <= 1) {
            return collection;
        }

        int pivot = collection->Front();

        List* less = new List();
        List* greater = new List();
        Node* buf = collection->nil_->next->next;

        while (buf != nullptr) {
            Node* tmp = buf->next;

            if (buf->value <= pivot) {
                less->PushBack(buf->value);
            } else {
                greater->PushBack(buf->value);
            }

            buf = tmp;
        }

        less = Sort(less);
        greater = Sort(greater);
        List* answer = new List();

        if (less->nil_->next) {
            answer->nil_->next = less->nil_->next;
            less->nil_->prev->next = collection->nil_->next;
            collection->nil_->next->prev = less->nil_->prev;
        } else {
            answer->nil_->next = collection->nil_->next;
        }

        collection->nil_->next->next = greater->nil_->next;

        if (greater->nil_->next) {
            greater->nil_->next->prev = collection->nil_->next;
            answer->nil_->prev = greater->nil_->prev;
        } else {
            answer->nil_->prev = collection->nil_->next;
        }

        answer->size_ = less->size_ + greater->size_ + 1;
        return answer;
    }

public:
    class Iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;

        explicit Iterator(Node* ptr) : ptr_(ptr){};

        reference operator*() const {
            return ptr_->value;
        }

        pointer operator->() {
            return &(ptr_->value);
        }

        Iterator& operator++() {
            ptr_ = ptr_->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        Iterator& operator--() {
            ptr_ = ptr_->prev;
            return *this;
        }

        Iterator operator--(int) {
            Iterator tmp = *this;
            --(*this);
            return tmp;
        }

        friend bool operator==(const Iterator& a, const Iterator& b) {
            return a.ptr_ == b.ptr_;
        };

        friend bool operator!=(const Iterator& a, const Iterator& b) {
            return a.ptr_ != b.ptr_;
        };

    private:
        Node* ptr_ = nullptr;
    };
};

}  // namespace task