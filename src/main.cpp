#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <typeinfo>

#include "stack.hpp"
#include "queue.hpp"
#include "deque.hpp"
#include "user.hpp"
#include "errors.hpp"

// ------------------------------------------------------------
// Утилиты ввода‑вывода
// ------------------------------------------------------------
void ClearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int GetInt(const std::string& prompt = "") {
    int v;
    if (!prompt.empty()) std::cout << prompt;
    if (!(std::cin >> v)) {
        ClearInput();
        throw Errors::InvalidArgument();
    }
    return v;
}

template<typename T>
T GetTyped(const std::string& prompt = "Enter value: ") {
    if (!prompt.empty()) std::cout << prompt;
    T value{};
    if (!(std::cin >> value)) {
        ClearInput();
        throw Errors::InvalidArgument();
    }
    return value;
}



template<>
Student GetTyped<Student>(const std::string& prompt) { Student s; std::cin >> s; return s; }

template<>
Teacher GetTyped<Teacher>(const std::string& prompt) { Teacher t; std::cin >> t; return t; }

// ------------------------------------------------------------
// Базовый интерфейс
// ------------------------------------------------------------
class IWrapper {
public:
    virtual ~IWrapper() = default;
    virtual void ShowElements() const = 0;
    virtual void Menu() = 0;             
    virtual std::string Info() const = 0; 
};

// ---------------------------------------------------
// Перечисления‑помощники
// ---------------------------------------------------
enum class StructKind { STACK = 1, QUEUE, DEQUE };
static inline std::string ToString(StructKind k) {
    switch (k) {
        case StructKind::STACK: return "stack";
        case StructKind::QUEUE: return "queue";
        case StructKind::DEQUE: return "deque";
    }
    return "?";
}

enum class Container { ARRAY = 1, LIST };
static inline std::string ToString(Container c) {
    return (c == Container::ARRAY ? "array" : "list");
}

// ------------------------------------------------------
// Шаблонные обертк
// -------------------------------------------------------

template<typename T>
class StackWrapper : public IWrapper {
    Container c_;
    std::string typeKey_;
    Stack<T>* st_;

    Stack<T>* makeStack() {
        if (c_ == Container::ARRAY) return new ArrayStack<T>();
        return new ListStack<T>();
    }
public:
    StackWrapper(Container c, const std::string& key) : c_(c), typeKey_(key), st_(makeStack()) {}
    ~StackWrapper() override { delete st_; }

    // IWrapper
    std::string Info() const override {
        return "Stack<" + typeKey_ + ">(" + ToString(c_) + ", size=" + std::to_string(st_->GetLength()) + ")";
    }

    void ShowElements() const override {
        std::cout << "[ ";
        for (int i = 0; i < st_->GetLength(); ++i) std::cout << st_->Get(i) << " ";
        std::cout << "]\n";
    }

    void Menu() override {
        while (true) {
            std::cout << "\n--- Stack menu ---\n";
            std::cout << "1. Push\n2. Pop\n3. Top\n4. Clear\n5. Show elements\n6. Back\nChoose: ";
            try {
                int ch = GetInt();
                switch (ch) {
                    case 1: {
                        T val = GetTyped<T>();
                        st_->Push(val);
                        break;
                    }
                    case 2: {
                        T val = st_->Pop();
                        std::cout << "Popped: " << val << "\n";
                        break;
                    }
                    case 3: {
                        std::cout << "Top: " << st_->Top() << "\n";
                        break;
                    }
                    case 4: st_->Clear(); std::cout << "Cleared\n"; break;
                    case 5: ShowElements(); break;
                    case 6: return; // back
                    default: std::cout << "Invalid\n";
                }
            } catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; }
        }
    }
};

//--------------------------------------------------------------

template<typename T>
class QueueWrapper : public IWrapper {
    Container c_;
    std::string typeKey_;
    Queue<T>* q_;

