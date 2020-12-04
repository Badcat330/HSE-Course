#pragma once
#include <cstddef>

namespace task
{

	class list
	{

	 public:

		struct node
		{
			node() {
				value = 0;
				left = nullptr;
				right = nullptr;
			}

			node(int _value, node* _left, node* _right) :
				value(_value),
				left(_left),
				right(_right) {
			}

			int value;
			node* left;
			node* right;
		};

		/**
		 * Complexity: Constant
		 */
		list() = default;

		/**
		 * Complexity: Linear in count
		 */
		explicit list(size_t count, const int& value = int());

		/**
		 * Linear in the size of the list
		 */
		~list();

		list(const list& other);

		/**
		 * Linear in the size of *this and other.
		 * Complexity: Linear in the size of *this and other.
		 */
		list& operator=(const list& other);

		/**
		 * Complexity: Constant
		 * @return reference to the first element
		 */
		int& front();
		const int& front() const;

		/**
		 * Complexity: Constant
		 * @return reference to the last element.
		 */
		int& back();
		const int& back() const;

		/**
		 * Complexity: Constant
		 * @return true if the container is empty, false otherwise
		 */
		bool empty() const;

		/**
		 * Complexity: Constant
		 * @return The number of elements in the container.
		 */
		size_t size() const;

		/**
		 * Erases all elements from the container.
		 * Complexity: Linear in the size of the container, i.e., the number of elements.
		 */
		void clear();

		/**
		 * Complexity: Constant
		 * @param value 	the value of the element to append
		 */
		void push_back(const int& value);

		/**
		 * Removes the last element of the container.
		 * Complexity: Constant
		 */
		void pop_back();

		/**
		 * Complexity: Constant
		 * @param value 	the value of the element to append
		 */
		void push_front(const int& value);

		/**
		 * Removes the first element of the container.
		 * Complexity: Constant
		 */
		void pop_front();

		/**
		 * If the current size is greater than count, the container is reduced to its first count elements.
		 * If the current size is less than count, additional default-inserted elements are appended
		 * Complexity: Linear in the difference between the current size and count.
		 * @param count 	new size of the container
		 */
		void resize(size_t count);

		/**
		 * Exchanges the contents of the container with those of other.
		 * Complexity: Constant.
		 * @param other
		 */
		void swap(list& other);

		/**
		 * Removes all elements that are equal to value
		 * Complexity: Linear in the size of the container
		 * @param value 	value of the elements to remove
		 */
		void remove(int value);

		/**
		 * Removes all consecutive duplicate elements from the container.
		 * Complexity: Linear in the size of the container
		 */
		void unique();

		/**
		 * Sorts the elements in ascending order. The order of equal elements is preserved.
		 * Complexity: Approximately N log N comparisons, where N is the number of elements in the list.
		 */
		void sort();

	 private:
		node* head = nullptr;
		node* tail = nullptr;
		size_t container_size = 0;

		list* sort(list* collection);
		void push_back(node* newNode);
		node* delete_node(node* node);
	};

}  // namespace task
