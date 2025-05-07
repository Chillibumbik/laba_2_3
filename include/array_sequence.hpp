#pragma once

#include "sequence.hpp"
#include "dynamic_array.hpp"
#include "errors.hpp"
#include <stdexcept>

// Изменяемая версия — базовый класс
template <typename T>
class MutableArraySequence : public Sequence<T> {
protected:
    DynamicArray<T>* items;

    Sequence<T>* CreateFromArray(DynamicArray<T>* array) const;

public:
    MutableArraySequence();
    MutableArraySequence(T* arr, int count);
    MutableArraySequence(const MutableArraySequence<T>& other);
    MutableArraySequence(const DynamicArray<T>& array);
    ~MutableArraySequence() override;

    T GetFirst() const override;
    T GetLast() const override;
    T Get(int index) const override;
    int GetLength() const override;

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override;
    Sequence<T>* Concat(const Sequence<T>* other) const override;

    Sequence<T>* Append(T item) override;
    Sequence<T>* Prepend(T item) override;
    Sequence<T>* InsertAt(T item, int index) override;
    Sequence<T>* Remove(int index) override;

    Sequence<T>* Instance() override;
    Sequence<T>* Clone() const override;
};

template <typename T>
MutableArraySequence<T>::MutableArraySequence() {
    items = new DynamicArray<T>(0);
}

template <typename T>
MutableArraySequence<T>::MutableArraySequence(T* arr, int count) {
    items = new DynamicArray<T>(arr, count);
}

template <typename T>
MutableArraySequence<T>::MutableArraySequence(const MutableArraySequence<T>& other) {
    items = new DynamicArray<T>(*other.items);
}

template <typename T>
MutableArraySequence<T>::MutableArraySequence(const DynamicArray<T>& array) {
    items = new DynamicArray<T>(array);
}

template <typename T>
MutableArraySequence<T>::~MutableArraySequence() {
    delete items;
}

template <typename T>
T MutableArraySequence<T>::GetFirst() const {
    if (GetLength() == 0) throw Errors::EmptyArray();
    return items->Get(0);
}

template <typename T>
T MutableArraySequence<T>::GetLast() const {
    if (GetLength() == 0) throw Errors::EmptyArray();
    return items->Get(GetLength() - 1);
}

template <typename T>
T MutableArraySequence<T>::Get(int index) const {
    return items->Get(index);
}

template <typename T>
int MutableArraySequence<T>::GetLength() const {
    return items->GetSize();
}

template <typename T>
Sequence<T>* MutableArraySequence<T>::GetSubsequence(int startIndex, int endIndex) const {
    DynamicArray<T>* sub = items->GetSubArray(startIndex, endIndex);
    auto* result = new MutableArraySequence<T>(*sub);
    delete sub;
    return result;
}

template <typename T>
Sequence<T>* MutableArraySequence<T>::Concat(const Sequence<T>* other) const {
    auto otherArray = dynamic_cast<const MutableArraySequence<T>*>(other);
    if (!otherArray) throw Errors::IncompatibleTypes();

    int totalSize = GetLength() + otherArray->GetLength();
    DynamicArray<T>* result = new DynamicArray<T>(totalSize);

    for (int i = 0; i < GetLength(); i++) result->Set(i, items->Get(i));
    for (int j = 0; j < otherArray->GetLength(); j++) result->Set(j + GetLength(), otherArray->Get(j));

    return CreateFromArray(result);
}

template <typename T>
Sequence<T>* MutableArraySequence<T>::Append(T item) {
    int size = items->GetSize();
    items->Resize(size + 1);
    items->Set(size, item);
    return this;
}

template <typename T>
Sequence<T>* MutableArraySequence<T>::Prepend(T item) {
    int size = items->GetSize();
    items->Resize(size + 1);
    for (int i = size; i > 0; i--)
        items->Set(i, items->Get(i - 1));
    items->Set(0, item);
    return this;
}

template <typename T>
Sequence<T>* MutableArraySequence<T>::InsertAt(T item, int index) {
    int size = items->GetSize();
    if (index < 0 || index > size) throw Errors::IndexOutOfRange();
    items->Resize(size + 1);
    for (int i = size; i > index; i--)
        items->Set(i, items->Get(i - 1));
    items->Set(index, item);
    return this;
}

