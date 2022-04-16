#ifndef _PROJECT__H_
#define _PROJECT__H_


// student struct
typedef struct student_st Student;
struct student_st {
    char id[7];     // student ID
    char* fname;    // student first name
    char* lname;    // student last name
    unsigned int points[7]; // list of points for all 6 exercises and last for sum.
};

// db struct (makes it easier to parse through student list)
typedef struct database db;
struct database {
    Student* students;          // array of students
    unsigned int numStudents;   // keeps track of number of students currently in the db
};

// functions

int add(db* stud_list, char* id, char* surname, char* name);   // adds new student to database
int update(db* stud_list, char* id, int round, unsigned int points);   // update exercise round points of a student
void list(db* stud_list);     // prints database content to standard output stream
int write(db* stud_list, char* filename);    // saves database to file
int load(db* stud_list, char* filename);     // loads students from file to database (removes any students currently in db)
int doCommand(db* stud_list, char* buf);     // parses command and calls necessary functions
void freeStudents(db* stud_list);  // frees all assigned memory

int main(); // main function



#endif //! _PROJECT__H_