//
// Created by Kasun on 04/04/2017.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//defines
#define DEFAULT_NUMBER 10
#define MAX_WIDTH 80
//end defines

//typedefs
typedef enum _ { false, true } bool;
//end typedefs

//function headers
bool isNum(const char*);//check is a string a valid number
void printVLine();//prints a vertical line char without new line
void printVLine();
void printUsage();//prints usage
//end function headers

int main(int argc,  char **argv) {
    int length = DEFAULT_NUMBER;
    int flag_word = 1;
    int flag_scaled = 0;

    //handle user input
    int i,opt_arg;
    for (i = 0; i < argc; i++) {
        char* arg = argv[i];

        if(strcmp(arg, "-w") == 0){
            flag_word = 1;
        }else if(strcmp(arg,"-c") == 0){
            flag_word = 0;
        }else if(strcmp(arg ,"--scaled") == 0){
            flag_scaled = 1;
        }else if(strcmp(arg ,"-l") == 0){
            if(i + 1 < argc){
                opt_arg = i + 1;
                if(isNum(argv[opt_arg]) == true){
                    length = atoi(argv[opt_arg]);

                    if(length < 0){
                        printf("Length should be a positive number\n");
                        exit(EXIT_FAILURE);
                    }else if(length == 0){
                        exit(0);
                    }
                }else{
                    printf("Invalid options for [-l]\n");
                    printUsage();
                    exit(EXIT_FAILURE);
                }
            }else{
                printf("Not enough options for [-l]\n");
                printUsage();
                exit(EXIT_FAILURE);
            }
        }else if(arg[0] == '-' && isNum(arg) == false){
            printf("invalid opt %s\n", arg);
            printUsage();
            exit(EXIT_FAILURE);
        }
        else{
            //we have habdle all the possible options, now processing the non opt args
            //we assume they are file names
            if(i != opt_arg && i != 0){
                printf("arg = %s\n", arg);
            }
        }
    }
    //end handling user input

    printf("word %d\n", flag_word);
    printf("scale %d\n", flag_scaled);
    printf("length %d\n", length);
    return 0;
}

void printUsage(){
    printf("usage: freq [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n");
}

void printVLine() {
    printf("%s", "\e(0\x78\e(B");
}

void printHLine() {
    printf("%s", "\e(0\x71\e(B");
}

bool isNum(const char* str){
    int i = 0;

    //if first char is '+/-' it is still could be a number
    int len = strlen(str);
    if(len > 1 && (str[0] == '+' || str[0] == '-'))
        i++;
    
    while (str[i] != '\0') {
        if(isdigit(str[i]) == 0)
            return false;
        i++;
    }

    return true;
}
