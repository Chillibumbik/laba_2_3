#pragma once

#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "errors.hpp"

template <class T>
class Queue {
public:
    virtual ~Queue() = default;

    virtual void Enqueue(const T& item) = 0;
    virtual T Dequeue() = 0;
    virtual T Peek() const = 0;

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(int index) const = 0;

    virtual int GetLength() const = 0;
    virtual bool IsEmpty() const = 0;

    virtual void Clear() = 0;
    
    virtual void Map(void (*func)(T&)) const = 0;
    virtual void Where(bool (*func)(T&)) const = 0;
    virtual T Reduce(T (*func)(const T&, const T&)) const = 0;
};


template <class T>
class ArrayQueue : public MutableArraySequence<T>, public Queue<T> {
public:
    ArrayQueue();
    ArrayQueue(T* items, int count);
    ArrayQueue(const ArrayQueue<T>& other);
    ~ArrayQueue() override;

    void Enqueue(const T& item) override;
    T Dequeue() override;
    T Peek() const override;

    T GetFirst() const override;
    T GetLast() const override;
    T Get(int index) const override;

    int GetLength() const override;
    bool IsEmpty() const override;

    void Clear() override;

    void Map(void (*func)(T&)) const override;
    void Where(bool (*func)(T&)) const override;
    T Reduce(T (*func)(const T&, const T&)) const override;

    ArrayQueue<T> Concat(const ArrayQueue<T>& other) const;
    ArrayQueue<T> Clutch(const ArrayQueue<T>& other) const;

    ArrayQueue<T> GetSubQueue(int startIndex, int endIndex) const;

    
};

template <typename T>
ArrayQueue<T>::ArrayQueue() : MutableArraySequence<T>() {}

template <typename T>
ArrayQueue<T>::ArrayQueue(T* items, int count) : MutableArraySequence<T>(items, count) {}

template <typename T>
ArrayQueue<T>::ArrayQueue(const ArrayQueue<T>& other) : MutableArraySequence<T>(other) {}

template <typename T>
ArrayQueue<T>::~ArrayQueue() = default;

template <typename T>
void ArrayQueue<T>::Enqueue(const T& item) {
    this->Append(item);
}

template <typename T>
T ArrayQueue<T>::Dequeue() {
    if (this->IsEmpty()) throw Errors::EmptyArray();
    T value = this->GetFirst();
    this->Remove(0);
    return value;
}

template <typename T>
T ArrayQueue<T>::Peek() const {
    return this->GetFirst();
}

template <typename T>
T ArrayQueue<T>::GetFirst() const {
    return MutableArraySequence<T>::GetFirst();
}

template <typename T>
T ArrayQueue<T>::GetLast() const {
    return MutableArraySequence<T>::GetLast();
}

template <typename T>
T ArrayQueue<T>::Get(int index) const {
    return MutableArraySequence<T>::Get(index);
}

template <typename T>
int ArrayQueue<T>::GetLength() const {
    return MutableArraySequence<T>::GetLength();
}

template <typename T>
bool ArrayQueue<T>::IsEmpty() const {
    return this->GetLength() == 0;
}

template <typename T>
void ArrayQueue<T>::Clear() {
    delete this->items;
    this->items = new DynamicArray<T>(0);
}

template <typename T>
void ArrayQueue<T>::Map(void (*func)(T&)) const {
    for (int i = 0; i < this->GetLength(); ++i) {
        func(this->items->GetRef(i));
    }
}

template <typename T>
void ArrayQueue<T>::Where(bool (*func)(T&)) const {
    for (int i = 0; i < this->GetLength(); ++i) {
        if (!func(this->items->GetRef(i))) {
            this->items->Remove(i--);
        }
    }
}

template <typename T>
T ArrayQueue<T>::Reduce(T (*func)(const T&, const T&)) const {
    if (this->IsEmpty()) throw Errors::EmptyArray();
    T result = this->Get(0);
    for (int i = 1; i < this->GetLength(); ++i)
        result = func(result, this->Get(i));
    return result;
}

template <typename T>
ArrayQueue<T> ArrayQueue<T>::Concat(const ArrayQueue<T>& other) const {
    return *static_cast<ArrayQueue<T>*>(this->Concat(&other));
}

template <typename T>
ArrayQueue<T> ArrayQueue<T>::Clutch(const ArrayQueue<T>& other) const {
    ArrayQueue<T> result;
    int len1 = this->GetLength();
    int len2 = other.GetLength();
    int minLen = std::min(len1, len2);

    for (int i = 0; i < minLen; ++i) {
        result.Enqueue(this->Get(i));
        result.Enqueue(other.Get(i));
    }
    for (int i = minLen; i < len1; ++i) result.Enqueue(this->Get(i));
    for (int i = minLen; i < len2; ++i) result.Enqueue(other.Get(i));
    return result;
}

