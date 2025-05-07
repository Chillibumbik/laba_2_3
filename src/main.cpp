
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "user.hpp"
#include "errors.hpp"

void ClearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int GetIntInput(const std::string& prompt = "") {
    int value;
    if (!prompt.empty()) std::cout << prompt;
    if (!(std::cin >> value)) {
        ClearInput();
        throw Errors::InvalidArgument();
    }
    return value;
}

template<typename T>
T GetTypedInput(const std::string& prompt = "Enter value: ") {
    T value;
    std::cout << prompt;
    if (!(std::cin >> value)) {
        ClearInput();
        throw Errors::InvalidArgument();
    }
    return value;
}

struct ISequenceWrapper {
    virtual ~ISequenceWrapper() = default;
    virtual void Show() const = 0;
    virtual void Append() = 0;
    virtual void Prepend() = 0;
    virtual void InsertAt() = 0;
    virtual void RemoveAt() = 0;
    virtual void GetAt() const = 0;
    virtual ISequenceWrapper* GetSubsequence() const = 0;
    virtual ISequenceWrapper* Concat(const ISequenceWrapper* other) const = 0;
    virtual const std::string& TypeKey() const = 0;
};

template<typename T>
struct SequenceWrapper : public ISequenceWrapper {
    Sequence<T>* seq;
    std::string structure;
    std::string type_key;

    SequenceWrapper(const std::string& structure_, const std::string& key) : structure(structure_), type_key(key) {
        if (structure == "array") seq = new MutableArraySequence<T>();
        else seq = new MutableListSequence<T>();
    }

    ~SequenceWrapper() override {
        delete seq;
    }

    void Show() const override {
        std::cout << "[ ";
        for (int i = 0; i < seq->GetLength(); ++i)
            std::cout << seq->Get(i) << " ";
        std::cout << "] (Type: " << type_key << ", Structure: " << structure << ")\n";
    }

    void Append() override {
        T val = GetTypedInput<T>("Enter value to append: ");
        seq->Append(val);
    }

    void Prepend() override {
        T val = GetTypedInput<T>("Enter value to prepend: ");
        seq->Prepend(val);
    }

    void InsertAt() override {
        std::cout << "Valid position: 0 to " << seq->GetLength() << "\n";
        int index = GetIntInput("Enter position: ");
        if (index < 0 || index > seq->GetLength()) throw Errors::IndexOutOfRange();
        T val = GetTypedInput<T>();
        seq->InsertAt(val, index);
    }

    void RemoveAt() override {
        std::cout << "Valid index: 0 to " << seq->GetLength() - 1 << "\n";
        int index = GetIntInput("Enter index to remove: ");
        if (index < 0 || index >= seq->GetLength()) throw Errors::IndexOutOfRange();
        seq->Remove(index);
    }

    void GetAt() const override {
        std::cout << "Valid index: 0 to " << seq->GetLength() - 1 << "\n";
        int index = GetIntInput("Enter index to get: ");
        if (index < 0 || index >= seq->GetLength()) throw Errors::IndexOutOfRange();
        std::cout << "Element: " << seq->Get(index) << "\n";
    }

    ISequenceWrapper* GetSubsequence() const override {
        std::cout << "Valid range: 0 to " << seq->GetLength() - 1 << "\n";
        int start = GetIntInput("Enter start: ");
        int end = GetIntInput("Enter end: ");
        if (start < 0 || end >= seq->GetLength() || start > end) throw Errors::IndexOutOfRange();
        Sequence<T>* sub = seq->GetSubsequence(start, end);
        auto* result = new SequenceWrapper<T>(structure, type_key);
        delete result->seq;
        result->seq = sub;
        return result;
    }

    ISequenceWrapper* Concat(const ISequenceWrapper* other) const override {
        auto other_casted = dynamic_cast<const SequenceWrapper<T>*>(other);
        if (!other_casted) throw Errors::ConcatTypeMismatchError();
    
        Sequence<T>* new_seq;
        if (structure == "array") {
            new_seq = new MutableArraySequence<T>();
        } else if (structure == "list") {
            new_seq = new MutableListSequence<T>();
        } else {
            throw Errors::InvalidArgument();
        }
    
        for (int i = 0; i < seq->GetLength(); ++i)
            new_seq->Append(seq->Get(i));
        for (int i = 0; i < other_casted->seq->GetLength(); ++i)
            new_seq->Append(other_casted->seq->Get(i));
    
        auto* result = new SequenceWrapper<T>(structure, type_key);
        delete result->seq;
        result->seq = new_seq;
        return result;
    }
    

    const std::string& TypeKey() const override {
        return type_key;
    }
};

void ShowTypeMenu() {
    std::cout << "Select type:\n1. int\n2. double\n3. string\n4. user\n";
}
void ShowStructureMenu() {
    std::cout << "Select structure:\n1. array\n2. list\n";
}

