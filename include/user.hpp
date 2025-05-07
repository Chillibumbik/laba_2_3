#pragma once
#include <iostream>
#include <string>

struct User {
    std::string name;
    int age;
    int id;

    User() = default;
    User(std::string name_, int age_) : name(std::move(name_)), age(age_) {}

    bool operator==(const User& other) const {
        return name == other.name && age == other.age && id == other.id;
    }

    friend std::ostream& operator<<(std::ostream& os, const User& user) {
        return os << "User(name: " << user.name << ", age: " << user.age << ", id: " << user.id << ")";
    }

    friend std::istream& operator>>(std::istream& is, User& user) {
        std::cout << "Enter name: ";
        is >> user.name;

        std::cout << "Enter age: ";
        is >> user.age;

        std::cout << "Enter id: ";
        is >> user.id;

        while (user.age < 0 || user.age > 150) {
            std::cout << "Age cannot be negative or more than 150. Try again: ";
            is >> user.age;
        }

        while (user.id < 0) {
            std::cout << "ID cannot be negative. Try again: ";
            is >> user.id;
        }

        return is;
    }
};
