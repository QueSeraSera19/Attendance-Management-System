#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Student {
    string studentID;
    string name;
    string course;
    int rollNumber; 
};

class AttendanceSystem {
private:
    vector<Student> attendanceList;

    // Function to get current date and time
    string getCurrentDateTime() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
        return buffer;
    }

    // Function to check if a student is already marked present
    bool isStudentPresent(const string& studentID) {
        return find_if(attendanceList.begin(), attendanceList.end(),
            [studentID](const Student& entry) {
                return entry.studentID == studentID;
            }) != attendanceList.end();
    }

public:
    // Function to mark attendance
    void markAttendance(const Student& student) {
        string dateTime = getCurrentDateTime();

        // Check if the student has a valid student ID
        if (student.studentID.length() != 5) {
            cout << "Error: Invalid student ID. It should be exactly 5 characters long." << endl;
            return;
        }

        // Check if the student has a valid roll number
        if (student.rollNumber < 1 || student.rollNumber > 100) {
            cout << "Error: Invalid roll number. It should be between 1 and 100." << endl;
            return;
        }

        if (!isStudentPresent(student.studentID)) {
            attendanceList.push_back(student);
            cout << "Attendance marked for student ID " << student.studentID << " at " << dateTime << endl;
        } else {
            cout << "Attendance already marked for student ID " << student.studentID << endl;
        }
    }

    // Function to display attendance
    void displayAttendance() {
        cout << "Attendance Records:" << endl;
        cout << setw(15) << "Student ID" << setw(20) << "Name" << setw(20) << "Course" << setw(15) << "Roll Number" << endl;
        cout << string(70, '-') << endl;

        for (const Student& entry : attendanceList) {
            cout << setw(15) << entry.studentID << setw(20) << entry.name << setw(20) << entry.course << setw(15) << entry.rollNumber << endl;
        }
    }

    // Function to save attendance to a file
    void saveAttendanceToFile(const string& fileName) {
        ofstream file(fileName);

        for (const Student& entry : attendanceList) {
            file << entry.studentID << " " << entry.name << " " << entry.course << " " << entry.rollNumber << endl;
        }

        file.close();
        cout << "Attendance saved to " << fileName << endl;
    }

    // Function to load attendance from a file
    void loadAttendanceFromFile(const string& fileName) {
        ifstream file(fileName);

        if (!file.is_open()) {
            cerr << "Error: Unable to open file " << fileName << endl;
            return;
        }

        attendanceList.clear();

        Student entry;
        while (file >> entry.studentID >> entry.name >> entry.course >> entry.rollNumber) {
            attendanceList.push_back(entry);
        }

        file.close();
        cout << "Attendance loaded from " << fileName << endl;
    }
};

int main() {
    AttendanceSystem attendanceSystem;

    // Load existing attendance records from a file
    attendanceSystem.loadAttendanceFromFile("attendance_records.txt");

    while (true) {
        cout << "\nAutomated Attendance System" << endl;
        cout << "1. Mark Attendance" << endl;
        cout << "2. Display Attendance" << endl;
        cout << "3. Save and Exit" << endl;

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                Student student;
                cout << "Enter student ID (exactly 5 characters): ";
                cin >> student.studentID;
                cout << "Enter student name: ";
                cin.ignore(); // Ignore newline character left in the buffer
                getline(cin, student.name);
                cout << "Enter student course: ";
                getline(cin, student.course);
                cout << "Enter roll number (1-100): ";
                cin >> student.rollNumber;

                attendanceSystem.markAttendance(student);
                break;
            }
            case 2:
                attendanceSystem.displayAttendance();
                break;
            case 3:
                // Save attendance and exit
                attendanceSystem.saveAttendanceToFile("attendance_records.txt");
                cout << "Exiting the program." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
