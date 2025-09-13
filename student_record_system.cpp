#include <bits/stdc++.h>
using namespace std;

struct Student {
    int id;
    string name;
    int age;
    string department;
    double cgpa;

    string toCSV() const {
        // escape commas in name/department if needed - simple version
        return to_string(id) + "," + name + "," + to_string(age) + "," + department + "," + to_string(cgpa);
    }

    static Student fromCSV(const string &line) {
        Student s;
        stringstream ss(line);
        string token;

        getline(ss, token, ','); s.id = stoi(token);
        getline(ss, s.name, ',');
        getline(ss, token, ','); s.age = stoi(token);
        getline(ss, s.department, ',');
        getline(ss, token, ','); s.cgpa = stod(token);
        return s;
    }
};

const string DB_FILENAME = "students.csv";

vector<Student> loadAll() {
    vector<Student> v;
    ifstream fin(DB_FILENAME);
    if (!fin.is_open()) return v;
    string line;
    while (getline(fin, line)) {
        if (line.size() > 0) {
            try {
                v.push_back(Student::fromCSV(line));
            } catch (...) { /* skip malformed */ }
        }
    }
    fin.close();
    return v;
}

void saveAll(const vector<Student> &v) {
    ofstream fout(DB_FILENAME, ios::trunc);
    for (const auto &s : v) fout << s.toCSV() << "\n";
    fout.close();
}

int generateId(const vector<Student> &v) {
    int mx = 0;
    for (auto &s : v) if (s.id > mx) mx = s.id;
    return mx + 1;
}

void addStudent() {
    vector<Student> v = loadAll();
    Student s;
    s.id = generateId(v);
    cout << "New Student ID: " << s.id << "\n";
    cout << "Name: "; getline(cin, s.name);
    cout << "Age: "; cin >> s.age; cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Department: "; getline(cin, s.department);
    cout << "CGPA: "; cin >> s.cgpa; cin.ignore(numeric_limits<streamsize>::max(), '\n');
    v.push_back(s);
    saveAll(v);
    cout << "Student added.\n";
}

void viewAll() {
    vector<Student> v = loadAll();
    if (v.empty()) { cout << "No records found.\n"; return; }
    cout << left << setw(6) << "ID" << setw(25) << "Name" << setw(6) << "Age" << setw(15) << "Department" << setw(6) << "CGPA\n";
    cout << string(60, '-') << "\n";
    for (auto &s : v) {
        cout << setw(6) << s.id << setw(25) << s.name.substr(0,24) << setw(6) << s.age << setw(15) << s.department.substr(0,14) << setw(6) << fixed << setprecision(2) << s.cgpa << "\n";
    }
}

void searchById() {
    vector<Student> v = loadAll();
    if (v.empty()) { cout << "No records.\n"; return; }
    int id; cout << "Enter ID to search: "; cin >> id; cin.ignore();
    for (auto &s : v) {
        if (s.id == id) {
            cout << "Found:\n";
            cout << "ID: " << s.id << "\nName: " << s.name << "\nAge: " << s.age << "\nDepartment: " << s.department << "\nCGPA: " << s.cgpa << "\n";
            return;
        }
    }
    cout << "Student not found.\n";
}

void updateStudent() {
    vector<Student> v = loadAll();
    if (v.empty()) { cout << "No records.\n"; return; }
    int id; cout << "Enter ID to update: "; cin >> id; cin.ignore();
    for (auto &s : v) {
        if (s.id == id) {
            cout << "Updating student: " << s.name << "\n";
            cout << "New Name (enter to keep): "; string tmp;
            getline(cin, tmp); if (!tmp.empty()) s.name = tmp;
            cout << "New Age (enter 0 to keep): "; int a; cin >> a; cin.ignore(); if (a>0) s.age = a;
            cout << "New Department (enter to keep): "; getline(cin, tmp); if (!tmp.empty()) s.department = tmp;
            cout << "New CGPA (enter -1 to keep): "; double g; cin >> g; cin.ignore(); if (g >= 0) s.cgpa = g;
            saveAll(v);
            cout << "Updated.\n";
            return;
        }
    }
    cout << "ID not found.\n";
}

void deleteStudent() {
    vector<Student> v = loadAll();
    if (v.empty()) { cout << "No records.\n"; return; }
    int id; cout << "Enter ID to delete: "; cin >> id; cin.ignore();
    auto it = remove_if(v.begin(), v.end(), [&](const Student &s){ return s.id == id; });
    if (it != v.end()) {
        v.erase(it, v.end());
        saveAll(v);
        cout << "Deleted.\n";
    } else cout << "ID not found.\n";
}

void sortByName() {
    vector<Student> v = loadAll();
    sort(v.begin(), v.end(), [](const Student &a, const Student &b){ return a.name < b.name; });
    saveAll(v);
    cout << "Sorted by name and saved.\n";
}

void menu() {
    while (true) {
        cout << "\n--- Student Record System ---\n";
        cout << "1. Add Student\n2. View All Students\n3. Search by ID\n4. Update Student\n5. Delete Student\n6. Sort by Name\n7. Exit\nChoose: ";
        int ch; cin >> ch; cin.ignore();
        switch (ch) {
            case 1: addStudent(); break;
            case 2: viewAll(); break;
            case 3: searchById(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: sortByName(); break;
            case 7: cout << "Bye.\n"; return;
            default: cout << "Invalid.\n";
        }
    }
}

int main() {
    cout << "Student Record System\n";
    menu();
    return 0;
}
