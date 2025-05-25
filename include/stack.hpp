#pragma once

#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "errors.hpp"


template <class T>
class Stack {
public:
    virtual ~Stack() = default;

    virtual void Push(const T& item) = 0;
    virtual T Pop() = 0;
    virtual T Top() const = 0;

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(int index) const = 0;

    virtual int GetLength() const = 0;
    virtual bool IsEmpty() const = 0;

    virtual void Clear() = 0;
};

template <class T>
class ArrayStack : public MutableArraySequence<T>, public Stack<T> {
public:
    ArrayStack();
    ArrayStack(T* items, int count);
    ArrayStack(const ArrayStack<T>& other);
    ~ArrayStack() override;

    void Push(const T& item) override;
    T Pop() override;
    T Top() const override;

    T GetFirst() const override;
    T GetLast() const override;
    T Get(int index) const override;

    int GetLength() const override;
    bool IsEmpty() const override;

    void Clear() override;
};

template <typename T>
ArrayStack<T>::ArrayStack() : MutableArraySequence<T>() {}

template <typename T>
ArrayStack<T>::ArrayStack(T* items, int count) : MutableArraySequence<T>(items, count) {}

template <typename T>
ArrayStack<T>::ArrayStack(const ArrayStack<T>& other)
    : MutableArraySequence<T>(other) {}

template <typename T>
ArrayStack<T>::~ArrayStack() = default;

// Методы стека
template <typename T>
void ArrayStack<T>::Push(const T& item) {
    this->Append(item);
}

template <typename T>
T ArrayStack<T>::Pop() {
    if (this->IsEmpty()) throw Errors::EmptyStackError();
    T item = this->GetLast();
    this->Remove(this->GetLength() - 1);
    return item;
}

template <typename T>
T ArrayStack<T>::Top() const {
    if (this->IsEmpty()) throw Errors::EmptyStackError();
    return this->GetLast();
}

// Методы последовательности
template <typename T>
T ArrayStack<T>::GetFirst() const { return MutableArraySequence<T>::GetFirst(); }

template <typename T>
T ArrayStack<T>::GetLast() const { return MutableArraySequence<T>::GetLast(); }

template <typename T>
T ArrayStack<T>::Get(int index) const { return MutableArraySequence<T>::Get(index); }

template <typename T>
int ArrayStack<T>::GetLength() const { return MutableArraySequence<T>::GetLength(); }

template <typename T>
bool ArrayStack<T>::IsEmpty() const { return this->GetLength() == 0; }

template <typename T>
void ArrayStack<T>::Clear() {
    while (!IsEmpty()) Pop();
}



template <class T>
class ListStack : public MutableListSequence<T>, public Stack<T> {
public:
    ListStack();
    ListStack(T* items, int count);
    ListStack(const ListStack<T>& other);
    ~ListStack() override;

    void Push(const T& item) override;
    T Pop() override;
    T Top() const override;

    T GetFirst() const override;
    T GetLast() const override;
    T Get(int index) const override;

    int GetLength() const override;
    bool IsEmpty() const override;

    void Clear() override;
};

template <typename T>
ListStack<T>::ListStack() : MutableListSequence<T>() {}

template <typename T>
ListStack<T>::ListStack(T* items, int count) : MutableListSequence<T>(items, count) {}

template <typename T>
ListStack<T>::ListStack(const ListStack<T>& other)
    : MutableListSequence<T>(other) {}

template <typename T>
ListStack<T>::~ListStack() = default;

// Методы стека
template <typename T>
void ListStack<T>::Push(const T& item) {
    this->Append(item);
}

template <typename T>
T ListStack<T>::Pop() {
    if (this->IsEmpty()) throw Errors::EmptyStackError();
    T item = this->GetLast();
    this->Remove(this->GetLength() - 1);
    return item;
}

template <typename T>
T ListStack<T>::Top() const {
    if (this->IsEmpty()) throw Errors::EmptyStackError();
    return this->GetLast();
}

// Методы последовательности
template <typename T>
T ListStack<T>::GetFirst() const { return MutableListSequence<T>::GetFirst(); }

template <typename T>
T ListStack<T>::GetLast() const { return MutableListSequence<T>::GetLast(); }

template <typename T>
T ListStack<T>::Get(int index) const { return MutableListSequence<T>::Get(index); }

template <typename T>
int ListStack<T>::GetLength() const { return MutableListSequence<T>::GetLength(); }

template <typename T>
bool ListStack<T>::IsEmpty() const { return this->GetLength() == 0; }

template <typename T>
void ListStack<T>::Clear() {
    while (!IsEmpty()) Pop();
}