#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <iomanip>

using namespace std;

class Book {
protected:
    int sn;
    char bname[30];
    char authname[30];
    char date[15];

public:
    void add();
    void booklist();
    void rmove();
    friend class Student;
};

class Student {
protected:
    int id;
    char stname[30];
    int bid;
    int clas;
    char bname[30];
    char date[15];

public:
    void issue();
    void retbook();
    void issuelist();
    void login();
    friend class Book;
};

fstream fb;
fstream fi;
fstream fp;
fstream fd;

int main() {
    system("COLOR 70");
    int choice;
    Student student;
    student.login(); // Logging in first
    MENU:
    cout << "\n\t____________Library Menu____________\n";
    cout << "\n\t\t 1.Add a Book ";
    cout << "\n\t\t 2.Issue a Book ";
    cout << "\n\t\t 3.Remove a Book";
    cout << "\n\t\t 4.Book List";
    cout << "\n\t\t 5.Issued Book List";
    cout << "\n\t\t 6.Return Issued Book";
    cout << "\n\t\t 0.Exit";
    cout << "\n\n\t\t Enter your choice :  ";
    cin >> choice;
    switch(choice) {
        case 1: {
            Book book;
            book.add();
            break;
        }
        case 2: {
        	// Student student;
            student.issue();
            break;
        }
        case 3: {
            Book book;
            book.rmove();
            break;
        }
        case 4: {
            Book book;
            book.booklist();
            break;
        }
        case 5: {
            Student student;
            student.issuelist();
            break;
        }
        case 6: {
            Student student;
            student.retbook();
            break;
        }
        case 0:
            exit(0);
        default:
            cout << "Enter Correct choice. ";
            goto MENU;
    }
    goto MENU;
    return 0;
}


void Book::add() {
    fb.open("library.txt", ios::out | ios::app | ios::binary);
    if (!fb.is_open()) {
        cout << "Error: Unable to open file for writing." << endl;
        return;
    }
    
    Book b;
    cout << "\nEnter Book id :  ";
    cin >> b.sn;
    cout << "\nEnter Book name :  ";
    cin.ignore();
    cin.getline(b.bname, 30);
    cout << "\nEnter Author Name :  ";
    cin.getline(b.authname, 30);
    cout << "\nEnter date in year/month/day order : ";
    cin.getline(b.date, 15);
    
    fb.write((char*)&b, sizeof(b));
    if (fb.fail()) {
        cout << "Error: Failed to write data to file." << endl;
    } else {
        cout << "\nBook Added Successfully!!!" << endl;
    }
    
    fb.close();
}



void Book::booklist() {
    system("cls");
    cout << "\n\t\t________Available Books________\n\n";
    cout << setw(10) << "Book ID" << setw(30) << "Book Name" << setw(20) << "Author" << setw(15) << "Date" << endl;
    fb.open("library.txt", ios::in | ios::binary);
    Book b;
    while (fb.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        cout << setw(10) << b.sn << setw(30) << b.bname << setw(20) << b.authname << setw(15) << b.date << endl;
    }
    cout << "\n\n\n\n\n\n\n";
    fb.close();
}

void Book::rmove() {
    int id, check = 0;
    system("cls");
    fb.open("library.txt", ios::in | ios::binary);
    cout << "\n\tEnter Book id to Remove : ";
    cin >> id;
    fd.open("dlt.txt", ios::out | ios::binary);
    Book b;
    while (fb.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (id == b.sn) {
            check = 1;
            continue;
        } else {
            fd.write(reinterpret_cast<char*>(&b), sizeof(b));
        }
    }
    fb.close();
    fd.close();
    remove("library.txt");
    rename("dlt.txt", "library.txt");
    if (check == 1) {
        cout << "\n\tBook Deleted Successfully.\n\n\n\n\n\n";
    }
    if (check != 1) {
        cout << "\n\tRecord not found!!\n\n\n\n\n\n";
    }
}

