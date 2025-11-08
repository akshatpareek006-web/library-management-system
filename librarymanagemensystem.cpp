#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <exception>
using namespace std;

// ---------- Base Class ----------
class Person {
protected:
    string name; int id;
public:
    Person(string n="", int i=0): name(n), id(i) {}
    virtual void showInfo() const = 0;
    virtual ~Person() {}
};

// ---------- Derived Classes ----------
class Member : public Person {
    static int count;
    int borrowed;
public:
    Member(string n="", int i=0): Person(n,i), borrowed(0) { count++; }
    void borrowBook() { borrowed++; }
    void showInfo() const override {
        cout << "Member: " << name << " | ID: " << id 
             << " | Borrowed: " << borrowed << endl;
    }
    static int getCount() { return count; }
    int getId() const { return id; }
};
int Member::count = 0;

class Librarian : public Person {
public:
    Librarian(string n="", int i=0): Person(n,i) {}
    void showInfo() const override {
        cout << "Librarian: " << name << " | ID: " << id << endl;
    }
};

// ---------- Book Class ----------
class Book {
    string title, author; int id; bool issued;
public:
    Book(string t="", string a="", int i=0): title(t), author(a), id(i), issued(false) {}
    void issue() { issued = true; }
    void showInfo() const {
        cout << "ID: " << id << " | " << title << " | " << author 
             << " | " << (issued ? "Issued" : "Available") << endl;
    }
    bool operator==(const Book& b) const { return id == b.id; }
    int getId() const { return id; }
    bool isIssued() const { return issued; }
    friend ostream& operator<<(ostream& os, const Book& b) {
        os << b.id << " " << b.title << " " << b.author << " "
           << (b.issued ? "Issued" : "Available");
        return os;
    }
};

// ---------- Template Function ----------
template <class T>
int searchItem(const vector<T>& v, T item) {
    for (size_t i = 0; i < v.size(); i++)
        if (v[i] == item) return i;
    return -1;
}

// ---------- Library Class ----------
class Library {
    vector<Book> books;
    vector<Member> members;
public:
    void addBook(Book b) { books.push_back(b); }
    void addMember(Member m) { members.push_back(m); }
    void showBooks() const { for (auto &b : books) b.showInfo(); }
    void showMembers() const { for (auto &m : members) m.showInfo(); }

    void issueBook(int bid, int mid) {
        try {
            int bi=-1, mi=-1;
            for(size_t i=0;i<books.size();i++)
                if(books[i].getId()==bid && !books[i].isIssued()) bi=i;
            for(size_t j=0;j<members.size();j++)
                if(members[j].getId()==mid) mi=j;
            if(bi==-1 || mi==-1) throw runtime_error("Book/Member not found!");
            books[bi].issue(); members[mi].borrowBook();
            cout << "Book issued successfully!\n";
        } catch(exception &e){ cout << "Error: " << e.what() << endl; }
    }

    void save(string file){
        ofstream f(file);
        for(auto &b: books) f << b << endl;
        f.close();
    }
    const vector<Book>& getBooks() const { return books; }
};

// ---------- Main ----------
int main() {
    Library L;
    L.addBook({"Engineering Mathematics Genius", "B.S. Grewal", 1});
    L.addBook({"Object Oriented Programming", "E. Balagurusamy", 2});
    L.addMember({"Akshat", 101});
    L.addMember({"Raghav", 102});

    cout << "\nBooks:\n";   L.showBooks();
    cout << "\nMembers:\n"; L.showMembers();

    Book findB("Object Oriented Programming", "E. Balagurusamy", 2);
    cout << "\nBook found at index: " << searchItem(L.getBooks(), findB) << endl;

    L.issueBook(2,101);
    L.save("books.txt");

    cout << "\nTotal Members: " << Member::getCount() << endl;
}
