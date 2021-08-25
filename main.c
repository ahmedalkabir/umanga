//
// Created by Ahmed Alkabir on 9/27/2017.
//

#include <stdio.h>
#include <uv.h>
#include <stdlib.h>
#include <umanga.h>
//
//void del_additional_char(char *str){
//    void *head = (unsigned char *)str;
//    int length = strlen(str);
//    int i =1;
//    while(*str++ != '\0'){
//        if(i == length - 1){
//            *str++ = '\0';
//            break;
//        }
//        i++;
//    }
//    str = head;
//}


int main() {
    uManga *MangaIF = uManga_new();
    set_manga_info(MangaIF, "Naruto");
    printf("Name of Manga : %s\n", MangaIF->name->value);
    printf("Author of Manga: %s\n", MangaIF->author->value);
    printf("Number of Chapters: %s\n", MangaIF->num_of_chapters->value);
    printf("Genre of Manga: %s\n", MangaIF->genre->value);
    download_chapter(MangaIF, "599");
    free_uManga(MangaIF);
    return 0;
}