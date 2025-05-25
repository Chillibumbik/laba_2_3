#pragma once
#include <stdexcept>
#include "errors.hpp"

template <class T>
class DynamicArray {
private:
    T* data;
    int size;
    int capacity;

public:
//    DynamicArray(); можно сделать, если сделать, чтобы вылетало уведомление о пропущенных полях
    DynamicArray(T* items, int count);
    DynamicArray(int size);
    DynamicArray(const DynamicArray<T>& other);
    ~DynamicArray();

    void EnsureCapacity(int newCapacity);

    T Get(int index) const;
    int GetSize() const;
    int GetCapacity() const;

    void Remove(int index);

    void Set(int index, T value);
    void Resize(int newSize);
    DynamicArray<T>* GetSubArray(int startIndex, int endIndex) const;


    T& operator[](int index);
    const T& operator[](int index) const;

    T& GetRef(int index) {
        if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
        return data[index];
    }
    
};


template <class T>
DynamicArray<T>::DynamicArray(T* items, int count) {
    if (count < 0)
        throw Errors::NegativeSize();

    size = count;
    capacity = count;

    data = new T[size];
    for (int i = 0; i < size; i++)
        data[i] = items[i];
}

template <class T>
DynamicArray<T>::DynamicArray(int size) {
    if (size < 0)
        throw Errors::NegativeSize();

    this->capacity = size;    
    this->size = size;
    data = new T[size];
}

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) {
    size = other.size;
    capacity = other.capacity;
    data = new T[size];
    for (int i = 0; i < size; i++)
        data[i] = other.data[i];
}

template <class T>
DynamicArray<T>::~DynamicArray() {
}

template <class T>
void DynamicArray<T>::EnsureCapacity(int newCapacity) {
    if (newCapacity < 0)
        throw Errors::NegativeSize();

    if (newCapacity > capacity) {
        T* newData = new T[newCapacity];
        for (int i = 0; i < size; i++)
            newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }
}

template <class T>
T DynamicArray<T>::Get(int index) const {
    if (index < 0 || index >= size)
        throw Errors::IndexOutOfRange();
    return data[index];
}

template <class T>
int DynamicArray<T>::GetSize() const {
    return size;
}

template <class T>
int DynamicArray<T>::GetCapacity() const {
    return capacity;
}

template <class T>
void DynamicArray<T>::Remove(int index) {
    if (size == 0) return;

    if (index < 0 || index >= size)
        throw Errors::IndexOutOfRange();

    for (int i = index; i < size - 1; ++i) {
        data[i] = data[i + 1];
    }

    size--;
}



template <class T>
void DynamicArray<T>::Set(int index, T value) {
    if (index < 0 || index >= size)
        throw Errors::IndexOutOfRange();
    data[index] = value;
}

template <class T>
void DynamicArray<T>::Resize(int newSize) {
    if (newSize < 0)
        throw Errors::NegativeSize();

    if (newSize > capacity)
        EnsureCapacity(newSize);

    size = newSize;
}


template <class T>
DynamicArray<T>* DynamicArray<T>::GetSubArray(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= size || startIndex > endIndex)
        throw Errors::InvalidIndices();

    int count = endIndex - startIndex + 1;
    
    DynamicArray<T>* result = new DynamicArray<T>(count);
    for (int i = 0; i < count; ++i) {
        result->Set(i, data[startIndex + i]);
    }
    return result;
    
}


template <class T>
T& DynamicArray<T>::operator[](int index) {
    if (index < 0 || index >= size)
        throw Errors::IndexOutOfRange();
    return data[index];
}

template <class T>
const T& DynamicArray<T>::operator[](int index) const {
    if (index < 0 || index >= size)
        throw Errors::IndexOutOfRange();
    return data[index];
}

template<typename T>
bool operator==(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) return false;
    for (int i = 0; i < lhs.GetSize(); ++i)
        if (lhs.Get(i) != rhs.Get(i)) return false;
    return true;
}
