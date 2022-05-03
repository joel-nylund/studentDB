# studentDB
A student database made for a class on programming in C

by running the main function of the program, it expects user input, corresponding to commands that can be given.

the student struct contains a 6-char string for student ID, a string for last name, a string for first name, and an array of 7 positive integers. The array represents the scores a student has gotten through 6 rounds of a course, and the 7th int represents the sum of these scores.

After a command has been given, it will print "SUCCESS" if the command was successful. If something went wrong, such as invalid user input, it will print an error message to the standard output stream.

The commands are as follows:

Add student to database, the student's id should be between 1 and 6 characters:
  A <student-id> <student-surname> <student-first-name>
  example: A 123456 Doe John
  
Update student's score the round is an integer between 1 and 6, and score has to be a positive integer:
  U <student-id> <round> <score>
  example: U 123456 1 5
  
List/print all students and their scores to the standard output stream:
  L
  
Write database to file:
  W <filename>
  example: W studentDB.txt
  
Load database from file (overwrites current database):
  O <filename>
  example: O studentDB.txt
