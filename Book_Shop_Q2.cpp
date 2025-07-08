#include <bits/stdc++.h>
using namespace std;



//////////////////////////////////////////
//Book
class Book {
protected:
    string isbn, title;
    int year;
    double price;

public:
    Book(string isbn, string title, int year, double price)
        : isbn(isbn), title(title), year(year), price(price) {}

    virtual ~Book() = default;

    string getISBN() const { return isbn; }
    int getYear() const { return year; }
    virtual bool isForSale() const = 0;
    virtual void buy(int quantity, const string& email, const string& address) = 0;
    virtual void display() const = 0;
};


//////////////////////////////////////////
// PaperBook
class PaperBook : public Book {
    int stock;

public:
    PaperBook(string isbn, string title, int year, double price, int stock)
        : Book(isbn, title, year, price), stock(stock) {}

    bool isForSale() const override { return true; }

    void buy(int quantity, const string& email, const string& address) override {
        if (stock < quantity)
            throw runtime_error("Not enough stock.");
        stock -= quantity;
        cout << " Sending paper book to address: " << address << endl;
    }

    void display() const override {
        cout << " PaperBook: " << title << " | Stock: " << stock << " | Price: " << price << endl;
    }
};



//////////////////////////////////////////
// EBook
class EBook : public Book {
    string filetype;

public:
    EBook(string isbn, string title, int year, double price, string filetype)
        : Book(isbn, title, year, price), filetype(filetype) {}

    bool isForSale() const override { return true; }

    void buy(int quantity, const string& email, const string& address) override {
        if (quantity > 1)
            throw runtime_error("EBook can only be bought in 1 copy.");
        cout << " Sending ebook to email: " << email << endl;
    }

    void display() const override {
        cout << " EBook: " << title << " | Filetype: " << filetype << " | Price: " << price << endl;
    }
};




//////////////////////////////////////////
//  ShowcaseBook
class ShowcaseBook : public Book {
public:
    ShowcaseBook(string isbn, string title, int year)
        : Book(isbn, title, year, 0.0) {}

    bool isForSale() const override { return false; }

    void buy(int, const string&, const string&) override {
        throw runtime_error("Showcase book is not for sale.");
    }

    void display() const override {
        cout << " ShowcaseBook: " << title << " [Not for sale]" << endl;
    }
};




//////////////////////////////////////////
// class Inventory
class Inventory {
    unordered_map<string, shared_ptr<Book>> books;

public:
    void addBook(shared_ptr<Book> book) {
        books[book->getISBN()] = book;
    }

    void removeOutdatedBooks(int maxAge) {
        time_t now = time(nullptr);
        int currentYear = 1900 + localtime(&now)->tm_year;

        for (auto it = books.begin(); it != books.end(); ) {
            if (currentYear - it->second->getYear() > maxAge)
                it = books.erase(it);
            else
                ++it;
        }
    }

    void buyBook(const string& isbn, int quantity, const string& email, const string& address) {
        if (!books.count(isbn)) {
            cout << " Book not found.\n";
            return;
        }

        auto book = books[isbn];
        if (!book->isForSale()) {
            cout << " Book not for sale.\n";
            return;
        }

        try {
            book->buy(quantity, email, address);
        } catch (const exception& ex) {
            cout << " Purchase failed: " << ex.what() << endl;
        }
    }

    void listBooks() const {
        cout << "\n Book Inventory:\n";
        for (const auto& [isbn, book] : books) {
            book->display();
        }
    }
};


int main() {
    Inventory store;


    store.addBook(make_shared<PaperBook>("111", "YouTobya", 2018, 150.0, 10));
    store.addBook(make_shared<EBook>("222", "Clean Code", 2022, 100.0, "PDF"));
    store.addBook(make_shared<ShowcaseBook>("333", "Old Book", 1920));

    store.listBooks();

    cout << "\n Buying Paper Book (2 copies):\n";
    store.buyBook("111", 2, "karam@gmail.com", "Mansoura");

    cout << "\n Buying EBook (1 copy):\n";
    store.buyBook("222", 1, "karam@gmail.com", "Mansoura");

    cout << "\n Trying to Buy Showcase Book:\n";
    store.buyBook("333", 1, "karam@gmail.com", "Mansoura");

    cout << "\n Removing outdated books (> 50 years):\n";
    store.removeOutdatedBooks(50);

    store.listBooks();

    return 0;
}