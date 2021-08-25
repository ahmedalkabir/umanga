//
// Created by korab on 9/28/2017.
//

#ifndef UMANGA_UVECTOR_H
#define UMANGA_UVECTOR_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* types */
typedef struct uvector_ uVector;
typedef struct uvector_obj_ uVector_Obj;

/* private functions */
uVector_Obj *get_next(uVector *vector);

/* public functions */

extern uVector *uVector_new(size_t max, size_t objsize, int options);
extern bool uvector_addat(uVector *vector, int index, const void *data);
extern bool uvector_addfirst(uVector *vector, const void *data);
extern bool uvector_addlast(uVector *vector, const void *data);
extern void *uvector_getat(uVector *vector, int index, bool newmem);
extern bool uvector_removefirst(uVector *vector);
extern bool uvector_removelast(uVector *vector);
extern bool uvector_removeat(uVector *vector,int index);
extern void *uvector_popfirst(uVector *vector);
extern void *uvector_poplast(uVector *vector);
extern void *uvector_popat(uVector *vector, int index);
extern bool uvector_setat(uVector *vector, int index, const void * data);
extern bool uvector_resize(uVector *vector, size_t newmax);
extern void uvector_clear(uVector *vector);

/* macro function */
#define foreach_uvector(iterator, uVector) \
    for(uVector_Obj *iterator;iterator = get_next(uVector);)

struct uvector_{

    /* simple encapsulated member functions */

    bool (*addfirst)(uVector *vector, const void *object);
    bool (*addlast)(uVector *vector, const void *object);
    bool (*addat)(uVector *vector, int index, const void *object);

    void *(*getat)(uVector *vector, int index, bool newmem);

    bool (*removefirst)(uVector *vector);
    bool (*removelast)(uVector *vector);
    bool (*removeat)(uVector *vector, int index);

    void *(*popfirst)(uVector *vector);
    void *(*poplast)(uVector *vector);
    void *(*popat)(uVector *vector, int index);

    bool (*setat)(uVector *vector, int index, const void * object);
    bool   (*resize)(uVector *vector, size_t newmax);

    void (*free)(uVector *vector);

    /* private variable - do not access directly */
    void *data;
    size_t num; /* number of elements*/
    size_t objsize; /* the size of each element */
    size_t max; /* allocated number of elements */
};

struct uvector_obj_{
    void *data;
    int index;
};
#endif //UMANGA_UVECTOR_H
