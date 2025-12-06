#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <sstream>
using namespace std;

struct Employee {
    string id;
    string employeename;
    double salary;
    string password;
};

struct Payslip {
    string empID;
    string name;
    double basic;
    double hra;
    double allowances;
    double gross;
    double pf;
    double tax;
    double net;
};

struct AttendanceRecord {
    string id;
    string name;
    string date;   
    char status;   
};

void addEmployee();
void listEmployees();
void searchEmployee();
void adminPanel();

bool loginEmployee(string &loggedEmployeeID);
void employeePanel(const string &employee);
void forgotPassword();
bool adminLogin();

Payslip calculatePayrollWithPresent(const Employee &emp, int presentDays);
void savePayslip(const Payslip &p);
void displayPayslip(const Payslip &p);
void payrollMenu();
void employeeGeneratePayslip(const string &employeeID);

bool isDuplicateID(string id);
bool isDuplicateName(string name);
bool isDuplicatePassword(string pass);

void markAttendanceForDate();
void viewAllAttendance();
void viewAttendanceForEmployee();
void editAttendance();
void deleteAttendance();
void adminAttendanceMenu();
void ensureAttendanceFileExists();
vector<AttendanceRecord> loadAllAttendance();
void saveAllAttendance(const vector<AttendanceRecord>& all);
void getAttendanceCountForMonth(const string &empID, const string &monthYYYYMM, int &present, int &absent);

bool isDuplicateID(string id) {
    ifstream file("employees.txt");
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find(',');
        if (p1 == string::npos) continue;
        string existingID = line.substr(0, p1);
        if (existingID == id)
            return true;
    }
    return false;
}

bool isDuplicateName(string name) {
    ifstream file("employees.txt");
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find(',');
        size_t p2 = line.find(',', p1 + 1);
        if (p1 == string::npos || p2 == string::npos) continue;
        string existingName = line.substr(p1 + 1, p2 - p1 - 1);
        if (existingName == name)
            return true;
    }
    return false;
}

bool isDuplicatePassword(string pass) {
    ifstream file("employees.txt");
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find(',');
        size_t p2 = line.find(',', p1 + 1);
        size_t p3 = line.find(',', p2 + 1);
        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;
        string existingPass = line.substr(p3 + 1);
        if (existingPass == pass)
            return true;
    }
    return false;
}

void addEmployee() {
    Employee emp;
    cout << "\n=== Add Employee ===\n";

    cout << "\nEnter Employee ID: ";
    cin >> emp.id;

    if (isDuplicateID(emp.id)) {
        cout << "\n❌ Duplicate Employee ID. Cannot add.\n";
        return;
    }

    cout << "Enter Employee Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, emp.employeename);

    cout << "Enter Salary: ";
    cin >> emp.salary;

    cout << "Enter Password: ";
    cin >> emp.password;

    if (isDuplicatePassword(emp.password)) {
        cout << "\n❌ Password already used. Enter unique password.\n";
        return;
    }

    ofstream file("employees.txt", ios::app);
    if (!file) {
        cout << "Error opening employees.txt for writing.\n";
        return;
    }
    file << emp.id << "," << emp.employeename << "," << emp.salary << "," << emp.password << endl;

    cout << "✔ Employee added successfully!\n";
}

