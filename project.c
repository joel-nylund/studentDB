#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief allocates memory and adds student to database
 * 
 * \param stud_list database
 * \param id id of student to be added to array
 * \param surname surname of student to be added to array
 * \param name name of student to be added to array
 * 
 * \return 1 on success, 0 on failure
 */

int add(db* stud_list, char* id, char* surname, char* name) {
    // reallocate memory for another student
    if (id == NULL || surname == NULL || name == NULL) return 0;
    if (stud_list->numStudents == 0) stud_list->students = malloc(sizeof(Student));
    else stud_list->students = realloc(stud_list->students, sizeof(Student) * (stud_list->numStudents + 1));
    // check that it worked out
    if (stud_list->students == NULL) return 0;
    // pointer to student to be added
    Student* s = &stud_list->students[stud_list->numStudents];
    // allocate memory for student names
    s->fname = malloc(strlen(name) + 1);
    s->lname = malloc(strlen(surname) + 1);
    // copy data
    strcpy(s->fname, name);
    strcpy(s->id, id);
    strcpy(s->lname, surname);
    // set points to 0
    for (int i = 0; i < 7; i++) {
        s->points[i] = 0;
    }
    stud_list->numStudents++;


    // SUCCESS!
    return 1;
}

/**
 * \brief updates the exercise score of a student in the database
 * 
 * \param stud_list database
 * \param id id of student in the array whose score is to be changed
 * \param round selects the exercise to change the score of (1 - 6)
 * \param points points for the selected round
 * 
 * \return 1 on success, 0 on failure
 */

int update(db* stud_list, char* id, int round, unsigned int points) {
    // current index
    unsigned int index = 0;
    while (index < stud_list->numStudents) {
        // pointer to current student
        Student* s = &stud_list->students[index];
        // check if current index contains the correct student
        if (strcmp(id, s->id) == 0) {
            // once correct student is found, set the points for the round.
            s->points[round - 1] = points;
            // calculate the new score sum
            unsigned int sum = 0;
            for (int i = 0; i < 6; i++) {
                sum += s->points[i];
            }
            // add updated sum to end of point array
            s->points[6] = sum;
            // SUCCESS
            return 1;
        } else {
            // increase index
            index++;
        }
    }
    return 0;


}

/**
 * \brief prints all students' info to standard output stream in descending order according their total exercise scores
 *        NOTE: while it prints the student info in a specific order, it does not change the order in the database
 * 
 * \param stud_list student database.
 * 
 * \return nothing
 */

void list(db* stud_list) {
    // loop through students and order them
    int order[stud_list->numStudents];
    // initialize values to point sum
    for (unsigned int i = 0; i < stud_list->numStudents; i++) {
        order[i] = i;
    }

    // temporary holders
    int ordtemp;
    /* 
    *  A simple bubblesort algorhithm. for a project of this size the bubblesort works fine.
    *  If the database was to includes hundreds of students, a quicksort algorhithm might be more suitable,
    *  but for the purposes of this assignment, that would overkill, and may even be slower on arrays this size.
    */ 
    for (unsigned int i = 0; i < stud_list->numStudents - 1; i++) {
        for (unsigned int j = 0; j < (stud_list->numStudents - i - 1); j++) {
            unsigned int pointa, pointb;
            // points of j and j+1
            pointa = stud_list->students[order[j]].points[6];
            pointb = stud_list->students[order[j+1]].points[6];

            // see if the values should swap
            if (pointa < pointb) {
                // swap them around
                ordtemp = order[j+1];
                order[j+1] = order[j];
                order[j] = ordtemp;
            }
        }
    }
        

    for (unsigned int i = 0; i < stud_list->numStudents; i++) {
        // pointer to current student (student chosen from database by the order array)
        Student* s = &stud_list->students[order[i]];
        // print <id> <surname> <name>
        printf("%s %s %s ", s->id, s->lname, s->fname);
        // print scores
        for (int j = 0; j < 6; j++) {
            printf("%u ", s->points[j]);
        }
        // print sum and newline
        printf("%u\n", s->points[6]);
    }
}

/**
 * \brief writes student info to the given txt file
 *        NOTE: Does not order them according to any particular metric. Simply writes them to the file
 *              in whatever order they were added to the database (Hence the output discrepency on the A+ evaluator)
 * 
 * \param stud_list database containing all students and their info to be printed to the file
 * \param filename name of the file the student info should be written to
 * 
 * \return 1 on success, 0 on failure
 */

int write(db* stud_list, char* filename) {
    // file pointer
    FILE* fp = NULL;
    // open file for writing
    fp = fopen(filename, "w");
    if (fp == NULL) return 0;

    for (unsigned int i = 0; i < stud_list->numStudents; i++) {
        Student* s = &stud_list->students[i];
        // format <student-number> <surname> <name> <exercise-1> ... <exercise-6> <total-points>
        // print strings
        fprintf(fp, "%s %s %s ", s->id, s->lname, s->fname);
        // print exercise scores
        for (int j = 0; j < 6; j++) {
            fprintf(fp, "%u ", s->points[j]);
        }
        // print sum and newline
        fprintf(fp, "%u\n", s->points[6]);
    }

    fclose(fp);
    return 1;
}

/**
 * \brief loads student info from txt file and adds it to database
 * 
 * \param stud_list array to write the students info to
 * \param filename name of the file for data to be loaded from
 * 
 * \return 1 on success, 0 on failure
 */


