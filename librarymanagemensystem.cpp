#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <exception>
using namespace std;

class Person {
protected:
    string name;
    int id;
public:
    Person(string n="", int i=0): name(n), id(i) {}
    virtual void showInfo() const = 0;
    virtual ~Person() {}
};

class Member : public Person {
    static int count;
    int borrowed;
    string department;   
public:
    Member(string n="", string d="", int i=0)
        : Person(n,i), borrowed(0), department(d) 
    { 
        count++; 
    }

    void borrowBook() { borrowed++; }

    void showInfo() const override {
        cout << "Member: " << name
             << " | ID: " << id
             << " | Department: " << department
             << " | Borrowed: " << borrowed << endl;
    }

    static int getCount() { return count; }

    int getId() const { return id; }
    string getName() const { return name; }
    string getDepartment() const { return department; }
};
int Member::count = 0;

class Book {
    string title, author;
    int id;
    bool issued;

    string issueDate;
    string returnDate;

    string issuedToName;
    int issuedToId;
    string issuedToDept;

public:
    Book(string t="", string a="", int i=0)
        : title(t), author(a), id(i), issued(false),
          issueDate("N/A"), returnDate("N/A"),
          issuedToName("N/A"), issuedToId(-1), issuedToDept("N/A") {}

    void issue(string idate, string rdate, string name, int mid, string dept) {
        issued = true;
        issueDate = idate;
        returnDate = rdate;
        issuedToName = name;
        issuedToId = mid;
        issuedToDept = dept;
    }

    void showInfo() const {
        cout << "Book ID: " << id
             << " | Title: " << title
             << " | Author: " << author
             << " | Status: " << (issued ? "Issued" : "Available");

        if (issued) {
            cout << " | Issue Date: " << issueDate
                 << " | Return Date: " << returnDate
                 << " | Issued To: " << issuedToName
                 << " (ID: " << issuedToId << ")"
                 << " | Department: " << issuedToDept;
        }
        cout << endl;
    }

    int getId() const { return id; }
    bool isIssued() const { return issued; }
};

class Library {
    vector<Book> books;
    vector<Member> members;

public:
    void addBook(Book b) { books.push_back(b); }
    void addMember(Member m) { members.push_back(m); }

    void showBooks() const {
        for (auto &b : books) b.showInfo();
    }
    void showMembers() const {
        for (auto &m : members) m.showInfo();
    }

    void issueBook(int bid, int mid, string issueD, string returnD) {
        try {
            int bi=-1, mi=-1;

            for(size_t i=0; i<books.size(); i++)
                if(books[i].getId()==bid && !books[i].isIssued())
                    bi=i;

            for(size_t j=0; j<members.size(); j++)
                if(members[j].getId()==mid)
                    mi=j;

            if(bi==-1 || mi==-1)
                throw runtime_error("Book/Member not found!");

            books[bi].issue(
                issueD, returnD,
                members[mi].getName(),
                mid,
                members[mi].getDepartment()
            );

            members[mi].borrowBook();

            cout << "\n📚 Book Issued Successfully!\n";
            cout << "Issued To: " << members[mi].getName() 
                 << " (ID: " << mid << ")\n";
            cout << "Department: " << members[mi].getDepartment() << endl;
            cout << "Issue Date: " << issueD << endl;
            cout << "Return Date: " << returnD << endl;

        } catch(exception &e) {
            cout << "Error: " << e.what() << endl;
        }
    }
};

int main() {
    Library L;

    int nBooks;
    cout << "Enter number of books: ";
    cin >> nBooks;

    for(int i=0; i<nBooks; i++) {
        string title, author;
        int id;
        cout << "Enter Book Title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter Book ID: ";
        cin >> id;

        L.addBook(Book(title, author, id));
        cout << "Book added!\n\n";
    }

    int nMembers;
    cout << "Enter number of members: ";
    cin >> nMembers;

    for(int i=0; i<nMembers; i++) {
        string name, dept;
        int id;
        cout << "Enter Member Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Department: ";
        getline(cin, dept);
        cout << "Enter Member ID: ";
        cin >> id;

        L.addMember(Member(name, dept, id));
        cout << "Member added!\n\n";
    }

    cout << "\nBooks:\n";
    L.showBooks();

    cout << "\nMembers:\n";
    L.showMembers();

    int bid, mid;
    string issueDate, returnDate;

    cout << "\nEnter Book ID to issue: ";
    cin >> bid;
    cout << "Enter Member ID: ";
    cin >> mid;
    cout << "Enter Issue Date (DD/MM/YYYY): ";
    cin >> issueDate;
    cout << "Enter Return Date (DD/MM/YYYY): ";
    cin >> returnDate;

    L.issueBook(bid, mid, issueDate, returnDate);

    cout << "\n📘 Updated Book List:\n";
    L.showBooks();

    return 0;
}