void listEmployees() {
    ifstream file("employees.txt");
    if (!file) {
        cout << "No employee records found.\n";
        return;
    }

    vector<Employee> employees;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find(',');
        size_t p2 = line.find(',', p1 + 1);
        size_t p3 = line.find(',', p2 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos)
            continue;

        Employee emp;
        emp.id = line.substr(0, p1);
        emp.employeename = line.substr(p1 + 1, p2 - p1 - 1);
        try {
            emp.salary = stod(line.substr(p2 + 1, p3 - p2 - 1));
        } catch (...) {
            emp.salary = 0.0;
        }
        emp.password = line.substr(p3 + 1);

        employees.push_back(emp);
    }

    if (employees.empty()) {
        cout << "No employees found.\n";
        return;
    }

    int choice;
    do {
        cout << "\n=== Employee List ===\n\n";
        cout << left << setw(5) << "No"
             << setw(10) << "ID"
             << setw(20) << "Employee Name"
             << setw(12) << "Salary"
             << "Password\n";
        cout << "--------------------------------------------------------------------------\n";

        for (size_t i = 0; i < employees.size(); ++i) {
            cout << left << setw(5) << i + 1
                 << setw(10) << employees[i].id
                 << setw(20) << employees[i].employeename
                 << setw(12) << employees[i].salary
                 << employees[i].password << endl;
        }

        cout << "\nEnter employee number to manage (0 to go back): ";
        cin >> choice;

        if (choice == 0) break;

        if (choice < 1 || choice > (int)employees.size()) {
            cout << "Invalid choice.\n";
            continue;
        }

        int action;
        cout << "\nChoose action for " << employees[choice - 1].employeename << ":\n";
        cout << "1 = Edit Employee ID\n";
        cout << "2 = Edit Employee Name\n";
        cout << "3 = Edit Salary\n";
        cout << "4 = Edit Password\n";
        cout << "5 = Delete Employee\n";
        cout << "0 = Cancel\n";
        cout << "Enter choice: ";
        cin >> action;

        switch (action) {
            case 1:
                cout << "Enter new Employee ID: ";
                cin >> employees[choice - 1].id;
                break;

            case 2:
                cout << "Enter new Employee Name: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, employees[choice - 1].employeename);
                break;

            case 3:
                cout << "Enter new Salary: ";
                cin >> employees[choice - 1].salary;
                break;

            case 4:
                cout << "Enter new Password: ";
                cin >> employees[choice - 1].password;
                break;

            case 5:
                employees.erase(employees.begin() + (choice - 1));
                cout << "Employee deleted.\n";
                break;

            default:
                cout << "Invalid action.\n";
        }

        ofstream out("employees.txt");
        if (!out) {
            cout << "Error writing employees.txt\n";
            return;
        }
        for (auto &e : employees)
            out << e.id << "," << e.employeename << "," << e.salary << "," << e.password << endl;

    } while (true);
}

void searchEmployee() {
    ifstream file("employees.txt");
    if (!file) {
        cout << "No employee records found.\n";
        return;
    }

    string keyword;
    cout << "\n=== Search Employee ===\n";
    cout << "\nEnter Employee ID or Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, keyword);

    bool found = false;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find(',');
        size_t p2 = line.find(',', p1 + 1);
        size_t p3 = line.find(',', p2 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;

        string id = line.substr(0, p1);
        string name = line.substr(p1 + 1, p2 - p1 - 1);
        double salary = 0;
        try { salary = stod(line.substr(p2 + 1, p3 - p2 - 1)); } catch(...) { salary = 0; }

        string idLower = id, nameLower = name, keyLower = keyword;
        transform(idLower.begin(), idLower.end(), idLower.begin(), ::tolower);
        transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
        transform(keyLower.begin(), keyLower.end(), keyLower.begin(), ::tolower);

        if (idLower.find(keyLower) != string::npos ||
            nameLower.find(keyLower) != string::npos) {

            cout << "\nEmployee Found:\n";
            cout << "ID: " << id << endl;
            cout << "Name: " << name << endl;
            cout << "Salary: " << salary << endl;
            found = true;
        }
    }

    if (!found) cout << "No matching employee found.\n";
}

void ensureAttendanceFileExists() {
    ifstream f("attendance.txt");
    if (!f) {
        ofstream o("attendance.txt");
        o.close();
    }
}

vector<AttendanceRecord> loadAllAttendance() {
    vector<AttendanceRecord> all;
    ifstream file("attendance.txt");
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        vector<string> parts;
        string token;
        stringstream ss(line);
        while (getline(ss, token, ',')) parts.push_back(token);
        if (parts.size() != 4) continue;
        AttendanceRecord a;
        a.id = parts[0];
        a.name = parts[1];
        a.date = parts[2];
        if (!parts[3].empty()) a.status = parts[3][0];
        else a.status = 'A';
        all.push_back(a);
    }
    return all;
}

void saveAllAttendance(const vector<AttendanceRecord>& all) {
    ofstream out("attendance.txt");
    for (auto &a : all) {
        out << a.id << "," << a.name << "," << a.date << "," << a.status << "\n";
    }
}

void getAttendanceCountForMonth(const string &empID, const string &monthYYYYMM, int &present, int &absent) {
    present = 0; absent = 0;
    vector<AttendanceRecord> all = loadAllAttendance();
    for (auto &a : all) {
        if (a.id != empID) continue;
        if (a.date.size() >= 7 && a.date.substr(0,7) == monthYYYYMM) {
            if (a.status == 'P' || a.status == 'p') present++;
            else absent++;
        }
    }
}