void Student::issue() {
    int check = 0;
    system("cls");
    fb.open("library.txt", ios::in | ios::binary);
    if (!fb.is_open()) {
        cout << "Error: Unable to open file for reading." << endl;
        return;
    }

    Book b;
    cout << "\n\tEnter book id to Issue : ";
    cin >> bid;
    while (fb.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.sn == bid) {
            check = 1;
            strcpy(bname, b.bname); 
            break;
        } else {
            check = 2;
        }
    }
    fb.close();
    if (check == 2) {
        cout << "\n\tRecord not found!!";
        cout << "\n\tPlease enter correct book id. ";
        sleep(2);
        return; 
    }

    fi.open("issuedbooks.dat", ios::app | ios::binary);
    if (!fi.is_open()) {
        cout << "Error: Unable to open file for writing." << endl;
        return;
    }

    cout << "\n\tEnter student name : ";
    cin.ignore();
    cin.getline(stname, 30);
    cout << "\n\tEnter student class : ";
    cin >> clas;
    cout << "\n\tEnter student id : ";
    cin.ignore();
    cin >> id;
    cout << "\n\tEnter date in year/month/day order : ";
    cin.ignore();
    cin.getline(date, 15);

    fi.write(reinterpret_cast<char*>(this), sizeof(*this));
    if (fi.fail()) {
        cout << "Error: Failed to write data to file." << endl;
    } else {
        cout << "\n\tBook Issued Successfully \n\n\n\n\n";
    }

    fi.close();
}

void Student::retbook() {
    fstream fs;
    int id, sn, check = 0;
    system("cls");
    fi.open("issuedbooks.dat", ios::in | ios::binary);
    cout << "\n\n\tEnter std id and book id to Remove : ";
    cin >> id >> sn;
    fs.open("dl.txt", ios::out | ios::binary);
    Student s;
    while (fi.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (id == s.id && sn == s.bid) {
            check = 1;
            continue;
        } else {
            fs.write(reinterpret_cast<char*>(&s), sizeof(s));
        }
    }
    fi.close();
    fs.close();
    remove("issuedbooks.dat");
    rename("dl.txt", "issuedbooks.dat");
    if (check == 1) {
        cout << "\n\tBook returned Successfully..\n\n\n\n\n";
    }
    if (check != 1) {
        cout << "\n\tRecord not found!!\n\n\n\n\n\n";
    }
}

void Student::issuelist() {
    system("cls");
    cout << "\n\t\t Issued Book list : ";
    cout << "\n" << setw(10) << "Student ID" << setw(20) << "Student Name" << setw(10) << "Student Class" << setw(10) << "Book ID" << setw(20) << "Book name" << setw(15) << "Date" << endl;
    fi.open("issuedbooks.dat", ios::in | ios::binary);
    Student s;
    while (fi.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        cout << "\n" << setw(10) << s.id << setw(20) << s.stname << setw(10) << s.clas << setw(10) << s.bid << setw(20) << s.bname << setw(15) << s.date << endl;
    }
    cout << "\n\n\n\n\n";
    fi.close();
}

void Student::login() {
    char us[]="devs";
    char pas[20];
    char ok[10];
    char ne[10];
    char get[10];
    int f,s;
    log:
    cout << "\n-------------------------------------------";
    cout << "\n\n\n\n     LIBRARY MANAGEMENT SYSTEM";
    cout << "\n\n\n--------------------------------------------\n\n";
    cout << "\n\tEnter the username  : ";
    cin.getline(ok, 10);
    cout << "\n\n\tEnter the password/pin  :";
    cin.getline(pas, 20);
    system("cls");
    fp.open("LOG.txt", ios::in | ios::out | ios::binary);
    fp.read(reinterpret_cast<char*>(&get), sizeof(get));
    if(strcmp(us,ok)!=0) {
        cout << "\n\tUsername wrong try again !";
        sleep(3);
        goto log;
    }
    if(strcmp(us,ok)==0 && strcmp(pas,get)==0) {
        cout << "\n\n\n\t\t\tLOGGED IN SUCCESSFULLY\n";
        sleep(3);
        system("cls");
    } else {
        fp.seekg(0, ios::beg);
        cout << "\n\tPassword wrong!!\n\tIf forgot password Press 1 and any other to retry: ";
        cin >> f;
        cin.ignore();
        if(f==1) {
            cout << "\n\tEnter Security pin to change password : ";
            cin >> s;
            if(s==62) {
                cout << "\n\tEnter New password/pin max length 8 : ";
                cin >> ne;
                fp.write(reinterpret_cast<char*>(&ne), sizeof(ne));
                cout << "\n\tPassword changed Successfully . ";
                sleep(3);
                system("cls");
            }
        } else {
            goto log;
        }
    }
    fp.close();
}

