#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <limits>

using namespace std;

// -------------------- STRUCT --------------------
struct Student {
    string id;
    string name;
    string subject;
    double score1 = 0.0;
    double score2 = 0.0;
    double score3 = 0.0;
    double average = 0.0;
    char grade = 'F';
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
void updateIDCounterFromStudents(const vector<Student> &students);
vector<string> splitLine(const string &line, char delimiter);

// -------------------- MAIN --------------------
int main() {
    vector<Student> students;
    string filename = "students.txt";
    int choice;

    loadFromFile(students, filename);
    updateIDCounterFromStudents(students);

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
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid entry. Please enter a number 1-8.\n";
            continue;
        }

        switch (choice) {
            case 1: addStudent(students); break;
            case 2: displayStudents(students); break;
            case 3: searchStudent(students); break;
            case 4: editStudent(students); break;
            case 5: deleteStudent(students); break;
            case 6: saveToFile(students, filename); break;
            case 7: loadFromFile(students, filename); updateIDCounterFromStudents(students); break;
            case 8: cout << "Exiting program... Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 8);

    return 0;
}

// -------------------- HELPER FUNCTIONS --------------------

// Generate simple incremental ID (persistent during program run)
string generateID() {
    static int idCounter = 1000;
    string id = "S" + to_string(idCounter++);
    return id;
}

// Update the internal static ID counter so newly generated IDs won't collide after loading file
void updateIDCounterFromStudents(const vector<Student> &students) {
    int maxNum = -1;
    for (const auto &s : students) {
        if (s.id.size() > 1 && (s.id[0] == 'S' || s.id[0] == 's')) {
            string numPart = s.id.substr(1);
            bool ok = !numPart.empty();
            for (char c : numPart) if (!isdigit((unsigned char)c)) { ok = false; break; }
            if (ok) {
                int val = stoi(numPart);
                if (val > maxNum) maxNum = val;
            }
        }
    }
    if (maxNum >= 0) {
        // set static variable in generateID to maxNum + 1
        // trick: call generateID until counter reaches desired value by creating and discarding strings
        // but better approach: use a lambda to set the static via one-time initialization:
        struct Setter {
            static void set(int newStart) {
                // We need to set the static inside generateID; easiest is to create a local static function-scoped variable here
                // However, direct access to generateID's static is not possible. Instead, we call generateID repeatedly until it exceeds newStart.
            }
        };
        // Simpler: call generateID repeatedly until the numeric part > maxNum
        // This will advance the static counter appropriately.
        while (true) {
            string nextId = generateID(); // this increments the static counter
            int val = stoi(nextId.substr(1));
            if (val > maxNum) break;
        }
    }
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

// safe numeric input for scores (0..100)
double readScore(const string &prompt, double currentValue = -1.0, bool allowKeep = false) {
    double sc;
    while (true) {
        if (allowKeep && currentValue >= 0.0) {
            cout << prompt << " (enter -1 to keep current " << fixed << setprecision(2) << currentValue << "): ";
        } else {
            cout << prompt << " (0-100): ";
        }
        if (!(cin >> sc)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid number. Try again.\n";
            continue;
        }
        if (allowKeep && sc == -1) return currentValue;
        if (sc < 0.0 || sc > 100.0) {
            cout << "Score must be between 0 and 100.\n";
            continue;
        }
        return sc;
    }
}

// -------------------- CRUD FUNCTIONS --------------------

void addStudent(vector<Student> &students) {
    Student s;
    s.id = generateID();

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\nEnter student name: ";
    getline(cin, s.name);
    if (s.name.find(',') != string::npos || s.name.find('|') != string::npos) {
        cout << "Warning: Avoid commas or '|' characters in name; they may affect file parsing.\n";
    }

    cout << "Enter subject: ";
    getline(cin, s.subject);
    if (s.subject.find(',') != string::npos || s.subject.find('|') != string::npos) {
        cout << "Warning: Avoid commas or '|' characters in subject; they may affect file parsing.\n";
    }

    s.score1 = readScore("Enter score 1");
    s.score2 = readScore("Enter score 2");
    s.score3 = readScore("Enter score 3");

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

    cout << "\n----------------------------------------------------------------\n";
    cout << left << setw(8) << "ID"
         << setw(20) << "Name"
         << setw(18) << "Subject"
         << setw(8) << "Avg"
         << setw(6) << "Grade" << endl;
    cout << "----------------------------------------------------------------\n";

    for (const auto &s : students) {
        cout << left << setw(8) << s.id
             << setw(20) << s.name
             << setw(18) << s.subject
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

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string keyword;
    cout << "\nEnter student ID or name to search: ";
    getline(cin, keyword);

    bool found = false;
    for (const auto &s : students) {
        string lowerName = s.name;
        string lowerKeyword = keyword;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);

        if (s.id == keyword || lowerName.find(lowerKeyword) != string::npos) {
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

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string id;
    cout << "\nEnter student ID to edit: ";
    getline(cin, id);

    for (auto &s : students) {
        if (s.id == id) {
            cout << "Editing student: " << s.name << " (" << s.id << ")\n";

            cout << "Enter new name (or press Enter to keep current): ";
            string nameInput;
            getline(cin, nameInput);
            if (!nameInput.empty()) s.name = nameInput;

            cout << "Enter new subject (or press Enter to keep current): ";
            string subjInput;
            getline(cin, subjInput);
            if (!subjInput.empty()) s.subject = subjInput;

            s.score1 = readScore("Enter new score 1", s.score1, true);
            s.score2 = readScore("Enter new score 2", s.score2, true);
            s.score3 = readScore("Enter new score 3", s.score3, true);

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

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string id;
    cout << "\nEnter student ID to delete: ";
    getline(cin, id);

    auto it = remove_if(students.begin(), students.end(),
                        [&](const Student &s) { return s.id == id; });

    if (it != students.end()) {
        cout << "Are you sure you want to delete student " << id << "? (y/n): ";
        char c;
        cin >> c;
        if (c == 'y' || c == 'Y') {
            students.erase(it, students.end());
            cout << "Student deleted successfully.\n";
        } else {
            cout << "Deletion cancelled.\n";
        }
    } else {
        cout << "Student ID not found.\n";
    }
}

// -------------------- FILE FUNCTIONS --------------------

// Split helper
vector<string> splitLine(const string &line, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream ss(line);
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void saveToFile(const vector<Student> &students, const string &filename) {
    ofstream file(filename);
    if (!file) {
        cout << "Error: Could not open file for writing.\n";
        return;
    }

    // prefer pipe '|' delimiter (safer than comma if names contain commas)
    for (const auto &s : students) {
        file << s.id << "|"
             << s.name << "|"
             << s.subject << "|"
             << s.score1 << "|"
             << s.score2 << "|"
             << s.score3 << "|"
             << s.average << "|"
             << s.grade << "\n";
    }

    cout << "Data saved successfully to " << filename << " (using '|' delimiter).\n";
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
        if (line.empty()) continue;
        vector<string> tokens;
        // support both '|' and ',' files
        if (line.find('|') != string::npos) tokens = splitLine(line, '|');
        else tokens = splitLine(line, ',');

        // If we have more than expected when comma is used and name/subject included commas,
        // we attempt a safe fallback: require at least 8 tokens, otherwise skip line.
        if (tokens.size() < 8) {
            cout << "Warning: Skipping malformed line in file: " << line << "\n";
            continue;
        }

        Student s;
        s.id = tokens[0];
        s.name = tokens[1];
        s.subject = tokens[2];

        // If tokens > 8 (comma in name/subject), try to reconstruct by taking last 5 tokens as scores/avg/grade
        if (tokens.size() > 8) {
            int n = tokens.size();
            // last 5 tokens should be score1,score2,score3,average,grade
            s.score1 = stod(tokens[n-5]);
            s.score2 = stod(tokens[n-4]);
            s.score3 = stod(tokens[n-3]);
            s.average = stod(tokens[n-2]);
            s.grade = tokens[n-1][0];
            // reconstruct name+subject by joining tokens[1..n-6] and splitting them naively
            // fallback: keep tokens[1] and tokens[2] as earlier, because accurate recovery is complex
        } else {
            try {
                s.score1 = stod(tokens[3]);
                s.score2 = stod(tokens[4]);
                s.score3 = stod(tokens[5]);
                s.average = stod(tokens[6]);
                s.grade = tokens[7][0];
            } catch (...) {
                cout << "Warning: Error parsing numeric fields for line: " << line << "\n";
                continue;
            }
        }
        students.push_back(s);
    }

    cout << "Data loaded from " << filename << " successfully. " << students.size() << " record(s) loaded.\n";
}
