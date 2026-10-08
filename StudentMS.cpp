#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>
#include <string>

using namespace std;

const string DATA_FILE = "students.txt";
const char SEPARATOR = '|';

struct Student
{
    string name;
    string rollNo;
    string course;
    string email;
    string phone;
    string address;
};

// ---------- Validation Functions ----------

bool isValidRoll(const string& rollNo)
{
    regex pattern("^[A-Za-z0-9]{3,20}$");
    return regex_match(rollNo, pattern);
}

bool isValidEmail(const string& email)
{
    regex pattern("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    return regex_match(email, pattern);
}

bool isValidPhone(const string& phone)
{
    regex pattern("^[0-9]{10}$");
    return regex_match(phone, pattern);
}

// ---------- Input Function ----------

string getInput(const string& message)
{
    string value;

    cout << message;
    getline(cin, value);

    return value;
}

Student readStudent()
{
    Student student;

    student.name = getInput("Enter Name: ");

    do
    {
        student.rollNo = getInput("Enter Roll Number: ");

        if (!isValidRoll(student.rollNo))
        {
            cout << "Invalid roll number. Use 3-20 letters or numbers.\n";
        }

    } while (!isValidRoll(student.rollNo));

    student.course = getInput("Enter Course: ");

    do
    {
        student.email = getInput("Enter Email: ");

        if (!isValidEmail(student.email))
        {
            cout << "Invalid email address. Try again.\n";
        }

    } while (!isValidEmail(student.email));

    do
    {
        student.phone = getInput("Enter Contact Number: ");

        if (!isValidPhone(student.phone))
        {
            cout << "Contact number must contain exactly 10 digits.\n";
        }

    } while (!isValidPhone(student.phone));

    student.address = getInput("Enter Address: ");

    return student;
}

// ---------- File Conversion ----------

string studentToLine(const Student& student)
{
    return student.name + SEPARATOR +
           student.rollNo + SEPARATOR +
           student.course + SEPARATOR +
           student.email + SEPARATOR +
           student.phone + SEPARATOR +
           student.address;
}

bool lineToStudent(const string& line, Student& student)
{
    stringstream stream(line);
    vector<string> fields;
    string value;

    while (getline(stream, value, SEPARATOR))
    {
        fields.push_back(value);
    }

    if (fields.size() != 6)
    {
        return false;
    }

    student.name = fields[0];
    student.rollNo = fields[1];
    student.course = fields[2];
    student.email = fields[3];
    student.phone = fields[4];
    student.address = fields[5];

    return true;
}

// ---------- Load and Save ----------

vector<Student> loadStudents()
{
    vector<Student> students;

    ifstream file(DATA_FILE);

    if (!file)
    {
        return students;
    }

    string line;
    Student student;

    while (getline(file, line))
    {
        if (lineToStudent(line, student))
        {
            students.push_back(student);
        }
    }

    file.close();

    return students;
}

void saveStudents(const vector<Student>& students)
{
    ofstream file(DATA_FILE);

    for (const Student& student : students)
    {
        file << studentToLine(student) << '\n';
    }

    file.close();
}

// ---------- Display ----------

void showStudent(const Student& student, int number)
{
    cout << "\n----------------------------------------\n";
    cout << "Student " << number << '\n';
    cout << "----------------------------------------\n";
    cout << "Name       : " << student.name << '\n';
    cout << "Roll No.   : " << student.rollNo << '\n';
    cout << "Course     : " << student.course << '\n';
    cout << "Email      : " << student.email << '\n';
    cout << "Contact    : " << student.phone << '\n';
    cout << "Address    : " << student.address << '\n';
}

void displayStudents()
{
    vector<Student> students = loadStudents();

    if (students.empty())
    {
        cout << "\nNo student records available.\n";
        return;
    }

    cout << "\n========== STUDENT RECORDS ==========\n";

    for (int i = 0; i < static_cast<int>(students.size()); i++)
    {
        showStudent(students[i], i + 1);
    }
}

// ---------- Add Student ----------

void addStudent()
{
    vector<Student> students = loadStudents();

    cout << "\n========== ADD STUDENT ==========\n";

    Student student = readStudent();

    for (const Student& existing : students)
    {
        if (existing.rollNo == student.rollNo)
        {
            cout << "\nThis roll number already exists.\n";
            return;
        }
    }

    students.push_back(student);
    saveStudents(students);

    cout << "\nStudent added successfully.\n";
}

// ---------- Find Student ----------

int findStudent(const vector<Student>& students, const string& rollNo)
{
    for (int i = 0; i < static_cast<int>(students.size()); i++)
    {
        if (students[i].rollNo == rollNo)
        {
            return i;
        }
    }

    return -1;
}

// ---------- Search ----------

void searchStudent()
{
    vector<Student> students = loadStudents();

    if (students.empty())
    {
        cout << "\nNo records available.\n";
        return;
    }

    cout << "\n========== SEARCH STUDENT ==========\n";

    string rollNo = getInput("Enter Roll Number: ");

    int index = findStudent(students, rollNo);

    if (index == -1)
    {
        cout << "\nStudent not found.\n";
        return;
    }

    showStudent(students[index], 1);
}

// ---------- Update ----------

void updateStudent()
{
    vector<Student> students = loadStudents();

    if (students.empty())
    {
        cout << "\nNo records available.\n";
        return;
    }

    cout << "\n========== UPDATE STUDENT ==========\n";

    string rollNo = getInput("Enter Roll Number to update: ");

    int index = findStudent(students, rollNo);

    if (index == -1)
    {
        cout << "\nStudent not found.\n";
        return;
    }

    cout << "\nCurrent Information:";
    showStudent(students[index], 1);

    cout << "\nEnter New Information:\n";

    Student updated = readStudent();

    for (int i = 0; i < static_cast<int>(students.size()); i++)
    {
        if (i != index && students[i].rollNo == updated.rollNo)
        {
            cout << "\nThat roll number belongs to another student.\n";
            return;
        }
    }

    students[index] = updated;
    saveStudents(students);

    cout << "\nStudent information updated successfully.\n";
}

// ---------- Delete ----------

void deleteStudent()
{
    vector<Student> students = loadStudents();

    if (students.empty())
    {
        cout << "\nNo records available.\n";
        return;
    }

    cout << "\n========== DELETE STUDENT ==========\n";

    string rollNo = getInput("Enter Roll Number to delete: ");

    int index = findStudent(students, rollNo);

    if (index == -1)
    {
        cout << "\nStudent not found.\n";
        return;
    }

    showStudent(students[index], 1);

    char choice;

    cout << "\nDelete this record? (Y/N): ";
    cin >> choice;
    cin.ignore();

    if (choice != 'Y' && choice != 'y')
    {
        cout << "\nDeletion cancelled.\n";
        return;
    }

    students.erase(students.begin() + index);
    saveStudents(students);

    cout << "\nStudent record deleted successfully.\n";
}

// ---------- Menu ----------

void showMenu()
{
    cout << "\n\n";
    cout << "============================================\n";
    cout << "         STUDENT MANAGEMENT SYSTEM\n";
    cout << "============================================\n";
    cout << "1. Add Student\n";
    cout << "2. Display Students\n";
    cout << "3. Search Student\n";
    cout << "4. Update Student\n";
    cout << "5. Delete Student\n";
    cout << "6. Exit\n";
    cout << "============================================\n";
}

// ---------- Main ----------

int main()
{
    int choice;

    do
    {
        showMenu();

        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
            case 1:
                addStudent();
                break;

            case 2:
                displayStudents();
                break;

            case 3:
                searchStudent();
                break;

            case 4:
                updateStudent();
                break;

            case 5:
                deleteStudent();
                break;

            case 6:
                cout << "\nThank you for using the Student Management System.\n";
                break;

            default:
                cout << "\nInvalid choice. Please select 1-6.\n";
        }

    } while (choice != 6);

    return 0;
}