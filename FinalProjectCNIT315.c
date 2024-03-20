#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


typedef struct question{
    char question[1000];
    char optionA[1000];
    char optionB[1000];
    char optionC[1000];
    char optionD[1000];
    char answerLetter;
    struct question* next;
} question;

typedef struct student{
    char username[32];
    char password[32];
    float grade;
    int numOfQuizzes;
    struct student* next;
} student;

char answerKey[100];
char studentAnswers[100];

struct question* quizList[100];

//log in              1) are you teacher or student  
//for teacher:        2) enter teacher password
//                    3) would you like to create quiz; view a quiz; view student list; view student and grades; exit
//for student:        2) whats your username/password
//                    3) would you like to take quiz; view grade; exit
//if taking quiz:     4) option to select which quiz to take (should print timestamp after submitting)

struct student* createStudent(struct student* s, const char* user, const char* pass) { //FUNCTION TO ADD STUDENT TO STUDENT LIST

    struct student* newStud = (struct student*) malloc(sizeof(struct student));

    strcpy(newStud->username, user); 
    strcpy(newStud->password, pass); 
    newStud->grade = 0.0;
    newStud->numOfQuizzes = 0;
    newStud->next = NULL;    

    if(s == NULL) {
        s = newStud;
    }
    else{
        struct student* temp = s;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newStud;
    }

    printf("\nStudent created!\n");

    return s; 
}

