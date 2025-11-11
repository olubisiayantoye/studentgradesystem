#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// -------------------- STRUCT --------------------
struct Student {
    string id;
    string name;
    string subject;
    double score1;
    double score2;
    double score3;
    double average;
    char grade;
};

// -------------------- FUNCTION DECLARATIONS --------------------
void addStudent(vector<Student> &students);
void displayStudents(const vector<Student> &students);
void saveToFile(const vector<Student> &students, const string &filename);
void loadFromFile(vector<Student> &students, const string &filename);
void searchStudent(const vector<Student> &students);
void editStudent(vector<Student> &students);
void deleteStudent(vector<Student> &students);
char getLetterGrade(double avg);
string generateID();
void calculateGrade(Student &s);

// -------------------- MAIN --------------------
int main() {
    vector<Student> students;
    string filename = "students.txt";
    int choice;

    loadFromFile(students, filename);

    do {
        cout << "\n===== STUDENT GRADE MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Edit Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Save to File\n";
        cout << "7. Load from File\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(students); break;
            case 2: displayStudents(students); break;
            case 3: searchStudent(students); break;
            case 4: editStudent(students); break;
            case 5: deleteStudent(students); break;
            case 6: saveToFile(students, filename); break;
            case 7: loadFromFile(students, filename); break;
            case 8: cout << "Exiting program... Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 8);

    return 0;
}

// -------------------- HELPER FUNCTIONS --------------------

// Generate simple incremental ID
string generateID() {
    static int id = 1000;
    return "S" + to_string(id++);
}

// Calculate average and grade
void calculateGrade(Student &s) {
    s.average = (s.score1 + s.score2 + s.score3) / 3.0;
    s.grade = getLetterGrade(s.average);
}

// Determine letter grade
char getLetterGrade(double avg) {
    if (avg >= 90) return 'A';
    else if (avg >= 80) return 'B';
    else if (avg >= 70) return 'C';
    else if (avg >= 60) return 'D';
    else return 'F';
}

// -------------------- CRUD FUNCTIONS --------------------

void addStudent(vector<Student> &students) {
    Student s;
    s.id = generateID();

    cin.ignore();
    cout << "\nEnter student name: ";
    getline(cin, s.name);

    cout << "Enter subject: ";
    getline(cin, s.subject);

    cout << "Enter score 1: ";
    cin >> s.score1;
    cout << "Enter score 2: ";
    cin >> s.score2;
    cout << "Enter score 3: ";
    cin >> s.score3;

    calculateGrade(s);
    students.push_back(s);

    cout << "\nStudent added successfully! ID: " << s.id << "\n";
}

// Display all student records
void displayStudents(const vector<Student> &students) {
    if (students.empty()) {
        cout << "\nNo student records found.\n";
        return;
    }

    cout << "\n---------------------------------------------------------------\n";
    cout << left << setw(8) << "ID"
         << setw(20) << "Name"
         << setw(15) << "Subject"
         << setw(8) << "Avg"
         << setw(6) << "Grade" << endl;
    cout << "---------------------------------------------------------------\n";

    for (const auto &s : students) {
        cout << left << setw(8) << s.id
             << setw(20) << s.name
             << setw(15) << s.subject
             << setw(8) << fixed << setprecision(2) << s.average
             << setw(6) << s.grade << endl;
    }
}

// Search student by name or ID
void searchStudent(const vector<Student> &students) {
    if (students.empty()) {
        cout << "\nNo student records to search.\n";
        return;
    }

    cin.ignore();
    string keyword;
    cout << "\nEnter student ID or name to search: ";
    getline(cin, keyword);

    bool found = false;
    for (const auto &s : students) {
        if (s.id == keyword || s.name.find(keyword) != string::npos) {
            cout << "\n--- Student Found ---\n";
            cout << "ID: " << s.id << "\nName: " << s.name
                 << "\nSubject: " << s.subject
                 << "\nScores: " << s.score1 << ", " << s.score2 << ", " << s.score3
                 << "\nAverage: " << fixed << setprecision(2) << s.average
                 << "\nGrade: " << s.grade << endl;
            found = true;
        }
    }

    if (!found) cout << "No matching student found.\n";
}

// Edit existing student
void editStudent(vector<Student> &students) {
    if (students.empty()) {
        cout << "\nNo students to edit.\n";
        return;
    }

    cin.ignore();
    string id;
    cout << "\nEnter student ID to edit: ";
    getline(cin, id);

    for (auto &s : students) {
        if (s.id == id) {
            cout << "Editing student: " << s.name << endl;

            cout << "Enter new name (or press Enter to keep current): ";
            string nameInput;
            getline(cin, nameInput);
            if (!nameInput.empty()) s.name = nameInput;

            cout << "Enter new subject (or press Enter to keep current): ";
            string subjInput;
            getline(cin, subjInput);
            if (!subjInput.empty()) s.subject = subjInput;

            cout << "Enter new score 1 (-1 to keep current): ";
            double sc;
            cin >> sc;
            if (sc >= 0) s.score1 = sc;

            cout << "Enter new score 2 (-1 to keep current): ";
            cin >> sc;
            if (sc >= 0) s.score2 = sc;

            cout << "Enter new score 3 (-1 to keep current): ";
            cin >> sc;
            if (sc >= 0) s.score3 = sc;

            calculateGrade(s);
            cout << "Student record updated successfully!\n";
            return;
        }
    }
    cout << "Student ID not found.\n";
}

// Delete student
void deleteStudent(vector<Student> &students) {
    if (students.empty()) {
        cout << "\nNo students to delete.\n";
        return;
    }

    cin.ignore();
    string id;
    cout << "\nEnter student ID to delete: ";
    getline(cin, id);

    auto it = remove_if(students.begin(), students.end(),
                        [&](const Student &s) { return s.id == id; });

    if (it != students.end()) {
        students.erase(it, students.end());
        cout << "Student deleted successfully.\n";
    } else {
        cout << "Student ID not found.\n";
    }
}

// -------------------- FILE FUNCTIONS --------------------

void saveToFile(const vector<Student> &students, const string &filename) {
    ofstream file(filename);
    if (!file) {
        cout << "Error: Could not open file for writing.\n";
        return;
    }

    for (const auto &s : students) {
        file << s.id << ","
             << s.name << ","
             << s.subject << ","
             << s.score1 << ","
             << s.score2 << ","
             << s.score3 << ","
             << s.average << ","
             << s.grade << "\n";
    }

    cout << "Data saved successfully to " << filename << endl;
}

void loadFromFile(vector<Student> &students, const string &filename) {
    ifstream file(filename);
    if (!file) {
        cout << "No saved data found. Starting fresh.\n";
        return;
    }

    students.clear();
    string line;
    while (getline(file, line)) {
        Student s;
        size_t pos = 0;
        vector<string> tokens;
        while ((pos = line.find(',')) != string::npos) {
            tokens.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }
        tokens.push_back(line);

        if (tokens.size() == 8) {
            s.id = tokens[0];
            s.name = tokens[1];
            s.subject = tokens[2];
            s.score1 = stod(tokens[3]);
            s.score2 = stod(tokens[4]);
            s.score3 = stod(tokens[5]);
            s.average = stod(tokens[6]);
            s.grade = tokens[7][0];
            students.push_back(s);
        }
    }

    cout << "Data loaded from " << filename << " successfully.\n";
}
