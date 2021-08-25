//
// Created by korab on 9/28/2017.
//

#ifndef UMANGA_USTRING_H
#define UMANGA_USTRING_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

typedef struct ustring_ uString;

/* macro functions */
#define uString_concat(dest, src) _Generic((dest,src), \
                                    char *: ustring_concat_char, \
                                    uString *: ustring_concat_ustring)((dest), (src))

#define uString_trunchar(dest, head, tail) _Generic((dest), \
                                            char *: ustring_trunchar_char,\
                                            uString *: ustring_trunchar_ustring)((dest) , (head), (tail))

/* public functions */
extern uString *uString_new(const char *str);
extern void ustring_concat_char(uString *str_dest,const char *str_src);
extern void ustring_concat_ustring(uString *str_dest,uString *str_src);
extern void ustring_trunchar_char(char *str, char head, char tail);
extern void ustring_trunchar_ustring(uString *str,char head, char tail);
extern char *ustring_tolower(const char *str);
extern void ustring_dellacate(uString *str);
/* private function */
char *strduplicate(const char *src);
void ustring_trim(uString *str,const char char_to_trim);

struct ustring_{
    char *value;
    size_t length;

    void (*free)(uString *);
    void (*trim_character)(uString *, const char);
};

#endif //UMANGA_USTRING_H
