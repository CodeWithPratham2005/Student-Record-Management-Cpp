#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class Student {
    int rollNo;
    string name;
    float marks;

public:
    void input() {
        cout << "\nEnter Roll Number: ";
        cin >> rollNo;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Marks: ";
        cin >> marks;
    }

    void display() const {
        cout << left << setw(10) << rollNo
             << setw(20) << name
             << setw(10) << marks << endl;
    }

    int getRollNo() const {
        return rollNo;
    }
};

void addRecord() {
    ofstream outFile("students.dat", ios::binary | ios::app);
    Student s;
    s.input();
    outFile.write(reinterpret_cast<char*>(&s), sizeof(s));
    outFile.close();
    cout << "\nRecord added successfully.\n";
}

void displayRecords() {
    ifstream inFile("students.dat", ios::binary);
    Student s;

    cout << "\nAll Student Records:\n";
    cout << left << setw(10) << "RollNo"
         << setw(20) << "Name"
         << setw(10) << "Marks" << endl;
    cout << "--------------------------------------------\n";

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        s.display();
    }

    inFile.close();
}

void updateRecord() {
    fstream file("students.dat", ios::binary | ios::in | ios::out);
    int roll;
    cout << "\nEnter Roll Number to update: ";
    cin >> roll;

    Student s;
    bool found = false;

    while (!file.eof()) {
        streampos pos = file.tellg();
        if (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
            if (s.getRollNo() == roll) {
                cout << "\nEnter New Details:\n";
                s.input();
                file.seekp(pos);
                file.write(reinterpret_cast<char*>(&s), sizeof(s));
                found = true;
                break;
            }
        }
    }

    file.close();

    if (found)
        cout << "\nRecord updated successfully.\n";
    else
        cout << "\nRecord not found.\n";
}

void deleteRecord() {
    int roll;
    cout << "\nEnter Roll Number to delete: ";
    cin >> roll;

    ifstream inFile("students.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    Student s;
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.getRollNo() != roll) {
            outFile.write(reinterpret_cast<char*>(&s), sizeof(s));
        } else {
            found = true;
        }
    }

    inFile.close();
    outFile.close();

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found)
        cout << "\nRecord deleted successfully.\n";
    else
        cout << "\nRecord not found.\n";
}

int main() {
    int choice;

    do {
        cout << "\n====== Student Record Management System ======\n";
        cout << "1. Add Record\n";
        cout << "2. Display All Records\n";
        cout << "3. Update a Record\n";
        cout << "4. Delete a Record\n";
        cout << "5. Exit\n";
        cout << "Enter your choice (1-5): ";
        cin >> choice;

        switch (choice) {
            case 1: addRecord(); break;
            case 2: displayRecords(); break;
            case 3: updateRecord(); break;
            case 4: deleteRecord(); break;
            case 5: cout << "\nExiting program...\n"; break;
            default: cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 5);

    return 0;
}
