#pragma once

#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "errors.hpp"

template <class T>
class Deque {
public:
    virtual ~Deque() = default;

    virtual void PushFront(const T& item) = 0;
    virtual void PushBack(const T& item) = 0;
    virtual T PopFront() = 0;
    virtual T PopBack() = 0;
    virtual T Front() const = 0;
    virtual T Back() const = 0;

    virtual T Get(int index) const = 0;
    virtual int GetLength() const = 0;
    virtual bool IsEmpty() const = 0;

    virtual void Clear() = 0;
};

// ArrayDeque

template <class T>
class ArrayDeque : public MutableArraySequence<T>, public Deque<T> {
public:
    ArrayDeque();
    ArrayDeque(T* items, int count);
    ArrayDeque(const ArrayDeque<T>& other);
    ~ArrayDeque() override;

    void PushFront(const T& item) override;
    void PushBack(const T& item) override;
    T PopFront() override;
    T PopBack() override;
    T Front() const override;
    T Back() const override;

    T Get(int index) const override;
    int GetLength() const override;
    bool IsEmpty() const override;

    void Clear() override;
};

template <typename T>
ArrayDeque<T>::ArrayDeque() : MutableArraySequence<T>() {}

template <typename T>
ArrayDeque<T>::ArrayDeque(T* items, int count) : MutableArraySequence<T>(items, count) {}

template <typename T>
ArrayDeque<T>::ArrayDeque(const ArrayDeque<T>& other) : MutableArraySequence<T>(other) {}

template <typename T>
ArrayDeque<T>::~ArrayDeque() = default;

template <typename T>
void ArrayDeque<T>::PushFront(const T& item) {
    this->Prepend(item);
}

template <typename T>
void ArrayDeque<T>::PushBack(const T& item) {
    this->Append(item);
}

template <typename T>
T ArrayDeque<T>::PopFront() {
    if (this->IsEmpty()) throw Errors::EmptyArray();
    T val = this->GetFirst();
    this->Remove(0);
    return val;
}

template <typename T>
T ArrayDeque<T>::PopBack() {
    if (this->IsEmpty()) throw Errors::EmptyArray();
    T val = this->GetLast();
    this->Remove(this->GetLength() - 1);
    return val;
}

template <typename T>
T ArrayDeque<T>::Front() const {
    return this->GetFirst();
}

template <typename T>
T ArrayDeque<T>::Back() const {
    return this->GetLast();
}

template <typename T>
T ArrayDeque<T>::Get(int index) const {
    return this->MutableArraySequence<T>::Get(index);
}

template <typename T>
int ArrayDeque<T>::GetLength() const {
    return this->MutableArraySequence<T>::GetLength();
}

template <typename T>
bool ArrayDeque<T>::IsEmpty() const {
    return this->GetLength() == 0;
}

template <typename T>
void ArrayDeque<T>::Clear() {
    delete this->items;
    this->items = new DynamicArray<T>(0);
}

// ListDeque

template <class T>
class ListDeque : public MutableListSequence<T>, public Deque<T> {
public:
    ListDeque();
    ListDeque(T* items, int count);
    ListDeque(const ListDeque<T>& other);
    ~ListDeque() override;

    void PushFront(const T& item) override;
    void PushBack(const T& item) override;
    T PopFront() override;
    T PopBack() override;
    T Front() const override;
    T Back() const override;

    T Get(int index) const override;
    int GetLength() const override;
    bool IsEmpty() const override;

    void Clear() override;
};

template <typename T>
ListDeque<T>::ListDeque() : MutableListSequence<T>() {}

template <typename T>
ListDeque<T>::ListDeque(T* items, int count) : MutableListSequence<T>(items, count) {}

template <typename T>
ListDeque<T>::ListDeque(const ListDeque<T>& other) : MutableListSequence<T>(other) {}

template <typename T>
ListDeque<T>::~ListDeque() = default;

template <typename T>
void ListDeque<T>::PushFront(const T& item) {
    this->Prepend(item);
}

template <typename T>
void ListDeque<T>::PushBack(const T& item) {
    this->Append(item);
}

template <typename T>
T ListDeque<T>::PopFront() {
    if (this->IsEmpty()) throw Errors::EmptyList();
    T val = this->GetFirst();
    this->Remove(0);
    return val;
}

template <typename T>
T ListDeque<T>::PopBack() {
    if (this->IsEmpty()) throw Errors::EmptyList();
    T val = this->GetLast();
    this->Remove(this->GetLength() - 1);
    return val;
}

template <typename T>
T ListDeque<T>::Front() const {
    return this->GetFirst();
}

template <typename T>
T ListDeque<T>::Back() const {
    return this->GetLast();
}

template <typename T>
T ListDeque<T>::Get(int index) const {
    return this->MutableListSequence<T>::Get(index);
}

template <typename T>
int ListDeque<T>::GetLength() const {
    return this->MutableListSequence<T>::GetLength();
}

template <typename T>
bool ListDeque<T>::IsEmpty() const {
    return this->GetLength() == 0;
}

template <typename T>
void ListDeque<T>::Clear() {
    delete this->list;
    this->list = new LinkedList<T>();
}