int load(db* stud_list, char* filename) {
    // file pointer
    FILE* fp = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL) return 0;

    freeStudents(stud_list);
    
    // for holding values
    char id[7];
    char surname[100];
    char name[100];
    unsigned int points[7];

    while (fscanf(fp,   "%s %s %s %u %u %u %u %u %u %u\n",
                        id, surname, name,
                        &points[0], &points[1], &points[2], &points[3], &points[4], &points[5], &points[6]) != EOF) {
        // inside the while loop starts here
        // add new student to database
        if (id == NULL || surname == NULL || name == NULL) break;
        add(stud_list, id, surname, name);
        // update student with points
        for (int i = 0; i < 6; i++) {
            update(stud_list, id, i+1, points[i]);
        }

    }

    fclose(fp);

    return 1;
}






/**
 * \brief Parses command given by user
 * 
 * \param stud_list array of students
 * \param buf one line read from the standard input stream
 * \return 1 for all cases except quit/exit
 */
int doCommand(db* stud_list, char* buf) {
    // which command (function) to call
    char command = buf[0];
    // check which of the functions it should call
    switch (command) {
        // add function
        case 'A': ;
            // local variables for storing stuff
            char aid[100];
            char surname[1000];
            char name[1000];
            // scan the input and make sure it has the right arguements
            if (sscanf(buf, "%*c%s%s%s", aid, surname, name) == 3) {
                // check for errors
                if (strlen(aid) > 6) {
                    printf("Student number %s cannot must be more than 6 digits.\n", aid);
                    break;
                }
                int con = 0;
                for (unsigned int i = 0; i < stud_list->numStudents; i++) {
                    if (strcmp(aid, stud_list->students[i].id) == 0) con = 1;
                }
                if (con == 1) printf("Student %s is already in the database.\n", aid);

                // if it checks out, call the add function with the gathered arguements
                else if (add(stud_list, aid, surname, name) == 1 && con == 0) printf("SUCCESS\n");
                // if add fails, let the user know
                else printf("Error adding student %s %s to database.\n", surname, name);
                break;
            } else {
                printf("A should be followed by exactly 3 arguments.\n");
                break;
            }

        // update function
        case 'U': ;
            // variables
            char uid[100];
            int round;
            int points;
            // scan the input and make sure it has the right arguements
            if (sscanf(buf, "%*c%s%d%d", uid, &round, &points) == 3) {
                // fail conditions
                int con = 0;
                for (unsigned int i = 0; i < stud_list->numStudents; i++) {
                    if (strcmp(uid, stud_list->students[i].id) == 0) con = 1;
                }
                if (strlen(uid) > 6 || con == 0) printf("Student %s is not in the database.\n", uid);
                else if (round < 1 || round > 6) printf("Round cannot be less than 1 or larger than 6\n");
                else if (points < 0) printf("Student cannot have negative points.\n");
                // call the function and check for success
                else if (update(stud_list, uid, round, points) == 1) printf("SUCCESS\n");
                // inform user in case of error
                else printf("Error updating score of student. make sure the ID is correct and that the round is within the margins (1 - 6)\n");
                break;
            } else {
                // if arguements invalid, let user know
                printf("U should be followed by exactly 3 arguments.\n");
                break;
            }

        // List/Print function
        case 'L':
            list(stud_list);
            printf("SUCCESS\n");
            break;
        // Write function
        
        case 'W': ;
            // variable
            char wfilename[100];
            // scan the input and make sure it has the right arguements
            if (sscanf(buf, "%*c%s", wfilename) == 1) {
                // write to file
                if (write(stud_list, wfilename) == 1) printf("SUCCESS\n");
                // if failed, print an error message
                else printf("Cannot open file %s for writing.\n", wfilename);
                break;
            } else {
                // if arguements invalid, let user know
                printf("Command arguements invalid. Format: \"W <filename>\"\n");
                break;
            }
        
        // Open/Load function
        case 'O': ;
            // variable
            char ofilename[100];
            // scan the input and make sure it has the right arguements
            if (sscanf(buf, "%*c%s", ofilename) == 1) {
                if (load(stud_list, ofilename) == 1) printf("SUCCESS\n");
                else printf("Cannot open file %s for reading.\n", ofilename);
                break;
            } else {
                printf("Command arguements invalid. Format: \"O <filename>\"\n");
                break;
            }
        
        // Exit program
        case 'Q':
            return 0;
            break;
        // invalid command
        default:
            printf("Invalid command %c\n", command);
            break;
    }
    return 1;
}


/**
 * \brief adds a student to the database
 * 
 * \param stud_list array of students to free memory of
 * 
 * \returns nothing
 */

void freeStudents(db* stud_list) {
    if (stud_list->students != NULL){
        // free memory allocated for names
    for (unsigned int i = 0; i < stud_list->numStudents; i++) {
        Student* s = &stud_list->students[i];
        free(s->fname);
        free(s->lname);
    }
    // free the db student list
    free (stud_list->students);
    }
    // no more students in the db, so set number of students to 0.
    stud_list->numStudents = 0;
}

/**
 * \brief main loop, takes user input, parses it through to doCommand function, repeats loop
 *  until doCommand returns 0. When it does, free the assigned memory of all students in the database
 *  with free function.
 * 
 */
int main() {
    // create database
    db student_list;
    student_list.students = NULL;
    student_list.numStudents = 0;
    
    // keep looping until user exits program
    int continue_loop = 1;
    while (continue_loop) {
        // buffer to take user input
        char buffer[1000];
        // read user input to buffer
        fgets(buffer, 1000, stdin);
        // do the thing. if user exits, continue_loop will be set to 0 and program will exit the loop
        continue_loop = doCommand(&student_list, buffer);
    }
    freeStudents(&student_list);
    printf("SUCCESS\n");
    return 0;
}