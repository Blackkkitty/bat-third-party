/*
    jsoner.cpp By blackkitty

    Useage:
        Convert JSON to bat commands.

    Example：
        jsoner [-f JSONFile]|[-s JSONString] SaveFile
        set NumberOfArguments=%errorlevel%
        call SaveFile [Arguments]

*/
#include <stdio.h>
#include <stdlib.h>

int json2cmd(const char * jsonStrPtr, FILE * hSaveFile);
bool _json2cmd(const char *&jsonStrPtr, FILE * hSaveFile, char * name);
void getContent(const char * strPtr, char * bufferPtr);
void ignoreBlank(char *& strPtr);


void f(const char *&ss) {
    ss += 2;
}

int main(void) {
    char s[] = "123213";
    const char *sp = s;

    return 0;
}

void ignoreBlank(const char *& strPtr) {
    for (;
            *strPtr == ' '  ||
            *strPtr == '\t' ||
            *strPtr == '\n' ||
            *strPtr == '\0'
        ; strPtr++);
}

void getContent(const char * strPtr, char * bufferPtr){
    for(;
            *strPtr != '"'  ||
            *strPtr != '\t' ||
            *strPtr != '\n' ||
            *strPtr != '\0'
        ;){

    }
}

int json2cmd(const char * jsonStrPtr, FILE * hSaveFile) {
    const char * p;
    char elementName[256];
    p = jsonStrPtr;
    ignoreBlank(p);
    for (; *p;) {
        if (*p == '"')
            _json2cmd(p, hSaveFile, elementName);


    }
}