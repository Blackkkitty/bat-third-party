/*
    jsoner
    Version     : 0.1 beta
    Auther      : Blackkitty
    Date        : 2017-3-7
    Description : JSON parsing for batch

    batch usage:
        jsoner [-f JSONFile]|[-s JSONString] SaveFile elementName
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

int json2cmd(const char * jsonStr, FILE * hSaveFile);
bool _json2cmd(const char *&jsonStr, FILE * hSaveFile, char * name);
char * getContent(const char * str, int & ctSize);
void whitespaceCLR(char *str);

#define MATCHED(c,s) (strchr(s,c)!=NULL)

inline bool IS_WHITESPACE(char c){
    return MATCHED(c," \t\n");
}
inline bool IS_KEYCHAR(char c){
    return MATCHED(c,"{}[],:\0");
}
int itoa(int x, char * buffer){
    if(x == 0){
        *buffer = '0';
        return 1;
    }
    int len = 0;
    for(int z=x;z;z/=10) len++;
    for(int i=len;x;x/=10){
        buffer[--i] = '0' + x%10;
    }
    return len;
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
    char s[100] = "-------------";  
    s[itoa(1234,s)] = '\0';
    puts(s);
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
    whitespaceCLR
    summary:
        delete all the whitespace who doesn't effect the value.
*/
void whitespaceCLR(char * str){
    bool flag = false;
    char *p = str;
    for (; *str != '\0';str++){
        if(*str == '"')
            flag = !flag;
        if(flag || (*str!=' ' && *str!='\t' && *str!='\n'))
            *p++ = *str;
    }
    *p = '\0';
}


/*
    getContent
    summary:
        return the content of key or value.
*/
char * getContent(const char * str, int & ctSize){
    /*
        start   end         ignoreBlank
        "       "           0
        :       ,}          1
        [       ,]          1
        ,       ,]          1
        number  whitespace  0
    */
    char * buffer;      /*constent buffer*/
    char ctType;        /*content type*/
    char * endFlag;

    switch(ctType = TYPE_CHECK(str)){
        case C_STRING_TYPE:     endFlag = "\""; str++; break;
        case C_BOOL_TYPE:       /* fallthrough... */
        case C_NULL_TYPE:       /* fallthrough... */
        case C_NUMBER_TYPE:     endFlag = " \t\n{}[],:\0";break;
        default: return NULL;
    }

    /* copy content to buffer */
    for(ctSize=0;!MATCHED(str[ctSize], endFlag);ctSize++);
    buffer = (char *)malloc(sizeof(char)*(ctSize+1));
    memcpy(buffer, str, sizeof(char)*ctSize);
    buffer[ctSize] = '\0';

    return buffer;
}


/*
    json2cmd
    summary:
        jsont to cmd main loop
*/
int json2cmd(char * jsonStr, FILE * hSaveFile, char * eName) {

    const char * p;
    
    char _stk[4096];            /* record "" {} [] : */
    char *stk = _stk;

    char name[4096];           /* record element name */
    strcpy(_name, eName);
    char *nametail = _name + strlen(_name);
    char *lasttail = _name;

    int _inx[4096] = {0};       /* record list index */
    int *inx = _inx;


    whitespaceCLR(jsonStr);
    p = jsonStr;

    for (; *p;) { 
        switch(*p){
            case '"':
                if(*stk == '"') stk--;
                else *stk-- = *p;

                break;
            case '{':
                *stk++ = *p;
                break;
            case '[':
                *stk++ = *p;
                break;
            case '}':
                if(*stk == '{') stk--;
                else return 1;
                break;
            case ']':
                if(*stk == '[') stk--;
                else return 1;
                break;
            case ':':
                *stk++ = *p;
                break;
            case ',':
                if(*stk == '[') ++*inx;
                

                break;
            default:;
        }
    }
}

bool _json2cmd(const char *&jsonStr, FILE * hSaveFile, char * name){
    return false;
}