void markAttendanceForDate() {
    ensureAttendanceFileExists();
    cout << "\n=== Mark Attendance for Date ===\n";
    string date;
    cout << "Enter date (YYYY-MM-DD): ";
    cin >> date;

    ifstream fin("employees.txt");
    if (!fin) {
        cout << "employees.txt not found!\n";
        return;
    }

    vector<Employee> employees;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find(',');
        size_t p2 = line.find(',', p1+1);
        size_t p3 = line.find(',', p2+1);
        if (p1==string::npos || p2==string::npos || p3==string::npos) continue;
        Employee e;
        e.id = line.substr(0,p1);
        e.employeename = line.substr(p1+1, p2-p1-1);
        try { e.salary = stod(line.substr(p2+1, p3-p2-1)); } catch(...) { e.salary = 0; }
        e.password = line.substr(p3+1);
        employees.push_back(e);
    }
    fin.close();

    vector<AttendanceRecord> all = loadAllAttendance();

    cout << "\nMark P for Present, A for Absent. (Enter P or A)\n";
    for (auto &e : employees) {
        char st;
        cout << "Employee " << e.id << " - " << e.employeename << ": ";
        cin >> st;
        st = toupper(st);
        if (st != 'P' && st != 'A') st = 'A'; 

        AttendanceRecord a;
        a.id = e.id;
        a.name = e.employeename;
        a.date = date;
        a.status = st;
        all.push_back(a);
    }

    saveAllAttendance(all);
    cout << "\n✔ Attendance saved \n";
}

void viewAllAttendance() {
    ensureAttendanceFileExists();
    vector<AttendanceRecord> all = loadAllAttendance();
    if (all.empty()) {
        cout << "No attendance records found.\n";
        return;
    }

    cout << "\n=== All Attendance Records ===\n";
    cout << left << setw(10) << "ID" << setw(25) << "Name" << setw(12) << "Date" << setw(8) << "Status\n";
    cout << "-----------------------------------------------------------\n";
    for (auto &a : all) {
        cout << left << setw(10) << a.id << setw(25) << a.name << setw(12) << a.date << setw(8) << a.status << "\n";
    }
}

void viewAttendanceForEmployee() 
{
     ensureAttendanceFileExists(); 
     string id; 
     cout << "\nEnter Employee ID: ";
     cin >> id;
     vector<AttendanceRecord> all = loadAllAttendance();
   bool found = false;
      cout << "\nAttendance for " << id << ":\n";
     cout << left << setw(12) << "Date" << setw(8) << "Status\n";
  cout << "-------------------------\n";
      for (auto &a : all) { if (a.id == id) 
        { 
      cout << left << setw(12) << a.date << setw(8) << a.status << "\n";
     found = true;
     }
     }
      if (!found) 
      cout << "No attendance records found for this employee.\n";
     }

