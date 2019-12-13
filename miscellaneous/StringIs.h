

#ifndef StringIs_h
#define StringIs_h
#include <string.h>
class StringIs{
    char * value = 0;
public:
    friend StringIs operator < (const char * str, const StringIs& _){
        StringIs strValSaver;
        strValSaver.value = strdup(str);
        return strValSaver;
    }
    friend bool operator > (const StringIs& strValContainer, const char * rhs){
        return strcmp(strValContainer.value, rhs) == 0;
    }
    
    ~StringIs(){
        if(value){
            free(value);
        }
    }
};

#define is <StringIs()>



#endif /* StringIs_h */
