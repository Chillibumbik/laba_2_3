#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "dynamic_array.hpp"
#include "linked_list.hpp"
#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "queue.hpp"
#include "stack.hpp"
#include "deque.hpp"
#include "user.hpp"

/*
Макрос	Что делает
REQUIRE(x)	Проверяет, что x == true. Если нет — стоп.
CHECK(x)	То же, но не останавливает тесты
REQUIRE_THROWS(expr)	Проверяет, что expr выбрасывает исключение
REQUIRE_THROWS_AS(expr, Type)	То же + проверка типа исключения
REQUIRE_FALSE(x)	Проверка, что x == false
*/

TEST_CASE("DynamicArray: Basic Operations", "[DynamicArray]") {
    DynamicArray<int> arr(3);
    arr.Set(0, 10);
    arr.Set(1, 20);
    arr.Set(2, 30);

    DynamicArray<int> expected1(3);
    expected1.Set(0, 10);
    expected1.Set(1, 20);
    expected1.Set(2, 30);
    REQUIRE(arr == expected1);

    arr.Resize(5);
    REQUIRE(arr.GetSize() == 5);
    REQUIRE(arr.GetCapacity() == 5);

    arr.Resize(2);
    DynamicArray<int> expected2(2);
    expected2.Set(0, 10);
    expected2.Set(1, 20);
    REQUIRE(arr == expected2);

    REQUIRE_THROWS_AS(arr.Get(10), std::out_of_range);
    REQUIRE_THROWS_AS(arr.Set(5, 100), std::out_of_range);

    arr.Remove(1);
    REQUIRE(arr.GetSize() == 1);
}

TEST_CASE("LinkedList: Basic Operations", "[LinkedList]") {
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);
    list.Prepend(0);

    REQUIRE(list.GetLength() == 3);
    REQUIRE(list.GetFirst() == 0);
    REQUIRE(list.GetLast() == 2);
    REQUIRE(list.Get(1) == 1);

    REQUIRE(list.GetTail() == 2);

    list.InsertAt(5, 1);
    REQUIRE(list.Get(1) == 5);
    REQUIRE(list.Get(2) == 1);

    LinkedList<int>* sub = list.GetSubList(1, 2);
    REQUIRE(sub->GetLength() == 2);
    REQUIRE(sub->Get(0) == 5);
    REQUIRE(sub->Get(1) == 1);

    REQUIRE_THROWS_AS(list.Get(10), std::out_of_range);
    REQUIRE_THROWS_AS(list.GetSubList(0, 10), std::out_of_range);
}

TEST_CASE("ArraySequence: Operations", "[ArraySequence]") {
    MutableArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Prepend(0);

    REQUIRE(seq.GetLength() == 3);
    REQUIRE(seq.Get(0) == 0);
    REQUIRE(seq.Get(2) == 2);

    seq.InsertAt(5, 1);
    REQUIRE(seq.Get(1) == 5);
    REQUIRE(seq.Get(2) == 1);

    auto sub = seq.GetSubsequence(1, 3);
    REQUIRE(sub->GetLength() == 3);
    REQUIRE(sub->Get(0) == 5);
}

TEST_CASE("ListSequence: Operations", "[ListSequence]") {
    MutableListSequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Prepend(0);

    REQUIRE(seq.GetLength() == 3);
    REQUIRE(seq.Get(0) == 0);
    REQUIRE(seq.Get(2) == 2);

    seq.InsertAt(5, 1);
    REQUIRE(seq.Get(1) == 5);
    REQUIRE(seq.Get(2) == 1);

    auto sub = seq.GetSubsequence(1, 3);
    REQUIRE(sub->GetLength() == 3);
    REQUIRE(sub->Get(0) == 5);
}

TEST_CASE("Sequence: Operators", "[Sequence]") {
    MutableListSequence<int> seq1;
    seq1.Append(1);
    seq1.Append(2);
    seq1.Prepend(0);

    MutableListSequence<int> seq2;
    seq2.Append(12);
    seq2.Append(3122);
    seq2.Prepend(7);

    MutableListSequence<int> seq3 = seq1 + seq2;
    REQUIRE(seq3.GetLength() == 6);
    REQUIRE(seq3.Get(0) == 0);

    seq1.Remove(1);
    REQUIRE(seq1.Get(1) == 2);


    MutableArraySequence<int> arr1;
    arr1.Append(1);
    arr1.Append(212);

    MutableArraySequence<int> arr2;
    arr2.Append(8);

    MutableArraySequence<int> arr3 = arr1 + arr2;
    REQUIRE(arr3.GetLength() == 3);
    REQUIRE(arr3.Get(0) == 1);


    ImmutableArraySequence<std::string> imarr1;
    auto* im1_temp = imarr1.Append("fd");
    auto* imarr1_final = im1_temp->Append("qual");
    delete im1_temp;


    ImmutableArraySequence<std::string> imarr2;
    auto* im2_temp = imarr2.Append("daddy");
    auto* imarr2_final = im2_temp->Append("^_^");
    delete im2_temp;
    

    ImmutableArraySequence<std::string> imarr3 = 
    *static_cast<ImmutableArraySequence<std::string>*>(imarr1_final) + 
    *static_cast<ImmutableArraySequence<std::string>*>(imarr2_final);


    REQUIRE(imarr3.GetLength() == 4); 
    REQUIRE(imarr3.GetLast() == "^_^");
    REQUIRE(imarr3.Get(0) == "fd");
    REQUIRE(imarr2_final->GetFirst() == "daddy");

}

