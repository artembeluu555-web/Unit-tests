#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>

using namespace std;

class Author {
    string name;
public:
    explicit Author(string n) : name(move(n)) {}
    string getName() const { return name; }
};

class Book {
protected:
    int id;
    string title;
    Author author;
    int year;
    bool available;
    string genre;
public:
    Book(int i, string t, Author a, int y, string g)
        : id(i), title(move(t)), author(move(a)), year(y), available(true), genre(move(g)) {}
    virtual ~Book() = default;

    virtual void printInfo() const = 0;
    virtual unique_ptr<Book> clone() const = 0;

    bool borrow() { if (!available) return false; available = false; return true; }
    void returnBook() { available = true; }
    string getTitle() const { return title; }
};

class PrintedBook : public Book {
    int pages;
public:
    PrintedBook(int i, string t, Author a, int y, string g, int p)
        : Book(i, move(t), move(a), y, move(g)), pages(p) {}
    void printInfo() const override {
        cout << "[Printed] " << title << " (" << year << "), " << author.getName()
             << ", " << pages << " pages, " << (available ? "available" : "borrowed") << "\n";
    }
    unique_ptr<Book> clone() const override { return make_unique<PrintedBook>(*this); }
};

class EBook : public Book {
    double sizeMB;
public:
    EBook(int i, string t, Author a, int y, string g, double s)
        : Book(i, move(t), move(a), y, move(g)), sizeMB(s) {}
    void printInfo() const override {
        cout << "[EBook] " << title << " (" << year << "), " << author.getName()
             << ", " << fixed << setprecision(1) << sizeMB << " MB, " << (available ? "available" : "borrowed") << "\n";
    }
    unique_ptr<Book> clone() const override { return make_unique<EBook>(*this); }
};

class AudioBook : public Book {
    double duration;
public:
    AudioBook(int i, string t, Author a, int y, string g, double d)
        : Book(i, move(t), move(a), y, move(g)), duration(d) {}
    void printInfo() const override {
        cout << "[Audio] " << title << " (" << year << "), " << author.getName()
             << ", " << fixed << setprecision(1) << duration << " hours, " << (available ? "available" : "borrowed") << "\n";
    }
    unique_ptr<Book> clone() const override { return make_unique<AudioBook>(*this); }
};

class Catalog {
    vector<unique_ptr<Book>> books;
public:
    void addBook(const Book& b) { books.push_back(b.clone()); }
    void listAll() const { for (const auto& b : books) b->printInfo(); }


    template<typename Pred>
    vector<Book*> search(Pred p) {
        vector<Book*> result;
        for (auto& b : books)
            if (p(*b)) result.push_back(b.get());
        return result;
    }
};

class User {
protected:
    string name;
    int borrowed;
public:
    User(string n) : name(move(n)), borrowed(0) {}
    virtual void showRole() const = 0;
    virtual bool canBorrow() const = 0;
    void borrowBook() { borrowed++; }
    void returnBook() { if (borrowed>0) borrowed--; }
    string getName() const { return name; }
};

class Student : public User {
    string faculty;
    int yearStudy;
public:
    Student(string n, string f, int y) : User(move(n)), faculty(move(f)), yearStudy(y) {}
    void showRole() const override {
        cout << name << " - Student, " << faculty << ", year " << yearStudy << "\n";
    }
    bool canBorrow() const override { return borrowed < 5; }
};

class Librarian : public User {
    string employeeId;
public:
    Librarian(string n, string id) : User(move(n)), employeeId(move(id)) {}
    void showRole() const override {
        cout << name << " - Librarian, ID: " << employeeId << "\n";
    }
    bool canBorrow() const override { return true; }
};

class Library {
    Catalog catalog;
    vector<unique_ptr<User>> users;
    int nextBookId = 1;
public:
    Catalog& getCatalog() { return catalog; }

    int newBookId() { return nextBookId++; }

    Student* addStudent(string n, string f, int y) {
        auto u = make_unique<Student>(move(n), move(f), y);
        Student* ptr = u.get();
        users.push_back(move(u));
        return ptr;
    }

    Librarian* addLibrarian(string n, string id) {
        auto u = make_unique<Librarian>(move(n), move(id));
        Librarian* ptr = u.get();
        users.push_back(move(u));
        return ptr;
    }
};