#include <gtest/gtest.h>
#include "library.h"
#include <stdexcept>
#include <chrono>
#include <string>
using namespace std;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


TEST(BookTest, BorrowAndReturn) {
    PrintedBook b(1, "Test", Author("Author"), 2020, "Drama", 300);
    EXPECT_TRUE(b.borrow());
    EXPECT_FALSE(b.borrow());
    b.returnBook();
    EXPECT_TRUE(b.borrow());
}

TEST(BookTest, CloneIndependence) {
    EBook b1(2, "Digital", Author("Alice"), 2021, "IT", 5.2);
    auto clone = b1.clone();
    EXPECT_NE(&b1, clone.get());
    EXPECT_EQ(b1.getTitle(), clone->getTitle());
    EXPECT_TRUE(clone->borrow());
    EXPECT_FALSE(clone->borrow());
}

TEST(CatalogTest, AddPrintedBookToCatalog) {
    Catalog c;
    PrintedBook b(3, "BookP", Author("P"), 2020, "Art", 250);
    c.addBook(b);
    auto result = c.search([](Book& bk){ return bk.getTitle() == "BookP"; });
    EXPECT_EQ(result.size(), 1);
}

TEST(CatalogTest, AddEBookToCatalog) {
    Catalog c;
    EBook b(4, "BookE", Author("E"), 2021, "Poetry", 2.1);
    c.addBook(b);
    auto result = c.search([](Book& bk){ return bk.getTitle() == "BookE"; });
    EXPECT_EQ(result.size(), 1);
}

TEST(CatalogTest, AddAudioBookToCatalog) {
    Catalog c;
    AudioBook b(5, "BookA", Author("A"), 2019, "Novel", 4.5);
    c.addBook(b);
    auto result = c.search([](Book& bk){ return bk.getTitle() == "BookA"; });
    EXPECT_EQ(result.size(), 1);
}

TEST(CatalogTest, SearchTitleLambda) {
    Catalog c;
    PrintedBook b(6, "C++ Advanced", Author("John"), 2020, "Tech", 400);
    c.addBook(b);
    auto found = c.search([](Book& bk){ return bk.getTitle().find("C++") != string::npos; });
    EXPECT_EQ(found.size(), 1);
}

TEST(CatalogTest, SearchByYear) {
    Catalog c;
    c.addBook(PrintedBook(7, "Math", Author("A"), 2020, "Science", 100));
    c.addBook(PrintedBook(8, "Physics", Author("B"), 2023, "Science", 200));
    auto found = c.search([](Book& bk){ return bk.getTitle() == "Physics"; });
    EXPECT_EQ(found.size(), 1);
}

TEST(LibraryTest, BookIdIncrements) {
    Library lib;
    int id1 = lib.newBookId();
    int id2 = lib.newBookId();
    EXPECT_EQ(id2, id1 + 1);
}

TEST(LibraryTest, AddStudent) {
    Library lib;
    auto s = lib.addStudent("Alex", "CS", 2);
    EXPECT_EQ(s->getName(), "Alex");
}

TEST(LibraryTest, AddLibrarian) {
    Library lib;
    auto l = lib.addLibrarian("Mary", "ID01");
    EXPECT_EQ(l->getName(), "Mary");
}

TEST(StudentTest, BorrowLimit) {
    Student s("Bob", "Math", 3);
    for (int i = 0; i < 5; i++) s.borrowBook();
    EXPECT_FALSE(s.canBorrow());
}

TEST(StudentTest, ReturnBook) {
    Student s("Jane", "Law", 2);
    s.borrowBook();
    s.returnBook();
    EXPECT_TRUE(s.canBorrow());
}

TEST(UserTest, ShowRoleOutputs) {
    testing::internal::CaptureStdout();
    Student s("Anna", "Design", 1);
    s.showRole();
    string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("Student"), string::npos);
}

TEST(BookTest, MixedBookTypesPrint) {
    testing::internal::CaptureStdout();
    PrintedBook p(9, "Printed", Author("P"), 2022, "Fiction", 200);
    EBook e(10, "Ebook", Author("E"), 2023, "Drama", 1.2);
    AudioBook a(11, "Audio", Author("A"), 2020, "FairyTale", 3.5);
    p.printInfo(); e.printInfo(); a.printInfo();
    string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("[Printed]"), string::npos);
    EXPECT_NE(out.find("[EBook]"), string::npos);
    EXPECT_NE(out.find("[Audio]"), string::npos);
}



TEST(BookTest, InvalidBookId) {
    EXPECT_THROW(PrintedBook(0, "Invalid", Author("Anon"), 2020, "Drama", 100), std::invalid_argument);
}

TEST(CatalogTest, EmptyCatalogSearch) {
    Catalog c;
    auto result = c.search([](Book& bk){ return bk.getTitle() == "None"; });
    EXPECT_EQ(result.size(), 0);
}

TEST(LibraryTest, BorrowNonExistingBook) {
    Library lib;
    EXPECT_THROW(lib.borrowBook(999), std::out_of_range);
}

TEST(BookTest, BorrowAndReturnCombined) {
    PrintedBook b(12, "Combined", Author("A"), 2022, "Novel", 150);
    EXPECT_TRUE(b.borrow());
    EXPECT_FALSE(b.borrow());
    b.returnBook();
    EXPECT_TRUE(b.borrow());
}

TEST(CatalogPerformance, SearchLargeCatalog) {
    Catalog c;
    for (int i = 0; i < 50000; ++i)
        c.addBook(PrintedBook(1000 + i, "Book" + to_string(i), Author("A"), 2000 + (i % 20), "Fiction", 100));

    auto start = chrono::high_resolution_clock::now();
    auto result = c.search([](Book& bk){ return bk.getTitle() == "Book49999"; });
    auto end = chrono::high_resolution_clock::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    EXPECT_EQ(result.size(), 1);
    EXPECT_LT(ms, 100); // має шукати швидко
}

TEST(LibraryTest, ComplexBorrowAndReturnScenario) {
    Library lib;
    auto s = lib.addStudent("Max", "History", 2);
    auto l = lib.addLibrarian("Olga", "LB01");

    auto id1 = lib.newBookId();
    auto id2 = lib.newBookId();
    lib.addBook(PrintedBook(id1, "War and Peace", Author("Tolstoy"), 1869, "Classic", 1200));
    lib.addBook(EBook(id2, "Algorithms", Author("Knuth"), 1997, "Tech", 3.4));

    EXPECT_NO_THROW(lib.borrowBook(id1));
    EXPECT_NO_THROW(lib.borrowBook(id2));
    EXPECT_THROW(lib.borrowBook(999), std::out_of_range);

    EXPECT_TRUE(lib.returnBook(id1));
    EXPECT_TRUE(lib.returnBook(id2));
}
