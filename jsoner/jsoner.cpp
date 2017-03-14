/*
    jsoner
    Version     : 1.0
    Auther      : blackkitty
    Date        : 2017-3-14
    Description : JSON parsing for batch

    batch usage:
        jsoner [-f JSONFile]|[-s JSONString] SaveFile elementName
        call SaveFile
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MATCHED(c,s) (strchr(s,c)!=NULL)        /* judge if c is in s */
#define IS_WHITESPACE(c) (MATCHED(c," \t\n"))   /* judge if c is a whitespace */
#define IS_KEYCHAR(c) (MATCHED(c,"{}[],:\0"))   /* judge if c is a json key character */

enum ContentType {STRING_TYPE, NUMBER_TYPE, BOOL_TYPE, NULL_TYPE, BAD_TYPE};
enum ErrorType{SUCCESS = 0, BAD_CONTENT_TYPE, WRONG_CHARACTER, NO_MATCH, INCOMPLETE};
class ValueName{
public:
    char key[4096];

    ValueName(){
        stk = _stk;
        *stk = 0;
        memset(key,0,sizeof(key));
    }

    ValueName push(const char * str){
        int t = 0;
        tail = key+*stk;
        for(;*str != '\0';t++)
            *tail++ = *str++;
        *tail = '\0';
        stk[1] = *stk + t;
        stk++;
        return *this;
    }
    
    ValueName pop(){
        stk--;
        key[*stk] = '\0';
        return *this;
    }

private:
    int _stk[4096];
    int *stk;
    char *tail;
};
struct EC{
    ErrorType et;
    const char * pos;
    EC(ErrorType _et, const char * _pos):et(_et), pos(_pos){}
};


bool isNumber(const char *str);
bool matchStr(const char *str1, const char *str2);
ContentType typeCheck(const char *str, ErrorType & et);

char * loadJsonFile(const char * filepath);
char * getContent(const char * str, int & ctSize, ErrorType & et);
char * getIndex(int inx);

void whitespaceCLR(char *str);
int itoa(int x, char *buffer);


const char * _ErrorType[] = {
    "Success.",
    "Unexpected charater or data type.",
    "There should not be a '%c'.",
    "There are no '%c' before to match the '%c'.",
    "The json string is not complete."
};

EC json2cmd(char * jsonStr, FILE * hSaveFile, char * eName);
void errorReporter(EC e, const char * json);

int main(int argc, char *argv[]) {

    if(argc >= 5){
        char * json = NULL;
        FILE * fhSave;
        fhSave = fopen(argv[3], "w+");
        if(fhSave == NULL){
                return 1;
        }

        if(matchStr(argv[1], "-f")){    
            json = loadJsonFile(argv[2]);
            if(json == NULL){
                fclose(fhSave);
                return 1;
            } 
        }
        else if(matchStr(argv[1], "-s")){
            json = argv[2];
        }
        else
            goto help;
        
        errorReporter(json2cmd(json, fhSave, argv[4]), json);
        fclose(fhSave);

        return 0;
    }

    help:;
        puts("    jsoner ver 0.9 By blackkitty");
        puts("    jsoner [-f JSONFile]|[-s JSONString] SaveFile ObjectName");
        puts("");
    return 0;
}


void errorReporter(EC e, const char * json){
    if(e.et == SUCCESS){
        puts(_ErrorType[e.et]);
        return;
    }
    const char *tmp = "}]";
    char * part = (char*)malloc(sizeof(char)*50);
    char * p = (char*)(json>e.pos-20?json:e.pos-20);
    int i;
    for(i = 0;p[i]!='\0'&&i<40;i++){
        part[i] = p[i];
    }
    part[i] = '\0';
    puts("ERROR:");
    printf("%s%s%s\n",p==json?"":"...", part, i<40?"":"...");
    printf("%*s\n", (p==json?e.pos-json:23)+1, "^");
    switch(e.et){
        case BAD_CONTENT_TYPE:      /* fallthrough */
        case INCOMPLETE:
            puts(_ErrorType[e.et]);
            break;
        case WRONG_CHARACTER:
            printf(_ErrorType[e.et],*e.pos);
            puts("");
            break;
        case NO_MATCH:
            printf(_ErrorType[e.et], 
                "{["[strchr(tmp,*e.pos)-tmp],
                *e.pos);
            puts("");
            break;
        default:
            puts("Unexcepted Error!");
            break;
    }
    free(part); 
}
/*
    json2cmd
    jsont to cmd main loop
*/
EC json2cmd(char * strJson, FILE * fhSave, char * _objectName) {

    const char * p;
    ErrorType et = SUCCESS; 
    
    char _stk[4096];            /* record "" {} [] : */
    char *stk = _stk;

    ValueName objectName;             /* record element name */
    objectName.push(_objectName);

    int _inx[4096] = {0};       /* record list index */
    int *inx = _inx;

    p = strJson;
    char * tmp;

    for (; *p;p++) { 
        switch(*p){
            case '{':
                if(*stk == '{') return EC(WRONG_CHARACTER,p);
                *++stk = *p;
                break;
            case '[':
                if(*stk == '{') return EC(WRONG_CHARACTER,p);
                *++stk = *p;
                *++inx = 0;
                tmp = getIndex(*inx);
                objectName.push(tmp);
                free(tmp);
                break;
            case '}':
                if(*stk == '{') stk--;
                else if(*stk == ':'){
                    stk--,p--;
                    objectName.pop();
                    objectName.pop();
                } 
                else return EC(NO_MATCH, p);
                break;
            case ']':
                if(*stk == '[') stk--;
                else return EC(NO_MATCH, p);
                inx--;
                objectName.pop();
                break;
            case ':':
                *++stk = *p;
                break;
            case ',':
                if(*stk == '['){
                    ++*inx;
                    tmp = getIndex(*inx);
                    objectName.pop();
                    objectName.push(tmp);
                    free(tmp);
                }
                if(*stk == ':'){
                    stk--;
                    objectName.pop();
                    objectName.pop();
                }
                break;
            default:
                int ctSize = -1;
                tmp = getContent(p, ctSize, et);
                if(tmp == NULL) return EC(et, p);
                if(*stk == '{'){
                    /* left value */
                    objectName.push(".");
                    objectName.push(tmp);
                }
                else{
                    /* right value */
                    fprintf(fhSave, "set \"%s=%s\"\n", objectName.key, tmp);
                }
                free(tmp);
                p+=ctSize;
                break;
        }
    }
    if(stk != _stk) return EC(INCOMPLETE, p);
    return EC(SUCCESS, p);
}

