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

#define C_STRING_TYPE '"'
#define C_NUMBER_TYPE '#'
#define C_BOOL_TYPE '@'
#define C_NULL_TYPE '$'

int json2cmd(const char * jsonStrPtr, FILE * hSaveFile);
bool _json2cmd(const char *&jsonStrPtr, FILE * hSaveFile, char * name);
char * getContent(const char * strPtr);
void ignoreBlank(char *& strPtr);

#define MATCHED(c,s) (strchr(s,c)!=NULL)

inline bool IS_WHITESPACE(char c){
    return MATCHED(c," \t\n");
}
inline bool IS_KEYCHAR(char c){
    return MATCHED(c,"{}[],:\0");
}

inline char TYPE_CHECK(const char * str){
    if(*str == C_STRING_TYPE) 
        return C_STRING_TYPE;
    if(*str >= '0' && *str <= '9')
        return C_NUMBER_TYPE;
    if(
        strstr(str,"true") == str && (IS_KEYCHAR(str[4]) || IS_WHITESPACE(str[4]))
      ||strstr(str,"false") == str && (IS_KEYCHAR(str[5]) || IS_WHITESPACE(str[5]))
      )
        return C_BOOL_TYPE;
    if(
        strstr(str,"null") == str && (IS_KEYCHAR(str[4]) || IS_WHITESPACE(str[4]))
    )
        return C_NULL_TYPE;
    return *str;
}

int main(int argc, char *argv[]) {
    /************test**************/
    char *sl[] = {"123,","true,","null]","\"asdf sa\"","false "};
    for(int i=0;i<sizeof(sl)/sizeof(char*);i++){
        puts(getContent(sl[i]));
    }
    return 0;
    /******************************/

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
char * getContent(const char * strPtr){
    /*
        start   end         ignoreBlank
        "       "           0
        :       ,}          1
        [       ,]          1
        ,       ,]          1
        number  whitespace  0
    */
    int ctSize;
    char * buffer;
    char ctType;
    char * endFlag;

    switch(ctType = TYPE_CHECK(strPtr)){
        case C_STRING_TYPE:     endFlag = "\""; strPtr++;break;
        case C_BOOL_TYPE:       /* fallthrough... */
        case C_NULL_TYPE:       /* fallthrough... */
        case C_NUMBER_TYPE:     endFlag = " \t\n{}[],:\0";break;
        default: return NULL;
    }
    printf("type: %c  ",ctType);
    for(ctSize=0;!MATCHED(strPtr[ctSize], endFlag);ctSize++);
    buffer = (char *)malloc(sizeof(char)*(ctSize+1));
    memcpy(buffer, strPtr, sizeof(char)*ctSize);
    buffer[ctSize] == '\0';

    return buffer;
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

bool _json2cmd(const char *&jsonStrPtr, FILE * hSaveFile, char * name){
    return false;
}