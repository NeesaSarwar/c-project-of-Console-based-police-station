#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <conio.h>
#include <cstdlib>
using namespace std;

// Here are the global Variables
string id, name, crime, punishment, dateOfCrime;
string choice, role, username, password;
const string file = "police_data.txt";
bool isAdmin = false;

// Below are the function declarations
void login();
void menu();
void enterData();
void displayData();
void searchData();
void updateData();
void deleteData();
bool checkDuplicateID(const string& checkId);
string getMaskedInput();

int main() {
    login();
    return 0;
}

// This function handles the login
void login() {
    system("cls");
    cout << "\n\n\t\tPolice Station Management System Login\n";
    cout << "\t\t--------------------------------------\n";
    cout << "\t\tSelect Portal:\n";
    cout << "\t\t1. Admin\n";
    cout << "\t\t2. Police Officer\n";
    cout << "\t\tEnter your choice: ";
    cin >> choice;

    if (choice == "1") {
        role = "admin";
    }
    else if (choice == "2") {
        role = "officer";
    }
    else {
        cout << "\t\tInvalid choice. Try again.\n";
        system("pause");
        login();
    }

    cout << "\n\t\tEnter Username: ";
    cin >> username;
    cout << "\t\tDo you want to mask password while typing? (Y/N): ";
    char opt;
    cin >> opt;
    cout << "\t\tEnter Password: ";
    if (opt == 'Y' || opt == 'y')
        password = getMaskedInput();
    else
        cin >> password;

    if (role == "admin" && username == "admin" && password == "admin") {
        isAdmin = true;
        cout << "\t\tAdmin Login Successful!\n";
        system("pause");
        menu();
    }
    else if (role == "officer" && username == "officer" && password == "officer") {
        isAdmin = false;
        cout << "\t\tOfficer Login Successful!\n";
        system("pause");
        menu();
    }
    else {
        cout << "\t\tInvalid credentials. Try again.\n";
        system("pause");
        login();
    }
}

// This displays the menu
void menu() {
    system("cls");
    cout << "\n\n\t\tWelcome to Police Station Management System (" << (isAdmin ? "Admin" : "Officer") << " Portal)\n";
    cout << "\t\t------------------------------------------------\n";
    cout << "\t1. Display All Records\n";
    cout << "\t2. Search Record\n";

    if (isAdmin) {
        cout << "\t3. Enter Data\n";
        cout << "\t4. Update Record\n";
        cout << "\t5. Delete Record\n";
        cout << "\t6. Exit\n";
    }
    else {
        cout << "\t3. Exit\n";
    }

    cout << "\tChoose an option: ";
    cin >> choice;

    if (choice == "1") {
        displayData();
    }
    else if (choice == "2") {
        searchData();
    }
    else if (choice == "3" && isAdmin) {
        enterData();
    }
    else if (choice == "4" && isAdmin) {
        updateData();
    }
    else if (choice == "5" && isAdmin) {
        deleteData();
    }
    else if ((choice == "6" && isAdmin) || (choice == "3" && !isAdmin)) {
        exit(0);
    }
    else {
        cout << "\tInvalid option! Try again.\n";
        system("pause");
        menu();
    }
}

void enterData() {
    system("cls");
    cout << "\n\tEnter Criminal ID: ";
    cin >> ws;
    getline(cin, id);

    if (checkDuplicateID(id)) {
        cout << "\n\tRecord with this ID already exists. Cannot add duplicate.\n";
        system("pause");
        menu();
        return;
    }

    cout << "\tEnter Name: ";
    getline(cin, name);
    cout << "\tEnter Crime: ";
    getline(cin, crime);
    cout << "\tEnter Punishment: ";
    getline(cin, punishment);
    cout << "\tEnter Date of Crime (DD/MM/YYYY): ";
    getline(cin, dateOfCrime);

    ofstream fileOut(file, ios::app);
    fileOut << id << "|" << name << "|" << crime << "|" << punishment << "|" << dateOfCrime << endl;
    fileOut.close();

    cout << "\n\tRecord added successfully.\n";
    system("pause");
    menu();
}

