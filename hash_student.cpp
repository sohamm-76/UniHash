#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <cstdlib>
using namespace std;

struct Node {
    int id;
    string name;
    double cgpa;
    int attendance;
    Node* next;

    Node(int i, string n, double c, int a) {
        id = i;
        name = n;
        cgpa = c;
        attendance = a;
        next = NULL;
    }
};

class StudentHashTable {
private:
    static const int TABLE_SIZE = 7;
    Node* table[TABLE_SIZE];
    string fileName;

    int hashFunction(int key) const {
        return (key % TABLE_SIZE + TABLE_SIZE) % TABLE_SIZE;
    }

    void header() const {
        cout << left << setw(8) << "Bucket"
             << setw(12) << "Student ID"
             << setw(20) << "Student Name"
             << right << setw(8) << "CGPA"
             << setw(12) << "Attendance" << "\n" << left;
        cout << string(60, '-') << "\n";
    }

    void printRow(const Node* n, int bucket) const {
        cout << left << setw(8) << bucket
             << setw(12) << n->id
             << setw(20) << n->name
             << right << setw(8) << fixed << setprecision(2) << n->cgpa
             << setw(12) << n->attendance << "\n" << left;
    }

    static bool cmpById(const Node* a, const Node* b) {
        return a->id < b->id;
    }

public:
    StudentHashTable(string file) {
        fileName = file;
        for (int i = 0; i < TABLE_SIZE; i++)
            table[i] = NULL;
    }

    ~StudentHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* cur = table[i];
            while (cur != NULL) {
                Node* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
            table[i] = NULL;
        }
    }

    int getBucket(int key) const {
        return hashFunction(key);
    }

    Node* search(int key) const {
        int idx = hashFunction(key);
        Node* cur = table[idx];
        while (cur != NULL) {
            if (cur->id == key)
                return cur;
            cur = cur->next;
        }
        return NULL;
    }

    bool insert(int id, string name, double cgpa, int att) {
        if (search(id) != NULL)
            return false;
        int idx = hashFunction(id);
        Node* newNode = new Node(id, name, cgpa, att);
        if (table[idx] == NULL) {
            table[idx] = newNode;
        } else {
            Node* cur = table[idx];
            while (cur->next != NULL)
                cur = cur->next;
            cur->next = newNode;
        }
        return true;
    }

    bool updateCGPA(int id, double newCgpa) {
        Node* n = search(id);
        if (n == NULL)
            return false;
        n->cgpa = newCgpa;
        return true;
    }

    bool remove(int id) {
        int idx = hashFunction(id);
        Node* cur = table[idx];
        Node* prev = NULL;
        while (cur != NULL && cur->id != id) {
            prev = cur;
            cur = cur->next;
        }
        if (cur == NULL)
            return false;
        if (prev == NULL)
            table[idx] = cur->next;
        else
            prev->next = cur->next;
        delete cur;
        return true;
    }

    bool isEmpty() const {
        for (int i = 0; i < TABLE_SIZE; i++)
            if (table[i] != NULL) return false;
        return true;
    }

    void printOne(const Node* n) const {
        header();
        printRow(n, hashFunction(n->id));
    }

    void display(int mode) const {
        if (isEmpty()) {
            cout << "Hash table is empty.\n";
            return;
        }
        if (mode == 1) {
            header();
            for (int i = 0; i < TABLE_SIZE; i++)
                for (Node* cur = table[i]; cur != NULL; cur = cur->next)
                    printRow(cur, i);
        } else if (mode == 2) {
            vector<const Node*> all;
            for (int i = 0; i < TABLE_SIZE; i++)
                for (Node* cur = table[i]; cur != NULL; cur = cur->next)
                    all.push_back(cur);
            sort(all.begin(), all.end(), cmpById);
            header();
            for (size_t i = 0; i < all.size(); i++)
                printRow(all[i], hashFunction(all[i]->id));
        } else if (mode == 3) {
            int total = 0;
            cout << "Bucket  Chain Length  Student IDs in Chain\n";
            cout << string(52, '-') << "\n";
            for (int i = 0; i < TABLE_SIZE; i++) {
                int len = 0;
                for (Node* cur = table[i]; cur != NULL; cur = cur->next)
                    len++;
                total += len;
                cout << left << setw(8) << i << setw(14) << len;
                if (len == 0) {
                    cout << "NULL";
                } else {
                    for (Node* cur = table[i]; cur != NULL; cur = cur->next) {
                        cout << cur->id;
                        if (cur->next != NULL) cout << " -> ";
                    }
                }
                cout << "\n";
            }
            cout << "Total records: " << total << " | Load factor: "
                 << fixed << setprecision(2) << (double)total / TABLE_SIZE << "\n";
        }
    }

    bool save() const {
        ofstream out(fileName.c_str());
        if (!out) return false;
        for (int i = 0; i < TABLE_SIZE; i++) {
            for (Node* cur = table[i]; cur != NULL; cur = cur->next) {
                out << cur->id << '|' << cur->name << '|'
                    << cur->cgpa << '|' << cur->attendance << '\n';
            }
        }
        return true;
    }

    int load() {
        ifstream in(fileName.c_str());
        if (!in) return 0;
        string line;
        int count = 0;
        while (getline(in, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            int id, att;
            double cgpa;
            string name;
            char sep;
            if (ss >> id >> sep && getline(ss, name, '|') && ss >> cgpa >> sep >> att) {
                if (insert(id, name, cgpa, att))
                    count++;
            }
        }
        return count;
    }
};

