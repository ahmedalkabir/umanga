//
// Created by korab on 10/1/2017.
//

#include <stdbool.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <winsock2.h>
#include <windows.h>
#include <sys/types.h>

#ifndef UMANGA_UFILE_H
#define UMANGA_UFILE_H

// Check File if exist
extern bool ufile_exist(const char *file_name);
// check directory(or folder) if exist
extern bool ufile_dir_exist(const char *folder_name);
extern bool ufile_mkdir(const char *dirpath, mode_t mode, bool recursive);
extern void *ufile_read(FILE *fp, size_t *nbytes);
extern ssize_t ufile_save(const char *filepath, const void *buf, size_t size,bool append);

#endif //UMANGA_UFILE_H
