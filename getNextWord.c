#include "stdio.h"
int getNextWord(FILE* fPtr, char* word){
    int c;
    int n = 0;
    c = fgetc(fPtr);
    while (c != '\n' && c != '\t' && c !=' ' && c != EOF)
    {
        word[n++] = (char) c;
        c = fgetc(fPtr);
    }
    word[n] = '\0';
    if(c == EOF && n == 0) return 0;
    return 1;
}