    Queue<T>* makeQueue() {
        if (c_ == Container::ARRAY) return new ArrayQueue<T>();
        return new ListQueue<T>();
    }
public:
    QueueWrapper(Container c, const std::string& key) : c_(c), typeKey_(key), q_(makeQueue()) {}
    ~QueueWrapper() override { delete q_; }

    std::string Info() const override {
        return "Queue<" + typeKey_ + ">(" + ToString(c_) + ", size=" + std::to_string(q_->GetLength()) + ")";
    }

    void ShowElements() const override {
        std::cout << "[ ";
        for (int i = 0; i < q_->GetLength(); ++i) std::cout << q_->Get(i) << " ";
        std::cout << "]\n";
    }

    void Menu() override {
        while (true) {
            std::cout << "\n--- Queue menu ---\n";
            std::cout << "1. Enqueue\n2. Dequeue\n3. Peek\n4. Clear\n5. Show elements\n6. Back\nChoose: ";
            try {
                int ch = GetInt();
                switch (ch) {
                    case 1: { // Enqueue
                        T val = GetTyped<T>();
                        q_->Enqueue(val);
                        break;
                    }
                    case 2: { // Dequeue
                        T val = q_->Dequeue();
                        std::cout << "Dequeued: " << val << "\n";
                        break;
                    }
                    case 3: { // Peek
                        std::cout << "Front element: " << q_->Peek() << "\n";
                        break;
                    }
                    case 4: q_->Clear(); std::cout << "Cleared\n"; break; 
                    case 5: ShowElements(); break;
                    case 6: return;
                    default: std::cout << "Invalid\n"; break;
                }
            } catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; }
        }
    }
};

//--------------------------------------------------------------------

template<typename T>
class DequeWrapper : public IWrapper {
    Container c_;
    std::string typeKey_;
    Deque<T>* d_;

    Deque<T>* makeDeque() {
        if (c_ == Container::ARRAY) return new ArrayDeque<T>();
        return new ListDeque<T>();
    }
public:
    DequeWrapper(Container c, const std::string& key) : c_(c), typeKey_(key), d_(makeDeque()) {}
    ~DequeWrapper() override { delete d_; }

    std::string Info() const override {
        return "Deque<" + typeKey_ + ">(" + ToString(c_) + ", size=" + std::to_string(d_->GetLength()) + ")";
    }

    void ShowElements() const override {
        std::cout << "[ ";
        for (int i = 0; i < d_->GetLength(); ++i) std::cout << d_->Get(i) << " ";
        std::cout << "]\n";
    }

    void Menu() override {
        while (true) {
            std::cout << "\n--- Deque menu ---\n";
            std::cout << "1. PushFront\n2. PushBack\n3. PopFront\n4. PopBack\n5. Front\n6. Back\n7. Clear\n8. Show elements\n9. Back to main\nChoose: ";
            try {
                int ch = GetInt();
                switch (ch) {
                    case 1: d_->PushFront(GetTyped<T>()); break; 
                    case 2: d_->PushBack(GetTyped<T>()); break;
                    case 3: std::cout << "PopFront: " << d_->PopFront() << "\n"; break;
                    case 4: std::cout << "PopBack: " << d_->PopBack() << "\n"; break;
                    case 5: std::cout << "Front: " << d_->Front() << "\n"; break;
                    case 6: std::cout << "Back: " << d_->Back() << "\n"; break;
                    case 7: d_->Clear(); std::cout << "Cleared\n"; break;
                    case 8: ShowElements(); break;
                    case 9: return;
                    default: std::cout << "Invalid\n";
                }
            } catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; }
        }
    }
};


struct TypeChoice {
    int id; std::string name;
};
static const std::vector<TypeChoice> typeChoices = {
    {1, "int"}, {2, "double"}, {3, "string"}, {4, "student"}, {5, "teacher"}
};

void ShowTypeMenu() {
    std::cout << "Select element type:\n";
    for (auto t : typeChoices) std::cout << t.id << ". " << t.name << "\n";
}

Container AskContainer() {
    std::cout << "Select container implementation:\n1. array\n2. list\nChoice: ";
    int v = GetInt();
    if (v == 1) return Container::ARRAY;
    if (v == 2) return Container::LIST;
    throw Errors::InvalidArgument();
}