int main(){

    int logOrSign = 0;
    int selection = 0;
    int teachOrStud = 0;


    char teacherPassword[100];

    struct student* studentList = NULL;

    bool loop = true; 

    while(loop) {
        char studentUserIn[100];
        char studentPassIn[100];
        bool teacherIn = false;
        bool studentIn = false;
        printf("\nWould you like to log in or sign up?\n1) Log in\n2) Sign up\n3) Quit\n");
        scanf("%d", &logOrSign);

        if(logOrSign == 1 || logOrSign == 2) {
            printf("Are you a teacher or student?\n1) Teacher\n2) Student\n");
            scanf("%d", &teachOrStud);
        }

        if(logOrSign == 1) { //USER WANTS TO LOG IN
            if(teachOrStud == 1){ //USER IS TEACHER
                printf("Enter your password: ");
                char passwordEntry[100];
                scanf("%s", &passwordEntry);
                if(strcmp(passwordEntry,teacherPassword) == 0) {
                    printf("Welcome teacher.\n");
                    teacherIn = true;
                }
                else {
                    printf("Invalid password!\n");
                }
            }
            else if(teachOrStud == 2) { //USER IS STUDENT
                printf("Enter your Username: ");
                char usernameEntry[100];
                scanf("%s", &usernameEntry);
                printf("Enter your Password: ");
                char passwordEntry[100];
                scanf("%s", &passwordEntry);

                struct student* listToTraverse = studentList;
                while(listToTraverse != NULL) {
                    if(strcmp(usernameEntry,listToTraverse->username) == 0 && strcmp(passwordEntry,listToTraverse->password) == 0) {
                        studentIn = true;
                        strcpy(studentUserIn, usernameEntry);
                        strcpy(studentPassIn, passwordEntry);
                        printf("\nWelcome %s!\n", usernameEntry);
                        break;
                    }
                    else{
                        listToTraverse = listToTraverse->next;
                        if(listToTraverse == NULL) {
                            printf("Invalid username or password!\n");
                        }
                    }
                }

            }
        }
        else if(logOrSign == 2) {//USER WANTS TO SIGN UP
            if(teachOrStud == 1) {//USER IS TEACHER
                printf("Enter your teacher password: ");
                scanf("%s", &teacherPassword);
                printf("Password created\n");
            }
            else if(teachOrStud == 2) {//USER IS STUDENT
                char tempUser[100];
                char tempPass[100];
                printf("Enter your student username: ");
                scanf("%s", &tempUser);
                printf("Enter your student password: ");
                scanf("%s", &tempPass);

                studentList = createStudent(studentList, tempUser, tempPass);
            }
        }
        else if(logOrSign == 3) {
            break;
        }
        

        while(studentIn == true) {
            int studentChoice = 0;
            printf("What would you like to do?\n1) Take quiz\n2) View grade\n3) Exit\n");
            scanf("%d", &studentChoice);

            if(studentChoice == 1) { //TAKE QUIZ
                if(quizList[0] == NULL) {
                    printf("No quizzes to view!\n");
                    continue;
                }
                
                int whichQuiz = 0;
                printf("Which quiz would you like to view?\n");

                for(int i = 0; i < sizeof(quizList) / sizeof(quizList[0]); i++) {
                    if(quizList[i] != NULL) {
                        printf("%d) Quiz %d\n", i+1, i+1);
                    }
                }
                scanf("%d", &whichQuiz);

                struct question* quizToPrint = quizList[whichQuiz-1];
                
                int questionNumber = 1;
                int numberCorrect = 0;

                while(quizToPrint != NULL) {
                    char response;
                    printf("QUESTION %d:\n", questionNumber);
                    printf("Question: %s\n", quizToPrint->question);
                    printf("Option A: %s\n", quizToPrint->optionA);
                    printf("Option B: %s\n", quizToPrint->optionB);
                    printf("Option C: %s\n", quizToPrint->optionC);
                    printf("Option D: %s\n", quizToPrint->optionD);
                    printf("Enter the letter answer you think is correct:\n");
                    scanf("\n%c", &response);

                    response = tolower(response);
                    char temp = quizToPrint->answerLetter;
                    temp = tolower(quizToPrint->answerLetter);


                    if(response == temp) {
                        numberCorrect++;
                    }
                    questionNumber++;
                    quizToPrint = quizToPrint->next;

                }
                float score = 100*(float)numberCorrect/(questionNumber-1);
                printf("\nYour score is: %.2f%%\n", score);

                struct student* listToTraverse = studentList;
                while(listToTraverse != NULL) {
                    if(strcmp(studentUserIn,listToTraverse->username) == 0 && strcmp(studentPassIn,listToTraverse->password) == 0) {
                        listToTraverse->grade = ((listToTraverse->grade * listToTraverse->numOfQuizzes) + score)/(listToTraverse->numOfQuizzes+1);
                        listToTraverse->numOfQuizzes++;
                        break;
                    }
                    listToTraverse = listToTraverse->next;
                }

            }
            else if(studentChoice == 2) { //VIEW GRADE
                struct student* listToTraverse = studentList;
                while(listToTraverse != NULL) {
                    if(strcmp(studentUserIn,listToTraverse->username) == 0 && strcmp(studentPassIn,listToTraverse->password) == 0) {
                        printf("Your grade in the class is: %.2f%%\n\n", listToTraverse->grade);
                        break;
                    }
                    listToTraverse = listToTraverse->next;
                }
            }
            else if(studentChoice == 3) { //EXIT
                studentIn = false;
                break;
            }
        }


        while(teacherIn == true) { //WHILE THE TEACHER IS LOGGED IN 
            int teacherChoice = 0;
            printf("What would you like to do?\n1) Create quiz\n2) View quiz\n3) View student list\n4) View student list and grades\n5) quit\n");
            scanf("%d", &teacherChoice);

            if(teacherChoice == 1) { //CREATE QUIZ
                int numOfQ = 0;
                printf("How many questions will be in this quiz?\n");
                scanf("%d", &numOfQ);

                struct question* newQuiz = (struct question*) malloc(sizeof(struct question));

                for(int i = 0; i < numOfQ; i++) {
                    struct question* newQ = (struct question*) malloc(sizeof(struct question));

                    char question[100];
                    char optionA[100];
                    char optionB[100];
                    char optionC[100];
                    char optionD[100];
                    char correctLetter;

                    printf("What is question %d?\n", i+1);
                    scanf("\n%[^\n]s", question);
                    printf("What is option A?\n");
                    scanf("%s", &optionA);
                    printf("What is option B?\n");
                    scanf("%s", &optionB);
                    printf("What is option C?\n");
                    scanf("%s", &optionC);
                    printf("What is option D?\n");
                    scanf("%s", &optionD);

                    printf("Which option letter is correct?\n");
                    scanf("\n%c", &correctLetter);

                    strcpy(newQ->question, question); 
                    strcpy(newQ->optionA, optionA); 
                    strcpy(newQ->optionB, optionB); 
                    strcpy(newQ->optionC, optionC); 
                    strcpy(newQ->optionD, optionD); 
                    newQ->answerLetter = correctLetter; 
                    newQ->next = NULL;

                    if(i == 0) {
                        newQuiz = newQ;
                    }
                    else{
                        struct question* temp = newQuiz;
                        while(temp->next != NULL) {
                            temp = temp->next;
                        }
                        temp->next = newQ;
                    }

                }

                for(int i = 0; i < sizeof(quizList) / sizeof(quizList[0]); i++) {
                    if(quizList[i] == NULL) {
                        quizList[i] = newQuiz;
                        break;
                    }
                }

            }
            else if(teacherChoice == 2) { //VIEW QUIZ
                if(quizList[0] == NULL) {
                    printf("No quizzes to view!\n");
                    continue;
                }
                
                int whichQuiz = 0;
                printf("Which quiz would you like to view?\n");

                for(int i = 0; i < sizeof(quizList) / sizeof(quizList[0]); i++) {
                    if(quizList[i] != NULL) {
                        printf("%d) Quiz %d\n", i+1, i+1);
                    }
                }
                scanf("%d", &whichQuiz);

                struct question* quizToPrint = quizList[whichQuiz-1];
                
                int questionNumber = 1;
                while(quizToPrint != NULL) {
                    printf("QUESTION %d:\n", questionNumber);
                    printf("Question: %s\n", quizToPrint->question);
                    printf("Option A: %s\n", quizToPrint->optionA);
                    printf("Option B: %s\n", quizToPrint->optionB);
                    printf("Option C: %s\n", quizToPrint->optionC);
                    printf("Option D: %s\n", quizToPrint->optionD);
                    printf("Correct Answer: %c\n", quizToPrint->answerLetter);
                    questionNumber++;
                    quizToPrint = quizToPrint->next;
                }
            }
            else if(teacherChoice == 3) { //PRINT STUDENT LIST
                struct student* temp = studentList;
                if(temp == NULL) {
                    printf("No students to print!\n");
                }
                else{
                    printf("Student List:\n");
                }
                while(temp != NULL) {
                    printf("Username: %s\n", temp->username);
                    temp = temp->next;
                }
            }
            else if(teacherChoice == 4) { //PRINT STUDENT LIST AND GRADES
                struct student* temp = studentList;
                if(temp == NULL) {
                    printf("No students to print!\n");
                }
                else{
                    printf("Student List:\n");
                }
                while(temp != NULL) {
                    printf("Username: %s, Grade: %.2f%%\n", temp->username, temp->grade);
                    temp = temp->next;
                }
            }
            else if(teacherChoice == 5) { //EXIT
                teacherIn = false;
                break;
            }
            else {
                printf("Invalid entry, try again\n");
            }
        }
    }

    
}