void editAttendance() {
    ensureAttendanceFileExists();
    vector<AttendanceRecord> all = loadAllAttendance();
    if (all.empty()) {
        cout << "No attendance to edit.\n";
        return;
    }

    string id, date;
    cout << "\nEnter Employee ID for edit: ";
    cin >> id;
    cout << "Enter Date (YYYY-MM-DD) to edit: ";
    cin >> date;

    bool found = false;
    for (auto &a : all) {
        if (a.id == id && a.date == date) {
            cout << "Found: " << a.id << ", " << a.name << ", " << a.date << ", " << a.status << "\n";
            char newStatus;
            cout << "Enter new status (P/A): ";
            cin >> newStatus;
            newStatus = toupper(newStatus);
            if (newStatus != 'P' && newStatus != 'A') {
                cout << "Invalid status. Aborting edit.\n";
                return;
            }
            a.status = newStatus;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Record not found.\n";
        return;
    }

    saveAllAttendance(all);
    cout << "Attendance updated.\n";
}

void deleteAttendance() {
    ensureAttendanceFileExists();
    vector<AttendanceRecord> all = loadAllAttendance();
    if (all.empty()) {
        cout << "No attendance to delete.\n";
        return;
    }

    string id, date;
    cout << "\nEnter Employee ID to delete record for: ";
    cin >> id;
    cout << "Enter Date (YYYY-MM-DD) to delete: ";
    cin >> date;

    auto it = remove_if(all.begin(), all.end(), [&](const AttendanceRecord &a){
        return a.id == id && a.date == date;
    });

    if (it == all.end()) {
        cout << "No matching record found.\n";
        return;
    }

    all.erase(it, all.end());
    saveAllAttendance(all);
    cout << "Record(s) deleted.\n";
}

void adminAttendanceMenu() {
    int ch;
    do {
        cout << "\n=== Attendance Menu (Admin) ===\n";
        cout << "1. Mark Attendance for Date\n";
        cout << "2. View All Attendance\n";
        cout << "3. View Attendance for Employee\n";
        cout << "4. Edit Attendance Record\n";
        cout << "5. Delete Attendance Record\n";
        cout << "6. Back\n";
        cout << "Enter choice: ";
        cin >> ch;

        switch (ch) {
            case 1: markAttendanceForDate(); break;
            case 2: viewAllAttendance(); break;
            case 3: viewAttendanceForEmployee(); break;
            case 4: editAttendance(); break;
            case 5: deleteAttendance(); break;
            case 6: break;
            default: cout << "Invalid choice.\n";
        }
    } while (ch != 6);
}

Payslip calculatePayrollWithPresent(const Employee &emp, int presentDays) {
    Payslip p;
    p.empID = emp.id;
    p.name = emp.employeename;

    double perDay = emp.salary / 30.0;
    p.basic = perDay * presentDays; 

    p.hra = p.basic * 0.20;
    p.allowances = p.basic * 0.10;
    p.gross = p.basic + p.hra + p.allowances;
    p.pf = p.basic * 0.12;

    if (p.gross <= 25000)
        p.tax = 0;
    else if (p.gross <= 50000)
        p.tax = p.gross * 0.05;
    else
        p.tax = p.gross * 0.10;

    p.net = p.gross - p.pf - p.tax;
    return p;
}

void savePayslip(const Payslip &p) {
    ofstream file("payslip.txt", ios::app);
    if (!file) {
        cout << "Error opening payslip.txt for writing.\n";
        return;
    }
    file << p.empID << "," << p.name << ","
         << fixed << setprecision(2) << p.basic << ","
         << p.hra << ","
         << p.allowances << ","
         << p.gross << ","
         << p.pf << ","
         << p.tax << ","
         << p.net << "\n";
    file.close();
}

void displayPayslip(const Payslip &p) {
    cout << "\n========== Payslip ==========\n";
    cout << "Employee ID: " << p.empID << endl;
    cout << "Name: " << p.name << endl;
    cout << fixed << setprecision(2);
    cout << "Basic Salary (Payable based on present days): " << p.basic << endl;
    cout << "HRA: " << p.hra << endl;
    cout << "Allowances: " << p.allowances << endl;
    cout << "Gross Salary: " << p.gross << endl;
    cout << "PF: " << p.pf << endl;
    cout << "Tax: " << p.tax << endl;
    cout << "Net Pay: " << p.net << endl;
    cout << "============================\n";
}

void generatePayrollForEmployeeID() {
    string id;
    cout << "Enter Employee ID to generate payroll for: ";
    cin >> id;

    string month;
    cout << "Enter month for payroll (YYYY-MM): ";
    cin >> month;

    ifstream fin("employees.txt");
    if (!fin) {
        cout << "employees.txt not found!\n";
        return;
    }

    string line;
    bool found = false;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find(',');
        size_t p2 = line.find(',', p1 + 1);
        size_t p3 = line.find(',', p2 + 1);
        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;

        string eid = line.substr(0, p1);
        if (eid != id) continue;

        Employee emp;
        emp.id = eid;
        emp.employeename = line.substr(p1 + 1, p2 - p1 - 1);
        try { emp.salary = stod(line.substr(p2 + 1, p3 - p2 - 1)); } catch(...) { emp.salary = 0; }
        emp.password = line.substr(p3 + 1);

        int present = 0, absent = 0;
        getAttendanceCountForMonth(emp.id, month, present, absent);

        Payslip p = calculatePayrollWithPresent(emp, present);
        cout << "\nPayroll for month: " << month << " (Present: " << present << ", Absent: " << absent << ")\n";
        displayPayslip(p);
        savePayslip(p);
        found = true;
        break;
    }
    fin.close();
    if (!found) cout << "Employee ID not found.\n";
}

