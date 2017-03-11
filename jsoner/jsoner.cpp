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
#define C_BAD_TYPE '!'

int json2cmd(const char * jsonStr, FILE * hSaveFile);
bool _json2cmd(const char *&jsonStr, FILE * hSaveFile, char * name);
char * getContent(const char * str, int & ctSize);
bool isNumber(const char *str);
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

bool isNumber(const char *str){
    /* -0.0e+003 */
    bool ef, df, ff, fnf, pzf;
    ef = false;     /* e flag */
    df = false;     /* dot flag */
    ff = true;      /* first position flag */
    fnf = true;     /* first number flag */
    pzf = false;    /* prefix zero flag */
    for (const char *p = str; !IS_KEYCHAR(*p);p++){
        if(MATCHED(*p, "+-")){
            if(!ef && *p == '+') return false;
            if(!ff) return false;
            ff = false;
        }
        else if(MATCHED(*p, "eE")){
            if(ff || fnf || ef) return false;
            ff = fnf = ef = df = true;
        }
        else if(*p == '.'){
            if(df || ff || fnf || ef) return false;
            df = fnf = true;
        }
        else if(*p >= '0' && *p <= '9'){
            if(*p == '0' && !df){
                if(pzf) return false;
                pzf = true;
            }
            ff = fnf = false;
        }
        else{
            return false;
        }
    }
    return !fnf;
}
bool matchStr(const char * str1, const char * str2){
    for (; str2 != '\0';str1++,str2++)
        if(*str1 != *str2)
            return false;
    return true;
}

inline char TYPE_CHECK(const char * str){
    if(*str == '"'){
        for (str++; *str != '"';str++);
        if(IS_KEYCHAR(str[1]))
            return C_STRING_TYPE;
        else
            return C_BAD_TYPE;
    }

    if(matchStr(str, "true") || matchStr(str, "false")))
        return C_BOOL_TYPE;
    
    if(matchStr(str, "null"))
        return C_NULL_TYPE;
    
    if(isNumber(str))
        return C_NUMBER_TYPE;
    
    return C_BAD_TYPE;
}

int main(int argc, char *argv[]) {
    /************test**************/
    char s[120];
    for (;~scanf("%s",s);){
        printf("%s\n", isNumber(s) ? "true" : "false");
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
        Return the content of key or value.
        Return NULL if content do not comply with grammar.
*/
char * getContent(const char * str, int & ctSize){
    char * buffer;      /*constent buffer*/
    char ctType;        /*content type*/
    char * endFlag;

    switch(ctType = TYPE_CHECK(str)){
        case C_STRING_TYPE:     endFlag = "\""; str++; break;
        case C_BOOL_TYPE:       /* fallthrough... */
        case C_NULL_TYPE:       /* fallthrough... */
        case C_NUMBER_TYPE:     endFlag = " \t\n{}[],:\0";break;
        case C_BAD_TYPE:        /* fallthrough... */
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
    strcpy(name, eName);
    char *nametail = name + strlen(name);
    char *lasttail = name;

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