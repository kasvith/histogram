//
// Created by Kasun on 04/04/2017.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define DEFAULT_NUMBER 10

typedef struct _word{
    char* content;//assume no word is greater than 100 chars
    int count;
    struct _word* next;
} word;
typedef struct _file{
    char* fname;
    struct _file* next;
} file;

//function headers
int isNum(const char*);//check is a string a valid number
void printVLine();//prints a vertical line char without new line
void printVLine();// prints a horizontal line char without newline
void printUsage();//prints usage
char* stripCharacters(char* str);//strips characters from a string and returns a new copy of that
void insertWord(char* str);//inserts a word into LL if it does not exists, otherwise count it up
void insertFile(char* fname);//inserts a file name into LL
int isFilesEmpty();//returns 0 if LL empty
int isWordsEmpty();//returns 0 if LL empty
int getWordCount();//returns the length of word list
word* findWord(char* key);//searchs a word, if not found return NULL
void sortWordsByFrequency();//sorts words descending order by frequency
void parseFile(char* fname, int flag_word);//parse a single file


//definitions
word* words = NULL;
word* currWord = NULL;

file* files = NULL;
file* currFile = NULL;

int count = 0;
//end definitions

int main(int argc,  char **argv) {
    int length = DEFAULT_NUMBER;
    int flag_word = 1;
    int flag_scaled = 0;

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
                if(isNum(argv[opt_arg])){
                    length = atoi(argv[opt_arg]);
                    if(length < 0){
                        printf("Length should be a positive number\n");
                        exit(EXIT_FAILURE);
                    }
                }else{
                    printf("Invalid options for [-l]\n");
                    printUsage();
                }
            }else{
                printf("Not enough options for [-l]\n");
                printUsage();
            }
        }else if(arg[0] == '-' && !isNum(arg)){
            printf("Invalid option [%s]\n", arg);
            printUsage();
        }
        else{
            //we have handle all the possible options, now processing the non opt args
            //we assume they are file names
            if(i != opt_arg && i != 0){
                insertFile(argv[i]);
                //insertWord(argv[i]);
            }
        }
    }
    //end handling user input

    if(isFilesEmpty()){
        printf("No input files were given\n");
        printUsage();
    }

    //if length is zero exit
    if(length == 0){
        exit(0);
    }

    //iterate over all file names
    file* fPtr = files;
    while (fPtr != NULL){
        parseFile(fPtr->fname, flag_word);
        fPtr = fPtr->next;
    }

    sortWordsByFrequency();

    word* pWords = words;
    while (pWords != NULL && length > 0){
        printf("%s => %.2f\n", pWords->content, ((float)pWords->count*100.0/(float)count));
        pWords = pWords->next;
        length--;
    }

    return 0;
}

void parseFile(char* fname, int flag_word){
    FILE* fp;
    fp = fopen(fname, "r");

    if(!fp){
        printf("Cannot open one or more given files\n");
        exit(EXIT_FAILURE);
    }


    if(flag_word){
        char w[1000];
        while(fscanf(fp, "%s ", w) != EOF){
            insertWord(w);
        }
    }else{
        char c;
        while (fscanf(fp, "%c", &c) != EOF){
            char a[2];
            a[0] = c;
            a[1] = '\0';
            insertWord(a);
        }
    }

    fclose(fp);
}

void sortWordsByFrequency(){
    int i, j, k, l, tmpCount, size = getWordCount();
    char* tmpContent;
    word* current;
    word* next;

    k = size;
    for ( i = 0; i < size - 1; i++, k--) {
        current = words;
        next = current->next;

        for (j = 1; j < k; ++j) {
            if(current->count < next->count){
                int wsz = strlen(current->content);
                tmpContent = (char*)malloc(wsz + 1);
                tmpCount = current->count;
                strcpy(tmpContent, current->content);

                current->content = realloc(current->content, strlen(next->content) + 1);
                strcpy(current->content, next->content);
                current->count = next->count;

                next->content = realloc(next->content, strlen(tmpContent) + 1);
                strcpy(next->content, tmpContent);
                next->count = tmpCount;

                free(tmpContent);
            }

            current = current->next;
            next = next->next;
        }
    }
}

int getWordCount(){
    int c = 0;
    word* pWord = words;
    while (pWord != NULL){
        c++;
        pWord = pWord->next;
    }

    return c;
}

void insertWord(char* str){
    char* s = stripCharacters(str);
    word* w = findWord(s);

    if(w != NULL){
        w->count++;
        count++;
    }else{
        if(s && strlen(s) > 0){
            word* tmp = (word *)malloc(sizeof(word))  ;
            tmp->content = s;
            tmp->count = 1;
            tmp->next = words;
            words = tmp;

            count++;
        }
    }
}

word* findWord(char* key){
    word* current = words;

    if(current == NULL){
        return NULL;
    }

    while(strcmp(key, current->content) != 0){
        if(current->next == NULL)
            return NULL;
        else
            current = current->next;
    }

    return current;
}

void insertFile(char* fname){
    file* node = (file* )malloc(sizeof(file));
    node->fname = (char*) malloc(strlen(fname) + 1);
    strcpy(node->fname, fname);

    node->next = files;
    files = node;
}

int isFilesEmpty(){
    return files == NULL;
}

int isWordsEmpty(){
    return words == NULL;
}

char* stripCharacters(char* str){
    char* s = malloc(strlen(str) + 1);

    if(!s){
        printf("%s\n", "Error allocating memory");
        exit(1);
    }

    int i, j = 0;
    for(i = 0; str[i] != '\0'; i++){
        if(isalnum(str[i]) && !isspace(str[i]) && !iscntrl(str[i])){
            s[j++] = str[i];
        }
    }
    s[j++] = '\0';

    char* orig = malloc(strlen(s) + 1);

    i = 0;j = 0;
    for(i = 0; i < strlen(s); i++){
        orig[j++] = s[i];
    }

    orig[j++] = '\0';
    free(s);

    return orig;
}

void printUsage(){
    printf("usage: freq [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n");
    exit(EXIT_FAILURE);
}

void printVLine() {
    printf("%s", "\e(0\x78\e(B");
}

void printHLine() {
    printf("%s", "\e(0\x71\e(B");
}

int isNum(const char* str){
    int i = 0;

    //if first char is '+/-' it is still could be a number
    int len = strlen(str);
    if(len > 1 && (str[0] == '+' || str[0] == '-'))
        i++;

    while (str[i] != '\0') {
        if(isdigit(str[i]) == 0)
            return 0;
        i++;
    }

    return 1;
}
