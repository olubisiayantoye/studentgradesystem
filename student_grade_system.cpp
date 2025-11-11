#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

// Structure to store student data
struct Student {
    string name;
    double score1;
    double score2;
    double score3;
    double average;
    char grade;
};

// Function declarations
void addStudent(vector<Student> &students);
void calculateGrade(Student &student);
void displayStudents(const vector<Student> &students);
void saveToFile(const vector<Student> &students, const string &filename);
void loadFromFile(vector<Student> &students, const string &filename);
char getLetterGrade(double avg);

int main() {
    vector<Student> students;
    string filename = "students.txt";

    int choice;
    loadFromFile(students, filename);

    do {
        cout << "\n=== Student Grade Management System ===\n";
        cout << "1. Add Student\n";
        cout << "2. Display Students\n";
        cout << "3. Save to File\n";
        cout << "4. Load from File\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addStudent(students);
                break;
            case 2:
                displayStudents(students);
                break;
            case 3:
                saveToFile(students, filename);
                break;
            case 4:
                loadFromFile(students, filename);
                break;
            case 5:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}

// Add a new student and compute average + grade
void addStudent(vector<Student> &students) {
    Student s;
    cout << "\nEnter student name: ";
    cin.ignore();
    getline(cin, s.name);

    cout << "Enter score 1: ";
    cin >> s.score1;
    cout << "Enter score 2: ";
    cin >> s.score2;
    cout << "Enter score 3: ";
    cin >> s.score3;

    s.average = (s.score1 + s.score2 + s.score3) / 3.0;
    s.grade = getLetterGrade(s.average);

    students.push_back(s);
    cout << "Student added successfully!\n";
}

// Determine letter grade based on average
char getLetterGrade(double avg) {
    if (avg >= 90) return 'A';
    else if (avg >= 80) return 'B';
    else if (avg >= 70) return 'C';
    else if (avg >= 60) return 'D';
    else return 'F';
}

// Display all students
void displayStudents(const vector<Student> &students) {
    if (students.empty()) {
        cout << "\nNo student records found.\n";
        return;
    }

    cout << "\n--- Student Records ---\n";
    cout << left << setw(20) << "Name" 
         << setw(10) << "Score1" 
         << setw(10) << "Score2"
         << setw(10) << "Score3"
         << setw(10) << "Average"
         << setw(10) << "Grade" << endl;
    cout << "-----------------------------------------------------------\n";

    for (const auto &s : students) {
        cout << left << setw(20) << s.name
             << setw(10) << s.score1
             << setw(10) << s.score2
             << setw(10) << s.score3
             << setw(10) << fixed << setprecision(2) << s.average
             << setw(10) << s.grade << endl;
    }
}

// Save students to file
void saveToFile(const vector<Student> &students, const string &filename) {
    ofstream file(filename);
    if (!file) {
        cout << "Error: Could not open file for writing.\n";
        return;
    }

    for (const auto &s : students) {
        file << s.name << ","
             << s.score1 << ","
             << s.score2 << ","
             << s.score3 << ","
             << s.average << ","
             << s.grade << endl;
    }

    file.close();
    cout << "Data saved to " << filename << " successfully.\n";
}

// Load students from file
void loadFromFile(vector<Student> &students, const string &filename) {
    ifstream file(filename);
    if (!file) {
        cout << "No previous data found. Starting fresh.\n";
        return;
    }

    students.clear();
    string line;

    while (getline(file, line)) {
        Student s;
        size_t pos = 0;
        size_t nextPos;
        vector<string> tokens;

        while ((nextPos = line.find(',', pos)) != string::npos) {
            tokens.push_back(line.substr(pos, nextPos - pos));
            pos = nextPos + 1;
        }
        tokens.push_back(line.substr(pos));

        if (tokens.size() == 6) {
            s.name = tokens[0];
            s.score1 = stod(tokens[1]);
            s.score2 = stod(tokens[2]);
            s.score3 = stod(tokens[3]);
            s.average = stod(tokens[4]);
            s.grade = tokens[5][0];
            students.push_back(s);
        }
    }

    file.close();
    cout << "Data loaded from " << filename << " successfully.\n";
}