void generatePayrollForAllEmployees() {
    string month;
    cout << "Enter month for payroll (YYYY-MM): ";
    cin >> month;

    ifstream fin("employees.txt");
    if (!fin) {
        cout << "employees.txt not found!\n";
        return;
    }

    string line;
    bool any = false;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find(',');
        size_t p2 = line.find(',', p1 + 1);
        size_t p3 = line.find(',', p2 + 1);
        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;

        Employee emp;
        emp.id = line.substr(0, p1);
        emp.employeename = line.substr(p1 + 1, p2 - p1 - 1);
        try { emp.salary = stod(line.substr(p2 + 1, p3 - p2 - 1)); } catch(...) { emp.salary = 0; }
        emp.password = line.substr(p3 + 1);

        int present = 0, absent = 0;
        getAttendanceCountForMonth(emp.id, month, present, absent);

        Payslip p = calculatePayrollWithPresent(emp, present);
        cout << "\nPayroll for " << emp.employeename << " (ID: " << emp.id << ") Month: " << month << " (P:" << present << ", A:" << absent << ")\n";
        displayPayslip(p);
        savePayslip(p);
        any = true;
    }
    fin.close();
    if (!any) cout << "No employees found to generate payroll.\n";
}

void payrollMenu() {
    int choice;
    do {
        cout << "\n===== Payroll Menu =====\n";
        cout << "1. Generate payroll for an Employee (by ID)\n";
        cout << "2. Generate payroll for ALL employees\n";
        cout << "3. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: generatePayrollForEmployeeID(); break;
            case 2: generatePayrollForAllEmployees(); break;
            case 3: break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 3);
}

bool loginEmployee(string &loggedEmployeeID) {
    string inputID, inputName, inputPass;
    cout << "\n=== Employee Login ===\n";

    cout << "\nEnter Employee ID: ";
    cin >> inputID;

    cout << "Enter Employee Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, inputName);

    cout << "Enter Password: ";
    cin >> inputPass;

    ifstream file("employees.txt");
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find(',');
        size_t p2 = line.find(',', p1 + 1);
        size_t p3 = line.find(',', p2 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;

        string id = line.substr(0, p1);
        string name = line.substr(p1 + 1, p2 - p1 - 1);
        string password = line.substr(p3 + 1);

        if (id == inputID && name == inputName && password == inputPass) {
            loggedEmployeeID = id;
            cout << "\n✔ Login successful! Welcome, " << name << "!\n";
            return true;
        }
    }

    cout << "\n❌ Login failed. Incorrect details.\n";
    return false;
}

void forgotPassword() {
    string name;
    cout << "\n=== Forgot Password ===\n";
    cout << "Enter your Employee Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    ifstream file("employees.txt");
    vector<Employee> employees;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find(',');
        size_t p2 = line.find(',', p1 + 1);
        size_t p3 = line.find(',', p2 + 1);
        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;

        Employee emp;
        emp.id = line.substr(0, p1);
        emp.employeename = line.substr(p1 + 1, p2 - p1 - 1);
        try { emp.salary = stod(line.substr(p2 + 1, p3 - p2 - 1)); } catch(...) { emp.salary = 0; }
        emp.password = line.substr(p3 + 1);
        employees.push_back(emp);
    }

    for (auto &e : employees) {
        if (e.employeename == name) {
            cout << "\nYour current password: " << e.password << endl;

            char ch;
            cout << "Change password? (y/n): ";
            cin >> ch;

            if (ch == 'y' || ch == 'Y') {
                cout << "Enter new password: ";
                cin >> e.password;

                ofstream out("employees.txt");
                if (!out) { cout << "Error writing employees.txt\n"; return; }
                for (auto &x : employees)
                    out << x.id << "," << x.employeename << "," << x.salary << "," << x.password << endl;

                cout << "✔ Password updated successfully!\n";
            }
            return;
        }
    }

    cout << "❌ Employee not found.\n";
}

bool adminLogin() {
    const string ADMIN_USER = "admin";
    const string ADMIN_PASS = "admin123";
    string u, p;

    cout << "\n=== Admin Login ===\n";
    cout << "Enter username: ";
    cin >> u;
    cout << "Enter password: ";
    cin >> p;

    if (u == ADMIN_USER && p == ADMIN_PASS) {
        cout << "✔ Admin login successful!\n";
        return true;
    }

    cout << "❌ Invalid admin credentials.\n";
    return false;
}

