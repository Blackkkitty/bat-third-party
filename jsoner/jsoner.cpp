/*
    jsoner
    Version     : 0.1 beta
    Auther      : Blackkitty
    Date        : 2017-3-7
    Description : JSON parsing for batch

    batch usage:
        jsoner [-f JSONFile]|[-s JSONString] SaveFile
        set NumberOfArguments=%errorlevel%
        call SaveFile [Arguments]
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define C_NUMBER_TYPE '#'
#define C_BOOL_TYPE '@'

int json2cmd(const char * jsonStrPtr, FILE * hSaveFile);
bool _json2cmd(const char *&jsonStrPtr, FILE * hSaveFile, char * name);
void getContent(const char * strPtr, char * bufferPtr);
void ignoreBlank(char *& strPtr);

#define MATCHED(c,s) (strchr(s,c)!=NULL)

inline bool IS_WHITESPACE(char c){
    return MATCHED(c," \t\n");
}
inline bool IS_KEYCHAR(char c){
    return MATCHED(c,"{}[],:\0");
}

inline char TYPE_CHECK(const char * str){
    if(*str >= '0' && *str <= '9')
        return C_NUMBER_TYPE;
    if(
        strstr(str,"true") == str && (IS_KEYCHAR(str[4]) || IS_WHITESPACE(str[4]))
      ||strstr(str,"false") == str && (IS_KEYCHAR(str[5]) || IS_WHITESPACE(str[5]))
      )
      return C_BOOL_TYPE;
    return *str;
}

int main(int argc, char *argv[]) {

    if(argc > 2){


        return 0;
    }

    help:;
        puts("jsoner");
    return 0;
}

/*
    ignoreBlank
    summary:
        jump to next charactor and ignore the blank charactor.
*/
inline void ignoreBlank(const char *& strPtr) {
    for (;*strPtr!='\0' && IS_WHITESPACE(*strPtr); strPtr++);
}

/*
    getContent
    summary:
        get the content of key or value.
*/
void getContent(const char * strPtr, char * bufferPtr){
    /*
        start   end         ignoreBlank
        "       "           0
        :       ,}          1
        [       ,]          1
        ,       ,]          1
        number  whitespace  0


    */
    const char * endflag;
    switch(TYPE_CHECK(*strPtr)){
        case '"':           endflag = "\"";break;
        case ':':           endflag = ",}";break;
        case '[':           /* fallthrough... */
        case ',':           endflag = ",]";break;
        case C_BOOL_TYPE:   /* fallthrough... */
        case C_NUMBER_TYPE: endflag = " \t\n{}[],:\0";break;
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