template <typename T>
Sequence<T>* MutableArraySequence<T>::Remove(int index) {
    if (items->GetSize() == 0) throw Errors::EmptyArray();
    items->Remove(index);
    return this;
}

template <typename T>
Sequence<T>* MutableArraySequence<T>::Instance() {
    return this;
}

template <typename T>
Sequence<T>* MutableArraySequence<T>::Clone() const {
    return new MutableArraySequence<T>(*this);
}

template <typename T>
Sequence<T>* MutableArraySequence<T>::CreateFromArray(DynamicArray<T>* array) const {
    return new MutableArraySequence<T>(*array);
}

template <typename T>
MutableArraySequence<T> operator+(const MutableArraySequence<T>& lhs, const MutableArraySequence<T>& rhs) {
    Sequence<T>* resultBase = lhs.Concat(&rhs);
    auto* result = static_cast<MutableArraySequence<T>*>(resultBase);
    MutableArraySequence<T> copy(*result);
    delete result;
    return copy;
}

template <typename T>
MutableArraySequence<T> operator==(const MutableArraySequence<T>& first, const MutableArraySequence<T>& second) { // надо еще сделать сравнение последовательности со статичным массивом
    
}

// Неизменяемая версия
template <typename T>
class ImmutableArraySequence : public MutableArraySequence<T> {
public:
    using MutableArraySequence<T>::MutableArraySequence;

    Sequence<T>* Concat(const Sequence<T>* other) const override;
    Sequence<T>* Append(T item) override;
    Sequence<T>* Prepend(T item) override;
    Sequence<T>* InsertAt(T item, int index) override;
    Sequence<T>* Remove(int index) override;

    Sequence<T>* Instance() override;
    Sequence<T>* Clone() const override;
};

template <typename T>
Sequence<T>* ImmutableArraySequence<T>::Concat(const Sequence<T>* other) const {
    const auto* otherArr = dynamic_cast<const ImmutableArraySequence<T>*>(other);
    if (!otherArr) throw Errors::IncompatibleTypes();

    int totalSize = this->GetLength() + otherArr->GetLength();
    DynamicArray<T> combined(totalSize);

    for (int i = 0; i < this->GetLength(); ++i)
        combined.Set(i, this->Get(i));
    for (int j = 0; j < otherArr->GetLength(); ++j)
        combined.Set(j + this->GetLength(), otherArr->Get(j));

    return new ImmutableArraySequence<T>(combined);
}

template <typename T>
Sequence<T>* ImmutableArraySequence<T>::Append(T item) {
    auto* clone = new ImmutableArraySequence<T>(*this);
    clone->MutableArraySequence<T>::Append(item);
    return clone;
}

template <typename T>
Sequence<T>* ImmutableArraySequence<T>::Prepend(T item) {
    auto* clone = new ImmutableArraySequence<T>(*this);
    clone->MutableArraySequence<T>::Prepend(item);
    return clone;
}

template <typename T>
Sequence<T>* ImmutableArraySequence<T>::InsertAt(T item, int index) {
    auto* clone = new ImmutableArraySequence<T>(*this);
    clone->MutableArraySequence<T>::InsertAt(item, index);
    return clone;
}

template <typename T>
Sequence<T>* ImmutableArraySequence<T>::Remove(int index) {
    return this->Clone()->Remove(index);
}

template <typename T>
Sequence<T>* ImmutableArraySequence<T>::Instance() {
    return this->Clone();
}

template <typename T>
Sequence<T>* ImmutableArraySequence<T>::Clone() const {
    return new ImmutableArraySequence<T>(*this);
}

template <typename T>
ImmutableArraySequence<T> operator+(const ImmutableArraySequence<T>& lhs, const ImmutableArraySequence<T>& rhs) {
    Sequence<T>* resultBase = lhs.Concat(&rhs);
    auto* result = dynamic_cast<ImmutableArraySequence<T>*>(resultBase);
    if (!result) throw std::runtime_error("Invalid Concat result type");
    ImmutableArraySequence<T> copy(*result);
    delete result;
    return copy;
}

