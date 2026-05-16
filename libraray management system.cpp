#include <iostream>
using namespace std;

class Book {
private:
    string title;
    string author;
    int bookID;
    double price;

public:
    //Default Constructor: Initializes book with default values.
    Book() {
        title = "Unknown";
        author = "Unknown";
        bookID = 0;
        price = 0.0;
        cout << "Default constructor called: A new book with default details is created.\n";
    }

    //Parameterized Constructor: Allows custom book details.
    Book(string t, string a, int id, double p) {
        title = t;
        author = a;
        bookID = id;
        price = p;
        cout << "Parameterized constructor called: '" << title << "' by " << author << " is added.\n";
    }

    // Copy Constructor: Creates a duplicate book.
    Book(const Book& b) {
        title = b.title;
        author = b.author;
        bookID = b.bookID;
        price = b.price;
        cout << " Copy constructor called: '" << title << "' has been copied.\n";
    }

    // Overloaded Constructor: Accepts only title and author, sets default values for others.
    Book(string t, string a) {
        title = t;
        author = a;
        bookID = -1;
        price = 0.0;
        cout << " Overloaded constructor called: '" << title << "' is added with default values.\n";
    }


    //  Setters 
    void setTitle(string t) { title = t; }
    void setAuthor(string a) { author = a; }
    void setBookID(int id) { bookID = id; }
    void setPrice(double p) { price = p; }

    //  Getters 
    string getTitle() { return title; }
    string getAuthor() { return author; }
    int getBookID() { return bookID; }
    double getPrice() { return price; }

    // Function to display book details
    void displayBook() {
        cout << "\n --- Book Details ---\n";
        cout << " Title: " << title << endl;
        cout << " Author: " << author << endl;
        if (bookID == -1)
            cout << " Book ID: -1" << endl;
        else
            cout << " Book ID: " << bookID << endl;

        cout << " Price: $" << price << endl;
        cout << "----------------------\n";
    }
};

int main() {
    cout << "Welcome to the Library Management System!\n\n";

    // 1 Creating a book using the Default Constructor
    Book book1;
    book1.displayBook();

    // 2 Creating a book using the Parameterized Constructor
    Book book2("C++ Programming", "Scott", 101, 29.99);
    book2.displayBook();

    // 3️ Creating a book using the Overloaded Constructor (Title + Author only)
    Book book3("Python for Beginners", "Rossum");
    book3.displayBook();

    // 4️ Copying a book using the Copy Constructor
    Book book4 = book2;
    book4.displayBook();

    // 5️ Modifying Book Details using Setters
    cout << "\n Updating book details using setters...\n";
    book1.setTitle("Java Fundamentals");
    book1.setAuthor("Brian");
    book1.setBookID(202);
    book1.setPrice(34.99);
    book1.displayBook();

    return 0;
}