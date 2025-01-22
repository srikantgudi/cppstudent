#include <iostream>
#include <ncurses.h>
#include <fstream>
#include <string.h>
#include <sstream>
#include <string.h>
#include <map>

using namespace std;

struct Student {
    int rollno;
    string fname;
    string lname;
    string grade;
};

class Students {
    string filename;
    
    bool findRecord(Student & s) {
        return records.find(s.rollno) != records.end();
    }

    public:
        map<int, Student> records;
        Students() {
            filename = "students.dat";
            cout << "\nReading from file\n";
            ifstream ifs(filename.c_str());
            Student s{};
            string line;
            while (ifs >> s.rollno >> s.fname >> s.lname >> s.grade) {
                records[s.rollno] = s;
            }
            ifs.close();
            cout << "\nFetched " << records.size() << " rows\n";
        }
        ~Students() {
            cout << "\nWriting to " << filename << ".. " << records.size() << " rows\n";
            ofstream ofs;
            map<int,Student>::iterator it;
            ofs.open(filename.c_str());
            for (it=records.begin(); it != records.end(); it++) {
                ofs << it->first << "\t" << it->second.fname << "\t" << it->second.lname << "\t" << it->second.grade << "\n";
            }
            ofs.close();
        }
        bool edit(Student & s) {
            cout << "First name: ";
            cin >> s.fname;
            if (s.fname.length() < 1) return false;
            cout << "Last name: ";
            cin >> s.lname;
            if (!s.lname.length()) return false;
            cout << "Grade: ";
            cin >> s.grade;
            if (!s.grade.length()) return false;

            return true;
        }
        void addNew() {
            Student s;
            cout <<"\nAdd Student\n";
            cout << "\nRoll No. (0 to exit): ";
            cin >> s.rollno;
            if (!s.rollno) return;
            if (findRecord(s)) {
                cout << "\nAlready existing...\n";
                return;
            } else {
                cout << "\nNew record...\n";
            }
            if (edit(s)) {
                records[s.rollno] = s;
            } else {
                cout << "\nIncomplete data...\n";
            }
        }

        void listAll() {
            Student s;
            cout << "\nStudents List\n\n";
            
            for (auto const s: records) {
                cout << s.first << " | " << s.second.fname << " | " << s.second.lname << " | " << s.second.grade << endl;
            }
        }

        void update() {
            Student s;
            cout << "\nUPDATE A RECORD\n";
            cout << "\nRoll No: ";
            cin >> s.rollno;
            if (!s.rollno) return;
            if (findRecord(s)) {
                if (edit(s)) {
                    records[s.rollno] = s;
                } else {
                    cout << "\nIncomplete data entered...\n";
                }
            } else {
                cout << "\nThat record does not exist...\n";
            }
            cin.get();
        }
};

int main() {
    char option = '?';
    Students students; 
    while (option != 'q') {
        cout << "\nStudent Management System :: " << students.records.size() << " rows\n";
        cout << "\na. Add new";
        cout << "\ne. Edit";
        cout << "\nl. List all";
        cout << "\nq. Quit ? ";
        cin >> option;
        switch (option) {
            case 'a': students.addNew(); break;
            case 'e': students.update(); break;
            case 'l': students.listAll(); break;
        }
    }

    return 0;
} 