void displayData() {
    system("cls");
    ifstream fileIn(file);
    bool found = false;
    cout << "\n\tAll Records:\n";
    cout << "\t" << left << setw(10) << "ID"
        << setw(20) << "Name"
        << setw(20) << "Crime"
        << setw(20) << "Punishment"
        << setw(15) << "Date of Crime" << endl;
    cout << "\t" << string(85, '-') << endl;

    string line;
    while (getline(fileIn, line)) {
        if (line.empty()) continue;

        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        size_t p4 = line.find('|', p3 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos || p4 == string::npos)
            continue; // skip malformed data

        id = line.substr(0, p1);
        name = line.substr(p1 + 1, p2 - p1 - 1);
        crime = line.substr(p2 + 1, p3 - p2 - 1);
        punishment = line.substr(p3 + 1, p4 - p3 - 1);
        dateOfCrime = line.substr(p4 + 1);

        cout << "\t" << left << setw(10) << id
            << setw(20) << name
            << setw(20) << crime
            << setw(20) << punishment
            << setw(15) << dateOfCrime << endl;
        found = true;
    }
    fileIn.close();

    if (!found) {
        cout << "\tNo records found.\n";
    }

    system("pause");
    menu();
}

void searchData() {
    system("cls");
    string searchId;
    bool found = false;

    cout << "\n\tEnter ID to search: ";
    cin >> ws;
    getline(cin, searchId);

    ifstream fileIn(file);
    string line;
    while (getline(fileIn, line)) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        size_t p4 = line.find('|', p3 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos || p4 == string::npos)
            continue;

        id = line.substr(0, p1);
        name = line.substr(p1 + 1, p2 - p1 - 1);
        crime = line.substr(p2 + 1, p3 - p2 - 1);
        punishment = line.substr(p3 + 1, p4 - p3 - 1);
        dateOfCrime = line.substr(p4 + 1);

        if (id == searchId) {
            cout << "\n\tRecord Found:\n";
            cout << "\tID: " << id << "\n";
            cout << "\tName: " << name << "\n";
            cout << "\tCrime: " << crime << "\n";
            cout << "\tPunishment: " << punishment << "\n";
            cout << "\tDate of Crime: " << dateOfCrime << "\n";
            found = true;
            break;
        }
    }
    fileIn.close();

    if (!found) {
        cout << "\n\tNo record found for ID: " << searchId << endl;
    }

    system("pause");
    menu();
}

void updateData() {
    system("cls");
    string updateId;
    bool found = false;
    ofstream tempFile("temp.txt");
    ifstream fileIn(file);

    cout << "\n\tEnter ID to update: ";
    cin >> ws;
    getline(cin, updateId);

    string line;
    while (getline(fileIn, line)) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        size_t p4 = line.find('|', p3 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos || p4 == string::npos)
            continue;

        id = line.substr(0, p1);

        if (id == updateId) {
            cout << "\n\tEnter new Name: ";
            getline(cin, name);
            cout << "\tEnter new Crime: ";
            getline(cin, crime);
            cout << "\tEnter new Punishment: ";
            getline(cin, punishment);
            cout << "\tEnter new Date of Crime: ";
            getline(cin, dateOfCrime);

            tempFile << id << "|" << name << "|" << crime << "|" << punishment << "|" << dateOfCrime << endl;
            found = true;
        }
        else {
            tempFile << line << endl;
        }
    }

    fileIn.close();
    tempFile.close();
    remove(file.c_str());
    rename("temp.txt", file.c_str());

    if (found) {
        cout << "\n\tRecord updated successfully.\n";
    }
    else {
        cout << "\n\tRecord not found.\n";
    }

    system("pause");
    menu();
}

void deleteData() {
    system("cls");
    string deleteId;
    bool found = false;
    ofstream tempFile("temp.txt");
    ifstream fileIn(file);

    cout << "\n\tEnter ID to delete: ";
    cin >> ws;
    getline(cin, deleteId);

    string line;
    while (getline(fileIn, line)) {
        size_t p1 = line.find('|');

        if (p1 == string::npos)
            continue;

        id = line.substr(0, p1);

        if (id != deleteId) {
            tempFile << line << endl;
        }
        else {
            found = true;
        }
    }

    fileIn.close();
    tempFile.close();
    remove(file.c_str());
    rename("temp.txt", file.c_str());

    if (found) {
        cout << "\n\tRecord deleted successfully.\n";
    }
    else {
        cout << "\n\tRecord not found.\n";
    }

    system("pause");
    menu();
}

bool checkDuplicateID(const string& checkId) {
    ifstream fileIn(file);
    string line;
    while (getline(fileIn, line)) {
        size_t p1 = line.find('|');
        if (p1 != string::npos) {
            string currentId = line.substr(0, p1);
            if (currentId == checkId) {
                return true;
            }
        }
    }
    return false;
}

string getMaskedInput() {
    string input;
    char ch;
    while ((ch = _getch()) != 13) {
        if (ch == 8) {
            if (!input.empty()) {
                input.pop_back();
                cout << "\b \b";
            }
        }
        else {
            input.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;
    return input;
}