#include <iostream>
#include <string>
using namespace std;

// User-defined class Book
class Book {
private:
    string title;
    string author;
public:
    // Constructor to initialize Book data
    Book(string t = "", string a = "") : title(t), author(a) {}

    // Friend function to overload the input operator (>>)
    friend istream& operator>>(istream& in, Book& b);

    // Friend function to overload the output operator (<<)
    friend ostream& operator<<(ostream& out, const Book& b);

    // Function to display Book details
    void display() const {
        cout << "Title: " << title << ", Author: " << author << endl;
    }
};

// Overload the input operator (>>)
istream& operator>>(istream& in, Book& b) {
    cout << "Enter book title: ";
    getline(in, b.title);  // Read full line for title
    cout << "Enter book author: ";
    getline(in, b.author);  // Read full line for author
    return in;
}

// Overload the output operator (<<)
ostream& operator<<(ostream& out, const Book& b) {
    out << "Title: " << b.title << ", Author: " << b.author;
    return out;
}

// Template class to store any data type
template <typename T>
class Storage {  // class template -> Independent elm
private:
    T data;  // This can store any type
public:
    // Constructor to initialize data
    Storage(T d) : data(d) {}

    // Function to display the stored data
    void display() const {
        cout << data << endl;  // Call overloaded << operator for Book object
    }
};

int main() {
    Book book1;
    // Using overloaded input operator to enter book details
    cin >> book1;

    // Creating an object of Storage class to store the Book object
    Storage<Book> storage(book1); // class instantiation -> dependent elm

    // Displaying stored Book details using overloaded << operator
    storage.display();

    return 0;
}