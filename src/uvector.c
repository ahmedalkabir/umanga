//
// Created by Ahmed Alkabir on 9/28/2017.
//

#include <uvector.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/**
 *  Create new uVector Container
 *
 *  notes:
 *      1 - EINVAL : Invalid argument
 *      2 - ENOMEM : Memory allocation failure
 * */
uVector *uVector_new(size_t max, size_t objsize, int options){
    if(objsize == 0){
        errno = EINVAL;
        return NULL;
    }

    uVector *vector = (uVector *)calloc(1, sizeof(uVector));
    if(vector == NULL){
        errno = ENOMEM;
        return NULL;
    }

    if(max == 0){
        vector->data = NULL;
        vector->num = 0;
        vector->max = 0;
        vector->objsize = objsize;
    }else{
        void *data = malloc(max * objsize);
        if(data == NULL){
            free(vector);
            errno = ENOMEM;
            return NULL;
        }

        vector->data = data;
        vector->num = 0;
        vector->objsize = objsize;
        vector->max = max;
    }


    vector->addat = uvector_addat;
    vector->addfirst = uvector_addfirst;
    vector->addlast = uvector_addlast;

    vector->getat = uvector_getat;

    vector->removefirst = uvector_removefirst;
    vector->removelast = uvector_removelast;
    vector->removeat = uvector_removeat;

    vector->popfirst = uvector_popfirst;
    vector->poplast = uvector_poplast;
    vector->popat = uvector_popat;

    vector->setat = uvector_setat;
    vector->resize = uvector_resize;

    vector->free = uvector_clear;

}

/**
 *  Inserts a element at the specified position in this vector
 * */
bool uvector_addat(uVector *vector, int index, const void *data){
    /// check arguemnts
    if(data == NULL){
        errno = EINVAL;
        return false;
    }

    /// check index
    if(index < 0){
        index += vector->num;
    }
    if(index > vector->num){
        errno = ERANGE;
        return false;
    }

    /// check whether the vector is full
    if(vector->num >= vector->max){
        /** TODO : imporove the resize implemention
         * */
        if(vector->num >= vector->max){
            size_t newmax = vector->max + 1;

            bool result = vector->resize(vector, newmax);
            if(result == false){
                errno = ENOMEM;
                return false;
            }
        }
    }

    ///shift data from index...(num - 1)  to index + 1...num
    for(int i = vector->num; i > index; i--){
        void *dst = (unsigned char *)vector->data + vector->objsize * i;
        void *src = (unsigned char *)vector->data + vector->objsize * (i - 1);

        memcpy(dst, src, vector->objsize);
    }

    void *add = (unsigned char *)vector->data + index * vector->objsize;
    memcpy(add, data, vector->objsize);
    vector->num++;

    return true;
}

/** Insertt a element at the beginning of this vector.
 * */
bool uvector_addfirst(uVector *vector, const void *data){
    return vector->addat(vector, 0, data);
}

/** Insert element at the end of this vector
 * */
bool uvector_addlast(uVector *vector, const void *data){
    return vector->addat(vector, vector->num, data);
}

/** Changes the allocated memory space size.
 * */
bool uvector_resize(uVector *vector, size_t newmax){

    if(newmax == 0){
        free(vector->data);
        vector->data = NULL;
        vector->max = 0;
        vector->num = 0;
        vector->objsize = 0;

        return true;
    }

    void *newdata = realloc(vector->data, newmax * vector->objsize);

    if(newdata == NULL){
        errno = ENOMEM;
        return false;
    }

    vector->data = newdata;
    vector->max = newmax;
    if(vector->num > newmax){
        vector->num = newmax;
    }
    return true;
}

/** get specific object by index
 * */
void *uvector_getat(uVector *vector, int index, bool newmem){
    if(index < 0){
        index += vector->num;
    }
    if(index >= vector->num){
        if(vector->num == 0){
            errno = ENOENT;
            return NULL;
        }else{
            errno = ERANGE;
            return NULL;
        }
    }

    void *src_data = (unsigned char *)vector->data + index * vector->objsize;
    if(newmem){
        void *dump_data = malloc(vector->objsize);
        if(dump_data == NULL){
            errno = ENOMEM;
            return NULL;
        }else{
            memcpy(dump_data, src_data, vector->objsize);
            return dump_data;
        }
    }else{
        return src_data;
    }
}

/** Set value at specific index
 *
 * */
bool uvector_setat(uVector *vector, int index, const void * data){
    void *old_data = uvector_getat(vector, index, false);
    if(old_data == NULL){
        return false;
    }
    memcpy(old_data, data, vector->objsize);

    return true;
}

/** Removes the element at the specified position in this vector
 * */
bool uvector_removeat(uVector *vector, int index){
    if(index < 0){
        index += vector->num;
    }
    if(index >= vector->num){
        if(vector->num == 0){
            errno = ENOENT;
            return false;
        }else{
            errno = ERANGE;
            return false;
        }
    }

    for(int i = index + 1; i < vector->num; i++){
        void *src = (unsigned char *)vector->data + i * vector->objsize;
        void *dst = (unsigned char *)vector->data + (i - 1) * vector->objsize;

        memcpy(dst,src, vector->objsize);
    }

    vector->num--;

    return true;
}
/** Removes the first element in this vector
 * */
bool uvector_removefirst(uVector *vector){
    return vector->removeat(vector, 0);
}

/** Removes the last element in this vector
 * */
bool uvector_removelast(uVector *vector){
    return vector->removeat(vector, -1);
}

/** Returns and remove the element at specified
 *  position in this vector
 * */
void *uvector_popat(uVector *vector, int index){
    void *data = vector->getat(vector, index, true);
    if(data == NULL){
        return NULL;
    }

    bool result = vector->removeat(vector, index);
    if(result == false){
        free(data);
        return NULL;
    }
    return data;
}

/** Returns and remove the first element in this vector
 * */
void *uvector_popfirst(uVector *vector){
    return vector->popat(vector, 0);
}

/** Returns the last element of this vector and delete it
 * */
void *uvector_poplast(uVector *vector){
    return vector->popat(vector, -1);
}

/**  Get next element in this vector
 * */
uVector_Obj *get_next(uVector *vector){
    static int index = 0;
    if(vector == NULL || index == vector->num){
        return NULL;
    }else{
        uVector_Obj *obj = (uVector_Obj *)malloc(vector->objsize);

        void *data = (unsigned char *)vector->data + index * vector->objsize;

        void *dump = malloc(vector->objsize);
        if(dump == NULL){
            errno = ENOMEM;
            return NULL;
        }
        memcpy(dump, data,vector->objsize);
        obj->data = data;

        index++;

        return obj;
    }
}

/** Remove all the elements in this vector
 */
void uvector_clear(uVector *vector){
    free(vector->data);
    vector->data = NULL;
    vector->num = 0;
    vector->objsize = 0;
    vector->max = 0;
}