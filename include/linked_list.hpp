#pragma once
#include <stdexcept>

#include "errors.hpp"

template <class T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(T data) : data(data), next(nullptr) {}
        Node(T data, Node* next) : data(data), next(next) {}
    };

    Node* root;
    Node* tail;
    int size;

public:
    LinkedList();
    LinkedList(T* items, int count);
    LinkedList(const LinkedList<T>& list);
    ~LinkedList();

    T GetFirst() const;
    T GetLast() const;
    T GetTail() const;
    T Get(int index) const;
    LinkedList<T>* GetSubList(int startIndex, int endIndex) const;
    int GetLength() const;

    void Append(T item);
    void Prepend(T item);
    void InsertAt(T item, int index);
    void Remove(int index);
    LinkedList<T>* Concat(const LinkedList<T>* list);

    T& GetRef(int index) {
        if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
        Node* current = root;
        for (int i = 0; i < index; ++i)
            current = current->next;
        return current->data;
    }
    
};


/*
LinkedList<T>::метод(){}
*/

template <class T>
LinkedList<T>::LinkedList(){
    root = nullptr;
    size = 0;
    tail = nullptr;
}

template <class T>
LinkedList<T>::LinkedList(T* items, int count){
    if (count < 0){
        throw Errors::NegativeCount();
    }

    if (count == 0) {
        root = nullptr;
        size = 0;
        tail = nullptr;
        return;
    }

    size = count;
    root = new Node{items[0], nullptr};
    Node* current = root;

    for(int i = 1; i<count; i++){
        Node* newNode =  new Node{items[i], nullptr};
        current->next = newNode;
        current = newNode;
    }

    tail = current;
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& list){
    if (list.root == nullptr) {
        root = nullptr;
        size = 0;
        tail = nullptr;
        return;
    }

    root = new Node{list.root->data, nullptr};
    Node* currentThis = root;
    Node* currentOther = list.root->next;

    while (currentOther != nullptr) {
        currentThis->next = new Node{currentOther->data, nullptr};
        currentThis = currentThis->next;
        currentOther = currentOther->next;
    }

    tail = currentThis;
    tail->next = nullptr;
    size = list.size;
}

template <class T>
LinkedList<T>::~LinkedList(){
    Node* cur = root;
    while (cur != nullptr) {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;
    }    
}

template <class T>
T LinkedList<T>::GetFirst() const{
    if(root == nullptr){
        throw Errors::EmptyList();
    }

    return root->data;
}

template <class T>
T LinkedList<T>::GetLast() const{
    if(root == nullptr){
        throw Errors::EmptyList();
    }

    Node* cur = root;

    while (cur->next != nullptr){
        cur = cur->next;
    }

    return cur->data;
}

template <class T>
T LinkedList<T>::GetTail() const {
    return tail->data;
}

template <class T>
T LinkedList<T>::Get(int index) const{
    if(root == nullptr){
        throw Errors::EmptyList();
    }

    if(index < 0 || index >= size){
        throw Errors::IndexOutOfRange();
    }

    if(index == 0){
        return root->data;
    }

    Node* cur = root;
    for(int i = 0; i<index; i++){
        cur = cur->next;
    }
    return cur->data;
}

template <class T>
LinkedList<T>* LinkedList<T>::GetSubList(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= size || startIndex > endIndex)
        throw Errors::InvalidIndices();

    LinkedList<T>* sublist = new LinkedList<T>();
    Node* current = root;

    for (int i = 0; i < startIndex; i++) {
        current = current->next;
    }

    sublist->Append(current->data);

    for (int i = startIndex + 1; i <= endIndex; i++) {
        current = current->next;
        sublist->Append(current->data);
    }

    return sublist;
}


template <class T>
int LinkedList<T>::GetLength() const{
    return size;
}

template <class T>
void LinkedList<T>::Append(T item){
   
    Node* newNode = new Node{item, nullptr};  
    if(root == nullptr){
        root = newNode;      
    }else{
        Node* cur = root;
        while (cur->next != nullptr)
        {
            cur = cur->next;
        }
        cur->next = newNode;
    }
    tail = newNode;
    size++;
}

template <class T>
void LinkedList<T>::Prepend(T item){
    Node* newNode = new Node{item, root};
    root = newNode;
    size++;
}

template <class T>
void LinkedList<T>::InsertAt(T item, int index){
    if(index>size || index<0){
        throw Errors::IndexOutOfRange();
    }
    Node* cur = root;
    Node* newNode = new Node{item};
    if(index == 0){
        newNode->next = root;
        root = newNode;
    }else{
        for(int i=0; i<index-1; i++){
            cur = cur->next;
        }
        Node* tmp = cur->next;
        cur->next = newNode;
        newNode->next = tmp;
    }
    size++;
}

template <class T>
void LinkedList<T>::Remove(int index){
    if(size == 0) throw Errors::EmptyList();
    
    if(index<0 || index>=size) throw Errors::IndexOutOfRange();

    Node* cur = root;
    Node* tmp;

    if(index == 0){
        root = cur->next;
        delete(cur);
    }else{
        for(int i=0; i<index-1; i++){
            cur = cur->next;
        }
        tmp = cur->next;
        cur->next=tmp->next;
        delete(tmp);
    }
    size--;
}

template <class T>
LinkedList<T>* LinkedList<T>::Concat(const LinkedList<T>* list){
/*     if (list == nullptr) throw Errors::NullList();

    LinkedList<T>* result = new LinkedList<T>(*this);
    Node* cur = list->root;
    while (cur != nullptr){
        result->Append(cur->data);
        cur = cur->next;
    }
    return result;   */
    if (list == nullptr) throw Errors::NullList();
    
    LinkedList<T>* result = new LinkedList<T>(*this);
    result->tail->next = list->root;

    result->size += list->size;
    result->tail = list->tail;
    return result;
}
