#ifndef UMANGA_UMANGA_H
#define UMANGA_UMANGA_H

#include <stdlib.h>
#include <ustring.h>
#include <ufile.h>
#include <jsonc/json.h>
#include <jsonc/json_tokener.h>
#include <jsonc/json_visit.h>
#include <curl/curl.h>
#include <uvector.h>
#include <uv.h>
#include <unistd.h>

typedef struct umanga_ uManga;
typedef struct download_work_holder DWN_WRK_HLDER;

#define MANGA_FILE_NAME "db_manga.json"

/* CURL */
CURL *curl;
CURLcode res;

/* UV */
uv_loop_t *loop;

/* public function */
extern uManga *uManga_new(void);
extern void set_manga_info(uManga *mng, const char *name);
extern void download_manga_chapter(const char *name, const char *chapter);
extern void download_chapter(uManga *mng, char *chapter);
extern void free_uManga(uManga *mng);

/* private function */
uString *get_all_manga();
json_object *get_object_of_manga(const char *name,const char *buffer);
uVector *get_urls_page(const char *url);
void start_download_pages(uVector *vec,const char *name_manga, const char *num_chapter);
void start_download_chapter(uManga *mng,char *chapter);
void retrieve_and_save_pages(const char *name,char *chapter,uVector *vec);
void save_image(int num_page,const char *folder,const char *chapter, const char *url);
void free_manga_details(uManga *mng);

/* callback functions */

// WRITEFUNCTION callback
size_t getBuffers(char *in, size_t size, size_t nmemb, void *out);
size_t getBufImage(char *in, size_t size, size_t nmemb, void *out);

// UV functions
void download(uv_work_t *req);
void after_download(uv_work_t *req, int status);

/* strucutres */

struct umanga_{
    uString *id;
    uString *num_of_chapters;
    uString *name;
    uString *author;
    uString *genre;
};

struct download_work_holder{
    int num_of_page;
    uString *name;
    uString *chapther;
    uString *url;
};
#endif