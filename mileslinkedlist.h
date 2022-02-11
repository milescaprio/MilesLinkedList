#pragma once
#include <initializer_list>

template <typename T> class mileslinkedlist {
private:
	struct mllentry {
		T elem;
		mllentry* next;
		mllentry* operator[](size_t i); //increment up the list
	};
	mllentry* first_;
	size_t length_;
	static const size_t elemsize = sizeof(T) + sizeof(void*);
	mllentry* entry(size_t i);
public:
	~mileslinkedlist();
	mileslinkedlist();
	mileslinkedlist(std::initializer_list<T> vals);
	T operator[](size_t i) const;
	T& operator[](size_t i);
	void insert(size_t i, T value);
	void remove(size_t i);
	void append(T value);
	void concat(mileslinkedlist<T>& adder);
	void shorten(size_t begin, size_t end);
	size_t length() { return length_; }
};

template <typename T> typename mileslinkedlist<T>::mllentry* mileslinkedlist<T>::entry(size_t i)
{
	if (i >= length_) throw ("Out of bounds");
	mllentry* current = first_;
	for (int j = 0; j < i; j++) {
		current = current->next;
	}
	return current;
}

template <typename T> mileslinkedlist<T>::~mileslinkedlist() {
	mllentry* lastimllentry = first_;
	mllentry* imllentry;
	while (lastimllentry) {
		imllentry = (mllentry*)lastimllentry->next;
		delete lastimllentry;
		lastimllentry = imllentry;
	}
}

template <typename T> mileslinkedlist<T>::mileslinkedlist() {
	first_ = nullptr;
	length_ = 0;
}

template <typename T> mileslinkedlist<T>::mileslinkedlist(std::initializer_list<T> vals) {
	length_ = 0;
	if (vals.size() == 0) {
		first_ = nullptr;
		return;
	}
	mllentry* lastimllentry = new mllentry;
	lastimllentry->next = nullptr;
	lastimllentry->elem = vals.begin()[vals.size() - 1];
	length_++;
	mllentry* imllentry;
	for (size_t i = vals.size() - 1 - 1 + 1; i >= 0 + 1; i--) { //everything added one so it doesnt underflow and continue forever
		imllentry = new mllentry;
		imllentry->next = lastimllentry;
		imllentry->elem = vals.begin()[i - 1];
		lastimllentry = imllentry;
		length_++;
	}
	first_ = lastimllentry;
}

template<typename T> typename mileslinkedlist<T>::mllentry* mileslinkedlist<T>::mllentry::operator[](size_t i)
{
	mllentry* current = this;
	for (int j = 0; j < i; j++) {
		current = current->next;
		if (current == nullptr) throw ("Out of bounds");
	}
	return current;
}


template <typename T> T mileslinkedlist<T>::operator[](size_t i) const
{
	if (i >= length_) throw ("Out of bounds");
	mllentry* current = first_;
	for (int j = 0; j < i; j++) {
		current = current->next;
	}
	return current->elem;
}

template <typename T> T& mileslinkedlist<T>::operator[](size_t i)
{
	if (i >= length_) throw ("Out of bounds");
	mllentry* current = first_;
	for (size_t j = 0; j < i; j++) {
		current = current->next;
	}
	return current->elem;
}

template <typename T> void mileslinkedlist<T>::insert(size_t i, T value)
{
	if (i >= length_) throw ("Out of bounds");
	mllentry* cutoff = entry(i - 1);
	mllentry* inserted = new mllentry;
	inserted->elem = value;
	inserted->next = (*cutoff)[1];
	cutoff->next = inserted;
}

template <typename T> void mileslinkedlist<T>::remove(size_t i)
{
	if (i >= length_) throw ("Out of bounds");
	mllentry* cutoff = entry(i - 1);
	if (i == length_ - 1) {
		cutoff->next = nullptr;
	}
	mllentry* temp = (*cutoff)[1];
	cutoff->next = (*temp)[1]; //1 + 1 instead of 1 then 2 saves a cycle
	delete temp;
	
}

template <typename T> void mileslinkedlist<T>::append(T value)
{
	mllentry* appended = new mllentry;
	mllentry* cutoff = entry(length_ - 1);
	appended->elem = value;
	appended->next = nullptr;
	cutoff->next = appended;
}

template <typename T> void mileslinkedlist<T>::concat(mileslinkedlist<T>& adder)
{
	if (adder.length() == 0) {
		return;
	}
	mllentry* cutoff = entry(length_ - 1);
	mllentry* lastimllentry = new mllentry;
	lastimllentry->next = nullptr;
	lastimllentry->elem = adder[adder.length() - 1];
	length_++;
	mllentry* imllentry;
	for (size_t i = adder.length() - 1 - 1 + 1; i >= 0 + 1; i--) { //everything added one so it doesnt underflow and continue forever
		imllentry = new mllentry;
		imllentry->next = lastimllentry;
		imllentry->elem = adder[i - 1];
		lastimllentry = imllentry;
		length_++;
	}
	cutoff->next = lastimllentry;
}

template <typename T> void mileslinkedlist<T>::shorten(size_t begin, size_t end) {
	size_t diff = end - begin;
	if (begin >= length_ || end >= length_) throw ("Out of bounds");
	mllentry* current = first_;
	mllentry* temp;
	for (int j = 0; j < begin; j++) {
		temp = current->next;
		delete current;
		current = temp;
	}
	first_ = current;
	mllentry* cutoff = current.entry(diff);
	for (int j = 0; j < diff; j++) {
		current = current->next;
	}
	while (current != nullptr) {
		temp = current->next;
		delete current;
		current = temp;
	}
}