TEST_CASE("ArrayQueue: Basic Operations and Map/Reduce", "[ArrayQueue]") {
    ArrayQueue<int> q;
    q.Enqueue(1);
    q.Enqueue(2);
    q.Enqueue(3);
    REQUIRE(q.GetLength() == 3);
    REQUIRE(q.Peek() == 1);
    REQUIRE(q.Dequeue() == 1);
    REQUIRE(q.Get(0) == 2);

    // Map: удвоить все значения
    q.Map([](int& x) { x *= 2; });
    REQUIRE(q.Get(0) == 4);
    REQUIRE(q.Get(1) == 6);

    // Where: оставить только > 4
    q.Where([](int& x) { return x > 4; });
    REQUIRE(q.GetLength() == 1);
    REQUIRE(q.Get(0) == 6);

    // Reduce: сохранить результат отдельно
    auto sum = q.Reduce([](const int& a, const int& b) { return a + b; });
    REQUIRE(sum == 6);
}

TEST_CASE("ListQueue: Basic Operations and Map/Reduce", "[ListQueue]") {
    ListQueue<int> q;
    q.Enqueue(10);
    q.Enqueue(20);
    q.Enqueue(30);
    REQUIRE(q.GetLength() == 3);
    REQUIRE(q.Peek() == 10);
    REQUIRE(q.Dequeue() == 10);
    REQUIRE(q.Get(0) == 20);

    q.Map([](int& x) { x += 1; });
    REQUIRE(q.Get(0) == 21);
    REQUIRE(q.Get(1) == 31);

    q.Where([](int& x) { return x % 2 == 1; });  // оставить нечётные
    REQUIRE(q.GetLength() == 2);

    auto result = q.Reduce([](const int& a, const int& b) { return a * b; });
    REQUIRE(result == 651);
}

TEST_CASE("ArrayStack: Basic Operations", "[ArrayStack]") {
    ArrayStack<std::string> st;
    st.Push("hello");
    st.Push("world");
    REQUIRE(st.Top() == "world");
    REQUIRE(st.Pop() == "world");
    REQUIRE(st.Top() == "hello");
    REQUIRE(st.GetLength() == 1);
    st.Clear();
    REQUIRE(st.IsEmpty());
}

TEST_CASE("ListStack: Basic Operations", "[ListStack]") {
    ListStack<double> st;
    st.Push(3.14);
    st.Push(2.71);
    REQUIRE(st.Top() == Approx(2.71));
    REQUIRE(st.Pop() == Approx(2.71));
    REQUIRE(st.Get(0) == Approx(3.14));
}

TEST_CASE("ArrayDeque: Basic Operations", "[ArrayDeque]") {
    ArrayDeque<int> d;
    d.PushBack(1);
    d.PushFront(0);
    d.PushBack(2);
    REQUIRE(d.Front() == 0);
    REQUIRE(d.Back() == 2);
    REQUIRE(d.PopFront() == 0);
    REQUIRE(d.PopBack() == 2);
    REQUIRE(d.GetLength() == 1);
}

TEST_CASE("ListDeque: Basic Operations", "[ListDeque]") {
    ListDeque<char> d;
    d.PushBack('a');
    d.PushFront('z');
    d.PushBack('b');
    REQUIRE(d.Front() == 'z');
    REQUIRE(d.Back() == 'b');
    d.PopBack();
    REQUIRE(d.Back() == 'a');
}

TEST_CASE("Student in Stack/Queue/Deque", "[Student][Integration]") {
    Student s1("Alice", 20, 1, "PMI", 4.8);
    Student s2("Bob", 21, 2, "AI", 4.2);
    Student s3("Clara", 19, 3, "Math", 4.5);

    SECTION("Stack<Student>") {
        ArrayStack<Student> st;
        st.Push(s1);
        st.Push(s2);
        REQUIRE(st.Top() == s2);
        REQUIRE(st.Pop() == s2);
        REQUIRE(st.Top() == s1);
    }

    SECTION("Queue<Student>") {
        ListQueue<Student> q;
        q.Enqueue(s1);
        q.Enqueue(s2);
        REQUIRE(q.Peek() == s1);
        REQUIRE(q.Dequeue() == s1);
        REQUIRE(q.Get(0) == s2);
    }

    SECTION("Deque<Student>") {
        ArrayDeque<Student> d;
        d.PushBack(s1);
        d.PushFront(s2);
        d.PushBack(s3);
        REQUIRE(d.Front() == s2);
        REQUIRE(d.Back() == s3);
        REQUIRE(d.Get(1) == s1);
    }

    SECTION("Comparison and Copy") {
        Student copy = s1;
        REQUIRE(copy == s1);
        copy.age = 99;
        REQUIRE_FALSE(copy == s1);
    }
}