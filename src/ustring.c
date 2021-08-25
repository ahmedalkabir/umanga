//
// Created by Ahmed Alkabir on 9/28/2017.
//

#include <ustring.h>
#include <string.h>
#include <stdlib.h>


/**
 * To duplicate a string
 */
char *strduplicate(const char *src){
    char *dest = (char *)malloc(strlen(src) + 1);
    if(dest == NULL) return NULL;
    strcpy(dest,src);
    return dest;
}

/**
 * Construction
 */
uString *uString_new(const char *str){
    // allocate a uString and return it
    uString *new_str = (uString *)malloc(sizeof(uString));
    // first let's get size of string
    new_str->length = strlen(str);
    // allocate char * in heap memory
    new_str->value = (char *)malloc(new_str->length);
    // copy the desired string
    new_str->value = strduplicate(str);

    // attaches the member functions
    new_str->trim_character = ustring_trim;
    // free function
    new_str->free = ustring_dellacate;


    // return it
    return new_str;
}

/** Dynamic Concatenating Strings
 *
 *  use generic macro function instead of specified functions
 *  it can deduce the type at compile time
 *
 *  Function : concat(uString , (char *|| uString ))
 * */


/// for (char *) types
void ustring_concat_char(uString *str_dest,const char *str_src){
    if(str_dest != NULL && str_src != NULL){
        int length = strlen(str_src);
        str_dest->value = (char *)realloc(str_dest->value,str_dest->length + length +1);
        str_dest->length += length;
        strcat(str_dest->value, str_src);
    }
}

/// for (uString) types
void ustring_concat_ustring(uString *str_dest,uString *str_src){
    if(str_dest != NULL && str_src != NULL){
        str_dest->value = (char *)realloc(str_dest->value,str_dest->length + str_src->length +1);
        str_dest->length += str_src->length;
        strcat(str_dest->value, str_src->value);
    }
}

/** to remove charecter from the haed and the tail of
 *  uString object
 * */
/// for (uString) types
void ustring_trunchar_ustring(uString *str,char head, char tail){
    if(str != NULL && str->length > 0){
        size_t len = str->length;
        if(len >= 2 && str->value[0] == head && str->value[len -1] == tail){
            memmove_s(str->value ,len ,str->value+1 , len - 2);
            str->value[len - 2] = '\0';
        }
    }
}

/// for (char *) types
void ustring_trunchar_char(char *str, char head, char tail){
    if(str != NULL){
        size_t len = strlen(str);
        if(len >= 2 && str[0] == head && str[len -1] == tail){
            memmove_s(str ,len ,str + 1, len - 2);
            str[len - 2] = '\0';
        }
    }
}
/** to make string at lower case
 ***/
char *ustring_tolower(const char *str){
    char *str_lower = (char *)malloc(strlen(str) + 1);
    memset(str_lower,0,strlen(str) + 1);
    // to back reference to head of variable address
    void *head = (unsigned char *)str_lower;
    if(str != NULL){
        while(*str){
            *str_lower = tolower(*str);
            str_lower++;
            str++;
        }
    }
    *str_lower = '\0';
    str_lower = head;
    return str_lower;
}

/**
 *  To remove specific character from the uString object
 * */
void ustring_trim(uString *str,const char char_to_trim){
    // make sure isn't empty
    if(str != NULL && str->length > 0){
        size_t new_length = 0;
        char *string = (char *)malloc(str->length + 1);
        void *head_ustring = (void *)str->value;
        void *head_string = (void *)string;
        *string = '\0';
        while(*str->value != '\0'){
            if(*str->value != char_to_trim){
                *(string++) = *str->value;
                new_length++;
            }
            str->value++;
        }

        if(new_length > 0){
            *string='\0';
            string = head_string;

            // free the old string
            str->value = head_ustring;
            free(str->value);
            str->value = (char *)malloc(new_length + 1);
            str->length = new_length;
            // copy the new string
            memcpy_s(str->value, new_length + 1, string, new_length +1 );
        }
    }

}
/**
 *  To clean and free uString object
 * */
void ustring_dellacate(uString *str){
    if(str != NULL){
        free(str->value);
        str->length = 0;
        str->value = NULL;

        // clean uString object
        free(str);
    }
}

