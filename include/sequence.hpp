
#pragma once

#include <stdexcept>
#include "errors.hpp"

template <class T>
class Sequence {
public:
    virtual ~Sequence() = default;

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(int index) const = 0;
    virtual int GetLength() const = 0;
    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const = 0;

    virtual Sequence<T>* Remove(int index) = 0;

    virtual Sequence<T>* Append(T item) = 0;
    virtual Sequence<T>* Prepend(T item) = 0;
    virtual Sequence<T>* InsertAt(T item, int index) = 0;
    virtual Sequence<T>* Concat(const Sequence<T>* other) const = 0;
    
    virtual Sequence<T>* Instance() = 0;
    virtual Sequence<T>* Clone() const = 0;
};

template<typename T>
Sequence<T>* operator+(const Sequence<T>& lhs, const Sequence<T>& rhs) {
    if (typeid(lhs) != typeid(rhs))
        throw Errors::ConcatTypeMismatchError();

    return lhs.Concat(&rhs);
}

template<typename T>
bool operator==(const Sequence<T>& lhs, const Sequence<T>& rhs) {
    if (lhs.GetLength() != rhs.GetLength()) return false;
    for (int i = 0; i < lhs.GetLength(); ++i)
        if (lhs.Get(i) != rhs.Get(i)) return false;
    return true;
}
