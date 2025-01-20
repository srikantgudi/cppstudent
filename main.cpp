#include <iostream>
#include <ncurses.h>
#include <fstream>
#include <iomanip>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

struct Student {
    int rollno;
    char fname[30];
    char lname[30];
    char grade[10];
};

FILE *fd;
size_t curpos;

bool findRecord(Student & s) {
    Student st;
    long n = 0;
    fseek(fd, 0, SEEK_SET);
    fread((char *) &st, sizeof(st), 1, fd);
    while (!feof(fd)) {
        if (st.rollno == s.rollno) {
            curpos = ftell(fd)-sizeof(st);
            s = st;
            return true;
        }
        fread((char *) &st, sizeof(st), 1, fd);
    }
    return false;
}

void addNew() {
    Student s;
    clear();
    mvaddstr(2,20,"Add Student");
    mvaddstr(4,10,"Roll No: ");
    int n = scanw("%d", &s.rollno);
    if (n == -1) return;
    if (!s.rollno) return;
    if (findRecord(s)) {
        mvprintw(4,40,"Already existing...%ld curpos | rec size: %ld", curpos, sizeof(s));
        getch();
        return;
    } else {
        mvaddstr(4,40,"New record...");
    }
    mvaddstr(6,10,"First name: ");
    scanw("%s", s.fname);
    if (!strlen(s.fname)) return;
    mvaddstr(8,10,"Last name: ");
    scanw("%s", s.lname);
    if (!strlen(s.lname)) return;
    mvaddstr(10,10,"Grade: ");
    scanw("%s", s.grade);
    if (!strlen(s.grade)) return;
    mvprintw(15,10,"Roll no.: [%d] First name: [%s] Last name: [%s] Grade: [%s]", s.rollno, s.fname, s.lname, s.grade);
    refresh();
    fseek(fd, 0, SEEK_END);
    fwrite((char *) &s, sizeof(s), 1, fd);
    getch();
}

void listAll() {
    Student s;
    clear();
    printw("\nStudents List\n\n");
    fseek(fd, 0, SEEK_SET);
    fread((char *)&s, sizeof(s), 1, fd);
    while (!feof(fd)) {
        printw("%d | %-30s | %-30s | %-10s\n", s.rollno, s.fname, s.lname, s.grade);
        fread((char *)&s, sizeof(s), 1, fd);
    }
    getch();
}

void update() {
    Student s;
    Student oldst;
    clear();
    mvaddstr(1, 10, "UPDATE A RECORD");
    mvaddstr(4, 10,"Roll No: ");
    int n = scanw("%d", &s.rollno);
    if (n == -1) return;
    if (!s.rollno) return;
    if (findRecord(s)) {
        oldst = s;
        // fseek(fd, curpos, SEEK_SET);
        // fread((char *)&oldst, sizeof(oldst), 1, fd);
        mvprintw(3,10, "Roll.no: %d | First name: %s | Last name: %s | Grade: %s", oldst.rollno, oldst.fname, oldst.lname, oldst.grade);
        mvprintw(4,15, "(leave empty for no change)");
        mvaddstr(6,10,"First name: ");
        scanw("%s", s.fname);
        if (strlen(s.fname)) {
            strcpy(oldst.fname, s.fname);
        }
        mvprintw(6,40, "%s", oldst.fname);
        mvaddstr(8,10,"Last name: ");
        getnstr(s.lname, sizeof(s.lname));
        if (strlen(s.lname)) {
            strcpy(oldst.lname, s.lname);
        }
        mvprintw(8,40, "%s", oldst.lname);
        mvaddstr(10,10,"Grade: ");
        scanw("%s", s.grade);
        if (strlen(s.grade)) {
            strcpy(oldst.grade, s.grade);
        }
        mvprintw(10,40, "%s", oldst.grade);
        mvprintw(15,10,"old: Roll no.: [%d] First name: [%s] Last name: [%s] Grade: [%s]", oldst.rollno, oldst.fname, oldst.lname, oldst.grade);
        refresh();
        mvaddstr(16,10,"u. Update / c. Cancel");
        char ok = getch();
        refresh();
        if (ok == 'u') {
            fseek(fd,curpos,SEEK_SET);
            fwrite((char *)&oldst, sizeof(oldst), 1, fd);
        }
    } else {
        mvprintw(10, 10,"That record does not exist...Press a key to continue..");
        getch();
    }
}

int main() {
    fd = fopen("students.dat", "rw+");
    char option = '?';
    // StudentFile stFile;
    initscr();
    keypad(stdscr, true); 
    while (option != 'q') {
        clear();
        mvprintw(2, 10, "Student Management System\n");
        mvprintw(4, 10, "a. Add new\n");
        mvprintw(5, 10, "e. Edit\n");
        mvprintw(6, 10, "l. List all\n");
        mvprintw(7, 10, "q. Quit ? ");
        option = getch();
        switch (option) {
            case 'a': addNew(); break;
            case 'e': update(); break;
            case 'l': listAll(); break;
        }
    }
    refresh();
    endwin();
    fclose(fd);

    return 0;
} 