/*
    whitespaceCLR
    delete all the whitespace who doesn't effect the value.
*/
void whitespaceCLR(char * str){
    bool flag = false;
    char *p = str;
    for (; *str != '\0';str++){
        if(*str == '\n') *str=' ';
        if(*str == '"')
            flag = !flag;
        if(flag || (*str!=' ' && *str!='\t' && *str!='\n'))
            *p++ = *str;
    }
    *p = '\0';
}


/*
    itoa
    int to string.
    Return length of string.

    Attention:
        enough memory of buffer.
*/
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


/*
    getIndex
    int to index string

    Attention:
        free the memory of index string.
*/
char * getIndex(int inx){
    if(inx < 0) return NULL;
    char * ret = (char *)malloc(sizeof(char)*15);
    ret[0] = '[';
    int t = itoa(inx, ret+1);
    ret[t+1] = ']';
    ret[t+2] = '\0';
    return ret;
} 

/*
    isNumber
    judge if a string is a json number.
*/
bool isNumber(const char *str){
    /* -0.0e+003 */
    bool ef, df, ff, fnf, pzf, pnf;
    ef = false;     /* e flag */
    df = false;     /* dot flag */
    ff = true;      /* first position flag */
    fnf = true;     /* first number flag */
    pzf = false;    /* prefix zero flag */
    pnf = false;      /* prefix non-zero flag */
    for (const char *p = str; !IS_KEYCHAR(*p);p++){
        if(MATCHED(*p, "+-")){
            if(!ef && *p == '+') return false;
            if(!ff) return false;
            ff = false;
        }
        else if(MATCHED(*p, "eE")){
            if(ff || fnf || ef) return false;
            ff = fnf = ef = df = true;
            pnf = false;
        }
        else if(*p == '.'){
            if(df || ff || fnf || ef) return false;
            df = fnf = true;
        }
        else if(*p >= '0' && *p <= '9'){
            if(*p == '0' && !pnf &&!df){
                if(pzf) return false;
                pzf = true;
            }
            if(*p != '0')
                pnf = true;
            ff = fnf = false;
        }
        else{
            return false;
        }
    }
    return !fnf;
}

/*
    matchStr
    judge if str2 matched str1. 
*/
bool matchStr(const char * str1, const char * str2){
    for (; *str2 != '\0';str1++,str2++)
        if(*str1 != *str2)
            return false;
    return IS_KEYCHAR(*str1);
}

/*
    typeCheck
    get the type of content.
*/
ContentType typeCheck(const char * str, ErrorType & et){

    if(matchStr(str, "true") || matchStr(str, "false"))
        return BOOL_TYPE;
    
    if(matchStr(str, "null"))
        return NULL_TYPE;
    
    if(isNumber(str))
        return NUMBER_TYPE;

    if(*str == '"'){
        for (str++; *str != '"';str++);
        if(IS_KEYCHAR(str[1]))
            return STRING_TYPE;
    }
    et = BAD_CONTENT_TYPE;
    return BAD_TYPE;
}

/*
    getContent
    Return content and size of content.
    Return NULL if content do not comply with grammar.

    Attention:
        free the memory of content
*/
char * getContent(const char * str, int & ctSize, ErrorType & et){
    char * buffer;      /*constent buffer*/
    char ctType;        /*content type*/
    const char * endFlag;

    switch(ctType = typeCheck(str, et)){
        case STRING_TYPE:     endFlag = "\""; str++; break;
        case BOOL_TYPE:       /* fallthrough... */
        case NULL_TYPE:       /* fallthrough... */
        case NUMBER_TYPE:     endFlag = " \t\n{}[],:\0";break;
        case BAD_TYPE:        /* fallthrough... */
        default: return NULL;
    }

    /* copy content to buffer */
    for(ctSize=0;!MATCHED(str[ctSize], endFlag);ctSize++);
    buffer = (char *)malloc(sizeof(char)*(ctSize+1));
    memcpy(buffer, str, sizeof(char)*ctSize);
    buffer[ctSize] = '\0';
    if(ctType == STRING_TYPE)
        ctSize++;
    else
        ctSize--;

    return buffer;
}

/*
    loadJsonFile
    return file content.

    Attention:
        free the memory of return.
*/
char *loadJsonFile(const char * filepath){
    char * ret;
    FILE *fh;
    fh = fopen(filepath, "r");
    if(fh == NULL) return NULL;

    /* get file size */
    int fsize = 0;
    fseek(fh,0,SEEK_SET);
    fsize = ftell(fh);
    fseek(fh,0,SEEK_END);
    fsize = ftell(fh) - fsize;

    /* load file */
    ret = (char *)malloc(sizeof(char)*(fsize+3));
    fseek(fh, 0, SEEK_SET);
    fsize = fread(ret, sizeof(char), fsize-1, fh);
    ret[fsize] = '\0';
    whitespaceCLR(ret);
    fclose(fh);

    return ret;
}