template <typename T>
ArrayQueue<T> ArrayQueue<T>::GetSubQueue(int startIndex, int endIndex) const {
    auto* sub = this->GetSubsequence(startIndex, endIndex);
    auto* casted = dynamic_cast<ArrayQueue<T>*>(sub);
    if (!casted) throw Errors::IncompatibleTypes();
    ArrayQueue<T> result(*casted);
    delete casted;
    return result;
}

template <class T>
class ListQueue : public MutableListSequence<T>, public Queue<T> {
public:
    ListQueue();
    ListQueue(T* items, int count);
    ListQueue(const ListQueue<T>& other);
    ~ListQueue() override;

    void Enqueue(const T& item) override;
    T Dequeue() override;
    T Peek() const override;

    T GetFirst() const override;
    T GetLast() const override;
    T Get(int index) const override;

    int GetLength() const override;
    bool IsEmpty() const override;

    void Clear() override;

    void Map(void (*func)(T&)) const override;
    void Where(bool (*func)(T&)) const override;
    T Reduce(T (*func)(const T&, const T&)) const override;

    ListQueue<T> Concat(const ArrayQueue<T>& other) const;
    ListQueue<T> Clutch(const ArrayQueue<T>& other) const;

    ListQueue<T> GetSubQueue(int startIndex, int endIndex) const;
};

template <typename T>
ListQueue<T>::ListQueue() : MutableListSequence<T>() {}

template <typename T>
ListQueue<T>::ListQueue(T* items, int count) : MutableListSequence<T>(items, count) {}

template <typename T>
ListQueue<T>::ListQueue(const ListQueue<T>& other) : MutableListSequence<T>(other) {}

template <typename T>
ListQueue<T>::~ListQueue() = default;

template <typename T>
void ListQueue<T>::Enqueue(const T& item) {
    this->Append(item);
}

template <typename T>
T ListQueue<T>::Dequeue() {
    if (this->IsEmpty()) throw Errors::EmptyList();
    T value = this->GetFirst();
    this->Remove(0);
    return value;
}

template <typename T>
T ListQueue<T>::Peek() const {
    return this->GetFirst();
}

template <typename T>
T ListQueue<T>::GetFirst() const {
    return MutableListSequence<T>::GetFirst();
}

template <typename T>
T ListQueue<T>::GetLast() const {
    return MutableListSequence<T>::GetLast();
}

template <typename T>
T ListQueue<T>::Get(int index) const {
    return MutableListSequence<T>::Get(index);
}

template <typename T>
int ListQueue<T>::GetLength() const {
    return MutableListSequence<T>::GetLength();
}

template <typename T>
bool ListQueue<T>::IsEmpty() const {
    return this->GetLength() == 0;
}

template <typename T>
void ListQueue<T>::Clear() {
    delete this->list;
    this->list = new LinkedList<T>();
}

template <typename T>
void ListQueue<T>::Map(void (*func)(T&)) const {
    for (int i = 0; i < this->GetLength(); ++i) {
        func(this->list->GetRef(i));
    }
}

template <typename T>
void ListQueue<T>::Where(bool (*func)(T&)) const {
    for (int i = 0; i < this->GetLength(); ++i) {
        if (!func(this->list->GetRef(i))) {
            this->list->Remove(i--);
        }
    }
}

template <typename T>
T ListQueue<T>::Reduce(T (*func)(const T&, const T&)) const {
    if (this->IsEmpty()) throw Errors::EmptyArray();
    T result = this->Get(0);
    for (int i = 1; i < this->GetLength(); ++i)
        result = func(result, this->Get(i));
    return result;
}

template <typename T>
ListQueue<T> ListQueue<T>::Concat(const ArrayQueue<T>& other) const {
    ListQueue<T> result(*this);
    for (int i = 0; i < other.GetLength(); ++i)
        result.Enqueue(other.Get(i));
    return result;
}

template <typename T>
ListQueue<T> ListQueue<T>::Clutch(const ArrayQueue<T>& other) const {
    ListQueue<T> result;
    int len1 = this->GetLength();
    int len2 = other.GetLength();
    int minLen = std::min(len1, len2);

    for (int i = 0; i < minLen; ++i) {
        result.Enqueue(this->Get(i));
        result.Enqueue(other.Get(i));
    }
    for (int i = minLen; i < len1; ++i) result.Enqueue(this->Get(i));
    for (int i = minLen; i < len2; ++i) result.Enqueue(other.Get(i));
    return result;
}

template <typename T>
ListQueue<T> ListQueue<T>::GetSubQueue(int startIndex, int endIndex) const {
    auto* sub = this->GetSubsequence(startIndex, endIndex);
    auto* casted = dynamic_cast<ListQueue<T>*>(sub);
    if (!casted) throw Errors::IncompatibleTypes();
    ListQueue<T> result(*casted);
    delete casted;
    return result;
}