int interface(std::vector<ISequenceWrapper*>& sequences) {
    while (true) {
        std::cout << "\n1. Show\n2. Append\n3. Prepend\n4. Remove element\n5. Insert element at index\n";
        std::cout << "6. Get element by index\n7. Get subsequence\n8. Concat sequences\n";
        std::cout << "9. Add sequence\n10. Remove sequence\n11. Exit\nChoose action: ";

        try {
            int choice = GetIntInput();

            if (choice != 9 && choice != 11 && sequences.empty()) {
                std::cout << "No sequences yet. Add a sequence first.\n";
                continue;
            }

            switch (choice) {
                case 1: // show
                    for (size_t i = 0; i < sequences.size(); ++i) {
                        std::cout << i << ": ";
                        sequences[i]->Show();
                    }
                    break;

                case 2: case 3: case 4: case 5: case 6: case 7: { // inner operation
                    std::cout << "Choose sequence index (from 0 to " << sequences.size() - 1 << "): ";
                    int idx = GetIntInput();
                    if (idx < 0 || static_cast<size_t>(idx) >= sequences.size()) throw Errors::IndexOutOfRange();
                    switch (choice) {
                        case 2: sequences[idx]->Append(); break; // append
                        case 3: sequences[idx]->Prepend(); break; // prepend
                        case 4: sequences[idx]->RemoveAt(); break; // remove
                        case 5: sequences[idx]->InsertAt(); break; // insert
                        case 6: sequences[idx]->GetAt(); break; // get element
                        case 7: { // get subs
                            ISequenceWrapper* result = sequences[idx]->GetSubsequence();
                            sequences.push_back(result);
                            std::cout << "Subsequence added as index " << sequences.size() - 1 << "\n";
                            break;
                        }
                    }
                    break;
                }

                case 8: { // concat sequence
                    std::cout << "First sequence index (0 to " << sequences.size() - 1 << "): ";
                    int i1 = GetIntInput();
                    std::cout << "Second sequence index (0 to " << sequences.size() - 1 << "): ";
                    int i2 = GetIntInput();
                    if (i1 < 0 || i2 < 0 || static_cast<size_t>(i1) >= sequences.size() || static_cast<size_t>(i2) >= sequences.size())
                        throw Errors::IndexOutOfRange();
                    ISequenceWrapper* result = sequences[i1]->Concat(sequences[i2]);
                    sequences.push_back(result);
                    std::cout << "Concatenated sequence added as index " << sequences.size() - 1 << "\n";
                    break;
                }

                case 9: { // add sequence
                    ShowTypeMenu();
                    int t = GetIntInput("Enter type: ");
                    std::string type;
                    switch (t) {
                        case 1: type = "int"; break;
                        case 2: type = "double"; break;
                        case 3: type = "string"; break;
                        case 4: type = "user"; break;
                        default: throw Errors::InvalidArgument();
                    }

                    ShowStructureMenu();
                    int s = GetIntInput("Enter structure: ");
                    std::string structure = (s == 1) ? "array" : (s == 2) ? "list" : throw Errors::InvalidArgument();

                    if (type == "int") sequences.push_back(new SequenceWrapper<int>(structure, type));
                    else if (type == "double") sequences.push_back(new SequenceWrapper<double>(structure, type));
                    else if (type == "string") sequences.push_back(new SequenceWrapper<std::string>(structure, type));
                    else if (type == "user") sequences.push_back(new SequenceWrapper<User>(structure, type));
                    break;
                }

                case 10: { //remove
                    std::cout << "Index to remove (0 to " << sequences.size() - 1 << "): ";
                    int idx = GetIntInput();
                    if (idx < 0 || static_cast<size_t>(idx) >= sequences.size()) throw Errors::IndexOutOfRange();
                    delete sequences[idx];
                    sequences.erase(sequences.begin() + idx);
                    std::cout << "Sequence has been removed";
                    break;
                }

                case 11: //exit
                    std::cout << "Exiting...\n";
                    for (auto* ptr : sequences) delete ptr;
                    return 0;

                default:
                    std::cout << "Invalid action.\n";
            }

        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

int main() {
    std::vector<ISequenceWrapper*> sequences;
    interface(sequences);
}


//main пустой ---ВЫПОЛНЕНО---
//не везде const в аргументах(напимер concat) ---ВЫПОЛНЕНО---
//ошибки - так не надо(сделать как у володи(в телефоне лежит)) - Не могу, ибо ловлю ошибки
//concat работает за линию, а можно за константу (очень просто) ---ВЫПОЛНЕНО---
//в dynamicArray->remove - сдвиги лишние (можно перенести сразу) ---ВЫПОЛНЕНО---
//сделайте перегрузку для concat (оператор +) ---ВЫПОЛНЕНО---
//arraySequence - где capacity? ---ВЫПОЛНЕНО---
//зачем явно вызывать delete? --- ВЫПОЛНЕНО---
//тесты (существуют (сарказм)) - сделать нормальные  ---ВЫПОЛНЕНО---
//везде сначала объявление, а потом реализация(для последовательностей нарушил)  ---ВЫПОЛНЕНО---
//хочу как в первой лабе список последовательностей, чтобы можно было разные последовательности выбирать ---ВЫПОЛНЕНО---
//добавить пользовательский тип - какую-нибудь структуру (например, user) ---ВЫПОЛНЕНО---
//когда у меня спрашивают индекс - пусть скажут, в каких пределах он может изменяться ---ВЫПОЛНЕНО---
//подпоследовательность и сконкатинированна пос-ть сохраняются в новую пос-ть ---ВЫПОЛНЕНО---

//shared / unique указатели до слнд еместра не использовать ---ВЫПОЛНЕНО
//тесты: сравнивать массивы и списки целиком, а не посимвольно (с ожидаемым массивом) ---ВЫПОЛНЕНО---
//перегрузка для сравнения (==)  ---ВЫПОЛНЕНО--- 
//ошибки макс скинет ---ВЫПОЛЕНО---
//show также выводит тип пос-ти ---ВЫПОЛНЕНО---
//не везде подсказывает доступные индексы (например, subsequence) ---ВЫПОЛНЕНО---
//subsequence - не добавляет в пос-ти результат ---ВЫПОЛНЕНО---
    //concat должен позволять добавлять к array list и наоборот. тип результата == типу первой пос-и ---ВЫРОЛНЕНО---