void employeePanel(const string &employee) {
    int choice;

    do {
        ifstream file("employees.txt");
        string line;

        string id, name, salary;

        while (getline(file, line)) {
            if (line.empty()) continue;
            size_t p1 = line.find(',');
            size_t p2 = line.find(',', p1 + 1);
            size_t p3 = line.find(',', p2 + 1);
            if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;

            id = line.substr(0, p1);
            name = line.substr(p1 + 1, p2 - p1 - 1);
            salary = line.substr(p2 + 1, p3 - p2 - 1);

            if (id == employee) break;
        }

        cout << "\n=== EMPLOYEE DASHBOARD ===\n";
        cout << "\n1. View Profile\n";
        cout << "2. Generate My Payslip (by month)\n";
        cout << "3. View My Attendance (all records)\n";
        cout << "4. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "\n--- Your Profile ---\n";
            cout << "ID: " << id << endl;
            cout << "Name: " << name << endl;
            cout << "Salary: " << salary << endl;
            break;

        case 2: {
            string month;
            cout << "Enter month (YYYY-MM): ";
            cin >> month;
            ifstream fin("employees.txt");
            string l;
            bool found = false;
            while (getline(fin, l)) {
                if (l.empty()) continue;
                size_t p1 = l.find(',');
                size_t p2 = l.find(',', p1 + 1);
                size_t p3 = l.find(',', p2 + 1);
                if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;
                string idd = l.substr(0,p1);
                if (idd != employee) continue;
                Employee emp;
                emp.id = idd;
                emp.employeename = l.substr(p1 + 1, p2 - p1 - 1);
                try { emp.salary = stod(l.substr(p2 + 1, p3 - p2 - 1)); } catch(...) { emp.salary = 0; }
                emp.password = l.substr(p3 + 1);

                int present = 0, absent = 0;
                getAttendanceCountForMonth(emp.id, month, present, absent);
                Payslip p = calculatePayrollWithPresent(emp, present);
                cout << "\nPayroll for month: " << month << " (Present: " << present << ", Absent: " << absent << ")\n";
                displayPayslip(p);
                savePayslip(p);
                found = true;
                break;
            }
            fin.close();
            if (!found) cout << "Employee record not found.\n";
            break;
        }

        case 3:

            {
                string id = employee;
                vector<AttendanceRecord> all = loadAllAttendance();
                bool any = false;
                cout << "\nAttendance records for " << id << ":\n";
                cout << left << setw(12) << "Date" << setw(8) << "Status\n";
                cout << "-------------------------\n";
                for (auto &a : all) {
                    if (a.id == id) {
                        cout << left << setw(12) << a.date << setw(8) << a.status << "\n";
                        any = true;
                    }
                }
                if (!any) cout << "No records found.\n";
            }
            break;

        case 4:
            cout << "\n✔ Logged out successfully!\n";
            return;

        default:
            cout << "Invalid choice.\n";
        }

    } while (choice != 4);
}

void adminPanel() {
    int choice;
    do {
        cout << "\n=========== ADMIN DASHBOARD ===========\n";
        cout << "\n1. Add Employee\n";
        cout << "2. View / Edit / Delete Employee\n";
        cout << "3. Search Employee\n";
        cout << "4. Payroll Menu\n";
        cout << "5. Attendance Menu\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: addEmployee(); break;
        case 2: listEmployees(); break;
        case 3: searchEmployee(); break;
        case 4: payrollMenu(); break;
        case 5: adminAttendanceMenu(); break;
        case 6: break;
        default: cout << "Invalid choice.\n";
        }

    } while (choice != 6);
}

int main() {
    int choice;
    string loggedEmployeeID;

    cout << "\n     --------------------------------------------";
    cout << "\n           PAYROLL MANAGEMENT SYSTEM";
    cout << "\n     --------------------------------------------";

    ensureAttendanceFileExists();

    do {
        cout << "\n\n1. Register Employee";
        cout << "\n2. Employee Login";
        cout << "\n3. Admin Login";
        cout << "\n4. Forgot Password";
        cout << "\n5. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: addEmployee(); break;
        case 2:
            if (loginEmployee(loggedEmployeeID))
                employeePanel(loggedEmployeeID);
            break;
        case 3:
            if (adminLogin())
                adminPanel();
            break;
        case 4: forgotPassword(); break;
        case 5: cout << "Exiting...\n"; break;
        default: cout << "Invalid choice.\n";
        }

    } while (choice != 5);

    return 0;
}