StructKind AskStructKind() {
    std::cout << "Select data structure:\n1. stack\n2. queue\n3. deque\nChoice: ";
    int v = GetInt();
    if (v >= 1 && v <= 3) return static_cast<StructKind>(v);
    throw Errors::InvalidArgument();
}

IWrapper* CreateWrapper() {
    StructKind sk = AskStructKind();
    Container cont = AskContainer();
    ShowTypeMenu();
    int typeId = GetInt("Choice: ");

    switch (typeId) {
        case 1: { // int
            if (sk == StructKind::STACK) return new StackWrapper<int>(cont, "int");
            if (sk == StructKind::QUEUE) return new QueueWrapper<int>(cont, "int");
            return new DequeWrapper<int>(cont, "int");
        }
        case 2: { // double
            if (sk == StructKind::STACK) return new StackWrapper<double>(cont, "double");
            if (sk == StructKind::QUEUE) return new QueueWrapper<double>(cont, "double");
            return new DequeWrapper<double>(cont, "double");
        }
        case 3: { // string
            if (sk == StructKind::STACK) return new StackWrapper<std::string>(cont, "string");
            if (sk == StructKind::QUEUE) return new QueueWrapper<std::string>(cont, "string");
            return new DequeWrapper<std::string>(cont, "string");
        }
        case 4: { // user (Student)
            if (sk == StructKind::STACK) return new StackWrapper<Student>(cont, "student");
            if (sk == StructKind::QUEUE) return new QueueWrapper<Student>(cont, "student");
            return new DequeWrapper<Student>(cont, "student");
        }
        case 5: { // user (Teacher)
            if (sk == StructKind::STACK) return new StackWrapper<Teacher>(cont, "teacher");
            if (sk == StructKind::QUEUE) return new QueueWrapper<Teacher>(cont, "teacher");
            return new DequeWrapper<Teacher>(cont, "teacher");
        }
        default: throw Errors::InvalidArgument();
    }
}


void RunApplication() {
    std::vector<IWrapper*> structs;
    while (true) {
        std::cout << "\n==== Main menu ====\n";
        std::cout << "1. List structures\n2. Add structure\n3. Work with structure\n4. Remove structure\n5. Exit\nChoose: ";
        try {
            int choice = GetInt();
            switch (choice) {
                case 1: { // list
                    if (structs.empty()) { std::cout << "No structures yet.\n"; break; }
                    for (size_t i = 0; i < structs.size(); ++i) {
                        std::cout << i << ": " << structs[i]->Info() << "\n";
                        structs[i]->ShowElements();
                    }
                    break;
                }
                case 2: { // add
                    structs.push_back(CreateWrapper());
                    std::cout << "Structure added as index " << structs.size() - 1 << "\n";
                    break;
                }
                case 3: { // work
                    if (structs.empty()) { std::cout << "No structures yet.\n"; break; }
                    std::cout << "Available indices: 0 to " << structs.size() - 1 << "\n";
                    int idx = GetInt("Index: ");
                    if (idx < 0 || static_cast<size_t>(idx) >= structs.size()) throw Errors::IndexOutOfRange();
                    structs[idx]->Menu();
                    break;
                }
                case 4: { // remove
                    if (structs.empty()) { std::cout << "Nothing to remove.\n"; break; }
                    std::cout << "Available indices: 0 to " << structs.size() - 1 << "\n";
                    int idx = GetInt("Index to remove: ");
                    if (idx < 0 || static_cast<size_t>(idx) >= structs.size()) throw Errors::IndexOutOfRange();
                    delete structs[idx];
                    structs.erase(structs.begin() + idx);
                    std::cout << "Removed.\n";
                    break;
                }
                case 5: { // exit
                    for (auto* p : structs) delete p;
                    std::cout << "Goodbye!\n";
                    return;
                }
                default: std::cout << "Invalid choice.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

int main() {
    RunApplication();
    return 0;
}