int readInt(const string& prompt) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return v;
        }
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
    }
}

double readDouble(const string& prompt) {
    double v;
    while (true) {
        cout << prompt;
        if (cin >> v) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return v;
        }
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
    }
}

int main() {
    StudentHashTable records("students.txt");
    cout << records.load() << " record(s) loaded from students.txt\n";

    while (true) {
        cout << "\n===== HASH TABLE STUDENT RECORD SYSTEM =====\n"
             << "1. Insert Student\n"
             << "2. Search Student\n"
             << "3. Display Students (Bucket-wise / Sorted / Chain Summary)\n"
             << "4. Update CGPA\n"
             << "5. Delete Student\n"
             << "6. Exit\n";

        int choice = readInt("Enter your choice: ");

        if (choice == 1) {
            int id = readInt("Enter student ID: ");
            if (records.search(id) != NULL) {
                cout << "Student ID already exists. Insertion cancelled.\n";
                continue;
            }
            string name;
            cout << "Enter student name: ";
            getline(cin, name);
            while (name.empty() || name.find('|') != string::npos) {
                cout << "Name must be non-empty and must not contain '|'. Enter again: ";
                getline(cin, name);
            }
            double cgpa = readDouble("Enter CGPA: ");
            if (cgpa < 0.0 || cgpa > 10.0) {
                cout << "Invalid CGPA! Enter between 0 and 10.\n";
                continue;
            }
            int att = readInt("Enter attendance (%): ");
            if (att < 0 || att > 100) {
                cout << "Invalid Attendance! Enter between 0 and 100.\n";
                continue;
            }

            records.insert(id, name, cgpa, att);
            records.save();
            cout << "Student added to bucket " << records.getBucket(id) << " successfully.\n";

        } else if (choice == 2) {
            int id = readInt("Enter student ID to search: ");
            Node* n = records.search(id);
            if (n != NULL) {
                cout << "Student found.\n";
                records.printOne(n);
            } else {
                cout << "Student with ID " << id << " not found.\n";
            }

        } else if (choice == 3) {
            cout << "1. Bucket-wise (0 to 6)\n2. Sorted by ID\n3. Chain Summary\n";
            int m = readInt("Select display mode: ");
            if (m < 1 || m > 3) {
                cout << "Invalid display mode.\n";
                continue;
            }
            records.display(m);

        } else if (choice == 4) {
            int id = readInt("Enter student ID: ");
            if (records.search(id) == NULL) {
                cout << "Student with ID " << id << " not found.\n";
                continue;
            }
            double cgpa = readDouble("Enter new CGPA: ");
            if (cgpa < 0.0 || cgpa > 10.0) {
                cout << "Invalid CGPA! Enter between 0 and 10.\n";
                continue;
            }
            records.updateCGPA(id, cgpa);
            records.save();
            cout << "CGPA updated successfully.\n";

        } else if (choice == 5) {
            int id = readInt("Enter student ID to delete: ");
            if (records.remove(id)) {
                records.save();
                cout << "Student deleted successfully.\n";
            } else {
                cout << "Student with ID " << id << " not found.\n";
            }

        } else if (choice == 6) {
            if (records.save())
                cout << "Records saved to students.txt\n";
            else
                cout << "Warning: could not save records.\n";
            cout << "Memory released. Exiting program.\n";
            break;

        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}