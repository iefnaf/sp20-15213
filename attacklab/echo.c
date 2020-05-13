#include <stdio.h>
#include <stdlib.h>

void smash(){
    printf("I've been smashed!\n");
    exit(0);
}

/* Implementation of library function gets() */
char *gets(char *s){
    int c;
    char *dest = s;
    while((c = getchar()) != '\n' && c != EOF)
        *dest++ = c;
    if(c == EOF && dest == s)
        /* No characters read */
        return NULL;
    *dest++ = '\0'; /* Terminate string */
    return s;
}

/** Read input line and write it back */
void echo(){
    char buf[4];
    gets(buf);
    puts(buf);
}

int main(int argc, char* argv[]){
    echo();
    return 0;
}