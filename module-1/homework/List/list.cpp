#include "list.h"

task::list::list(size_t count, const int& value) {
	for (size_t i = 0; i < count; ++i) {
		push_back(value);
	}
}

void task::list::push_back(const int& value) {
	node* newNode = new node(value, tail, nullptr);
	if (head != nullptr) {
		tail->right = newNode;
		tail = newNode;
	}
	else {
		head = newNode;
		tail = newNode;
	}
	++container_size;
}

void task::list::push_back(node* newNode) {
	if (head != nullptr) {
		tail->right = newNode;
		newNode->left = tail;
		newNode->right = nullptr;
		tail = newNode;
	}
	else {
		newNode->right = nullptr;
		newNode->right = nullptr;
		head = newNode;
		tail = newNode;
	}
	++container_size;
}

void task::list::pop_back() {
	if (container_size > 1) {
		tail = tail->left;
		delete tail->right;
		tail->right = nullptr;
		--container_size;
	}

	if (container_size == 1) {
		delete head;
		head = nullptr;
		tail = nullptr;
		--container_size;
	}
}

void task::list::push_front(const int& value) {
	node* newNode = new node(value, nullptr, head);
	if (head != nullptr) {
		head->left = newNode;
		head = newNode;
	}
	else {
		head = newNode;
		tail = newNode;

	}
	++container_size;
}

void task::list::pop_front() {
	if (container_size > 1) {
		head = head->right;
		delete head->left;
		head->left = nullptr;
		--container_size;
	}

	if (container_size == 1) {
		delete head;
		head = nullptr;
		tail = nullptr;
		--container_size;
	}
}

size_t task::list::size() const {
	return container_size;
}

bool task::list::empty() const {
	return head != nullptr;
}

void task::list::clear() {
	if (head == nullptr)
		return;

	node* buf = head->right;
	delete head;
	while (buf != nullptr) {
		head = buf;
		buf = buf->right;
		delete head;
	}

	container_size = 0;
}

task::list::~list() {
	clear();
}

int& task::list::front() {
	return head->value;
}

const int& task::list::front() const {
	return head->value;
}

int& task::list::back() {
	return tail->value;
}

const int& task::list::back() const {
	return tail->value;
}

void task::list::remove(const int& value) {
	node* current = head;
	while (current != nullptr) {
		if (value == current->value) {
			current = delete_node(current);
		}
		else {
			current = current->right;
		}
	}
}

void task::list::unique() {
	node* current = head;
	while (current != nullptr && current->right != nullptr) {
		if (current->value == current->right->value)
			delete_node(current->right);
		else
			current = current->right;
	}
}

task::list::node* task::list::delete_node(task::list::node* arg) {
	arg->left->right = arg->right;
	arg->right->left = arg->left;

	if (arg == head)
		head = arg->right;

	if (arg == tail)
		tail = arg->left;

	node* buf = arg->right;
	delete arg;
	--container_size;
	return buf;
}

void task::list::swap(task::list& other) {
	node* buf = other.head;

	other.head = head;
	head = buf;

	buf = other.tail;
	other.tail = tail;
	tail = buf;

	int bufInt = other.container_size;
	other.container_size = container_size;
	container_size = bufInt;
}

void task::list::resize(size_t count) {
	while (count > container_size)
		push_back(0);

	while (count < container_size)
		pop_back();
}

task::list& task::list::operator=(const task::list& other) {
	node* current = head;
	node* current_other = other.head;

	if (container_size > other.container_size)
		resize(other.container_size);

	while (current_other != nullptr) {
		if (current == nullptr)
			push_back(current_other->value);
		else {
			current->value = current_other->value;
			current = current->right;
		}

		current_other = current_other->right;
	}

	return *this;
}

void task::list::sort() {
	list* answer = sort(this);
	head = answer->head;
	tail = answer->tail;
	container_size = answer->size();
}

task::list* task::list::sort(task::list* collection) {
	if (collection->size() == 1) {
		return collection;
	}

	int pivot = collection->front();

	list* less;
	list* greater;
	node* buf = collection->head->right;

	while (buf != nullptr) {
		if(buf->value <= pivot)
			less->push_back(buf);
		else
			greater->push_back(buf);

		buf = buf->right;
	}

	less = sort(less);
	greater = sort(greater);
	list* answer;
	answer->head = less->head;
	less->tail->right = collection->head;
	collection->head->left = less->tail;
	collection->head->right = greater->head;
	greater->head->left = collection->head;
	answer->tail = greater->tail;
	answer->container_size = less->size() + greater->size() + 1;
	return answer;
}
