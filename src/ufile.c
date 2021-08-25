//
// Created by Ahmed Alkabir on 10/1/2017.
//
#include <ufile.h>
#include <sys/stat.h>

/** Check File if exist
 *  @param : file_name : name of file
 *  return true in case file if exist
 *          otherwise return false
 *
 *  F_OK = 0;
 **/
bool ufile_exist(const char *file_name){
    if(access(file_name,F_OK) == 0)
        return true;
    return false;
}

/* read file from the stream */
void *ufile_read(FILE *fp, size_t *nbytes){
    size_t memsize = 1024;
    size_t size = 0;

    if(nbytes != NULL && *nbytes > 0){
        memsize = *nbytes;
        size = *nbytes;
    }

    int c;
    size_t  c_count;
    char *data = NULL;
    for(c_count = 0; (c = fgetc(fp)) != EOF;){
        if(size > 0 && c_count == size)
            break;

        if(c_count == 0){
            data = (char *)malloc(sizeof(char) * memsize);
            if(data == NULL){
                return NULL;
            }
        }else if(c_count == memsize -1){
            memsize *= 2;

            /* Here, we don't use realloc() sometimes it is unstable */
            char *datatemp = (char *)malloc(sizeof(char) * (memsize + 1));
            if(datatemp == NULL){
                free(data);
                return NULL;
            }
            memcpy(datatemp, data, c_count);
            free(data);
            data = datatemp;
        }
        data[c_count++] = (char) c;
    }

    if(c_count == 0 && c == EOF)
        return NULL;
    data[c_count] = '\0';

    if(nbytes != NULL)
        *nbytes = c_count;

    return (void *) data;
}

/** save file at specfied directory */
ssize_t ufile_save(const char *filepath, const void *buf, size_t size, bool append){
    int fd;

    if(append == false){
        fd = open(filepath, O_CREAT | O_WRONLY | O_TRUNC,
                  (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
    }else{
        fd = open(filepath, O_CREAT | O_WRONLY | O_APPEND,
                  (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
    }

    if(fd < 0)
        return -1;

    ssize_t count = write(fd, buf, size);
    close(fd);

    return count;
}

/** check folder or directory if exists
 *  @param: folder_name : name of folder
 *                        or directory
 * */
bool ufile_dir_exist(const char *folder_name){
    struct stat sb;
    if(stat(folder_name, &sb) == 0 && S_ISDIR(sb.st_mode)){
        return true;
    }else{
        return false;
    }
}

/**  Attempts to create a directory recursively
 *   TODO: add recursive support
 *   TODO : add mode permissions for linux platform
 * */
bool ufile_mkdir(const char *dirpath, mode_t mode, bool recursive){
    if(mkdir(dirpath) == 0)
        return true;
}
