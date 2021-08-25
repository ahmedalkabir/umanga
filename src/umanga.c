//
// Created by Ahmed Alkabir on 9/27/2017.
//
/** TODO: Add uManga Destructor
 * */

#include <umanga.h>



struct myprogress {
    double lastruntime;
    CURL *curl;
};

/* this is how the CURLOPT_XFERINFOFUNCTION callback works */
static int xferinfo(void *p,
                    curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow)
{
    /** TODO: Implement it for gtk+ progress bar
     * */
    return 0;
}

/* constructor of uManga
 * prepare uManga object
 * and download db in case if doesn't exist
 * */
uManga *uManga_new(void){

    uManga *new_manga = (uManga *)malloc(sizeof(uManga));

    // set properties to NULL
    new_manga->id = NULL;
    new_manga->name = NULL;
    new_manga->author = NULL;
    new_manga->num_of_chapters = NULL;
    new_manga->genre = NULL;

    /** Check if manga database file exist
     *  Otherwise download it
     *
     *  Type of Database is document database (JSON)
     * */
    if(ufile_exist(MANGA_FILE_NAME)){
        return new_manga;
    }else{
        /* init the winsock stuffs */
        curl_global_init(CURL_GLOBAL_ALL);

        /* get a curl handle */
        curl = curl_easy_init();

        uString *all_manga = get_all_manga();
        ufile_save(MANGA_FILE_NAME,(char *)all_manga->value,all_manga->length,false);

        // free all_manga object
        all_manga->free(all_manga);

        return new_manga;
    }
}

/*  To Set information of Manga
 * */
void set_manga_info(uManga *mng, const char *name) {
/** Check if manga database file exist
 *  Otherwise download it
 *
 *  Type of Database is document database (JSON)
 * */
    if(ufile_exist(MANGA_FILE_NAME)) {
        FILE *fp = fopen(MANGA_FILE_NAME, "r");
        // get objects of database
        json_object *dump_object = json_tokener_parse((char *) ufile_read(fp, NULL));
        // get mangas array
        json_object *object_manga = get_object_of_manga(
                name, json_object_to_json_string(json_object_object_get(dump_object, "mangas")));

        // make sure its properties  are freed
        free_manga_details(mng);

        // set values to uManga type
        mng->id = uString_new(json_object_to_json_string(json_object_object_get(object_manga, "id")));
        // to delete quotes
        uString_trunchar(mng->id,'"','"');
        mng->name = uString_new(
                json_object_to_json_string(json_object_object_get(object_manga, "name")));
        uString_trunchar(mng->name, '"', '"');

        mng->author = uString_new(
                json_object_to_json_string(json_object_object_get(object_manga, "author")));
        uString_trunchar(mng->author, '"', '"');

        mng->genre = uString_new(
                json_object_to_json_string(json_object_object_get(object_manga, "genre")));
        uString_trunchar(mng->genre, '"', '"');

        mng->num_of_chapters = uString_new(
                json_object_to_json_string(json_object_object_get(object_manga, "chapter_count")));
        uString_trunchar(mng->num_of_chapters, '"', '"');

        fclose(fp);
    }
}

void download_manga_chapter(const char *name_manga, const char *chapter_num){
    if(ufile_exist(MANGA_FILE_NAME)){
        FILE *fp = fopen(MANGA_FILE_NAME, "r");
        // get objects of database
        json_object *dump_object = json_tokener_parse((char *) ufile_read(fp, NULL));
        // to get specified manga by its manga
        json_object *object_manga = get_object_of_manga(
                name_manga, json_object_to_json_string(json_object_object_get(dump_object, "mangas")));

        if(object_manga != NULL){
            uString *url = uString_new("http://json.mangapanda.com/chapter/");
            uString *path_to_save = uString_new(""); // create path_to_save string to hold path of downloaded manga
            const char *id = json_object_to_json_string(json_object_object_get(object_manga, "id"));
            const char *name_manga = json_object_to_json_string(json_object_object_get(object_manga, "name"));
            uString_trunchar((char *)name_manga,'"','"');
            uString_trunchar((char *)id,'"','"');

            // to concatenate strings
            uString_concat(url,(char *)id);
            uString_concat(url,"/");
            uString_concat(url,(char *)chapter_num);

            uString_concat(path_to_save,(char *)name_manga);
            uString_concat(path_to_save, "/");
            uString_concat(path_to_save,(char *)chapter_num);
            uString_concat(path_to_save, "/");

            // get urls of pages of chapter
            uVector *vec = get_urls_page(url->value);
            if(vec != NULL){
                // now start to download
                start_download_pages(vec, name_manga, chapter_num);
            }

            // clean everything
            vec->free(vec);
            url->free(url);
            path_to_save->free(path_to_save);
            free((void *)name_manga);
            free((void *)id);
        }
        fclose(fp);
    }
}
/*  start to download pages of chapter of manga
 * */
void start_download_pages(uVector *vec_urls, const char *name_manga,const char *chapter){
    int counter = 0;
    foreach_uvector(i,vec_urls){
        uString_trunchar(((uString *)(i->data))->value,'"','"');
        ((uString *)(i->data))->trim_character((uString *)(i->data),'\\');
        save_image(++counter,name_manga , chapter,((uString *)i->data)->value);
        printf("Done Downloaded %d\n",counter);
    }
}
/*  get vector of urls
 * */
uVector *get_urls_page(const char *url){
    // make uVector object
    uVector *new_vector;

    // we're going to use buf to store buffer of url
    // as of flexibility of uString object and its member functions
    // just check uUtils Documentation
    uString *buf = uString_new("");

    /* initialize curl handles */

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, url);
        // just we made getBuffers as callback function of receiving
        // data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getBuffers);
        // we made buf custom pointer passed to the write callback
        // just curl documentation for more info
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);

        // perform the task
        res = curl_easy_perform(curl);

        json_object *object_chapters = json_object_object_get(json_tokener_parse(buf->value),"chapters");
        // retrieve list of chapters to use its length
        array_list *list_chapters = json_object_get_array(object_chapters);

        if(list_chapters != NULL){
            new_vector = uVector_new(list_chapters->length,sizeof(uString),0);

            for(int i=1;i <= list_chapters->length; i++){
                new_vector->addlast(new_vector,uString_new(
                        json_object_to_json_string(json_object_object_get(
                                array_list_get_idx(list_chapters,i - 1),"imgurl"
                        ))
                ));
            }

            if(res != CURLE_OK){
                fprintf(stderr, "1 curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));
                return NULL;
            }
            // clean stuffs
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            buf->free(buf);

            return new_vector;
        }else{
            return NULL;
        }
    }
}
/* return list of all manga in uString object form */
uString *get_all_manga(){

    // to hold buffers of manga database
    uString *buf = uString_new("");

    struct myprogress prog;

    if(curl){
        prog.lastruntime = 0;
        prog.curl = curl;

        curl_easy_setopt(curl, CURLOPT_URL, "http://json.mangapanda.com/list/popular");
        // callback for writing received data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getBuffers);
        // set callback progress
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
        // TODO: uncomment it in case using gui
//        //pass the struct pointer into the xferinfo function
//        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
//        // custom pointer passed to the write callback
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);
//        // switch on the progress meter
//        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return buf;
}

json_object *get_object_of_manga(const char *name,const char *buffer){

    // object to returr
    json_object *manga_object;

    // parse the buffer to json
    json_object *new_obj = json_tokener_parse(buffer);

    // all list of manga
    array_list *list_manga = json_object_get_array(new_obj);

    // variables to hold at lowercase
    char *name_manga_dest;
    char *name_to_get;

    for(int i=1;i <= list_manga->length ; i++){
        uString *str = uString_new(json_object_to_json_string(
                json_object_object_get(array_list_get_idx(list_manga, i-1),"name")));

        // delete quotes '"'
        uString_trunchar(str,'"','"');

        //lower case to name to make comparision easier
        name_manga_dest = ustring_tolower(str->value);
        name_to_get = ustring_tolower(name);

        // check if matched otherwise return NULL
        if((strcmp(name_manga_dest, name_to_get)) == 0){
            manga_object = array_list_get_idx(list_manga, i - 1);
            break;
        }
        else{
            manga_object = NULL;
        }

        // free the str object to prevent from memory leaks
        free(name_manga_dest);
        free(name_to_get);
        str->free(str);
    }

    return manga_object;

}

/* download specified chapter
 * TODO:1 add thread support for speed
 * TODO:2 make it can download range of chapters
 * */
void download_chapter(uManga *mng,char *chapter){
    if(mng != NULL){
        if(ufile_dir_exist(mng->name->value)){
            // to start download chapter after
            // checking folder if exist or
            // create new one
            start_download_chapter(mng,chapter);
        }else{
            if(ufile_mkdir(mng->name->value,0,false)){
                start_download_chapter(mng,chapter);
            }
        }
    }
}

/** start download chapter
 * */
void start_download_chapter(uManga *mng,char *chapter){

    uVector *vec;
    uString *Buf = uString_new("");
    char *url = (char *)malloc(strlen("http://json.mangapanda.com/chapter/9999/999") + 1);
    sprintf(url,"http://json.mangapanda.com/chapter/%s/%s",mng->id->value,chapter);

    /* initialize curl handles */

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getBuffers);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, Buf);

        res = curl_easy_perform(curl);

        json_object *object_chapters = json_object_object_get(json_tokener_parse(Buf->value),"chapters");
        array_list *list_chapters = json_object_get_array(object_chapters);

        if(list_chapters != NULL){
            vec = uVector_new(list_chapters->length,sizeof(uString),0);

            for(int i=1;i <= list_chapters->length; i++){
                vec->addlast(vec,uString_new(
                        json_object_to_json_string(json_object_object_get(
                                array_list_get_idx(list_chapters,i - 1),"imgurl"
                        ))
                ));
            }

        }else{
            return;
        }
        if(res != CURLE_OK)
            fprintf(stderr, "1 curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    /** start download images
     *  First Argument : Name of Manga
     *  Second Argument : Chapter to download
     *  Third Argument : uVector object is just array of url of chapter to download
     * */
    retrieve_and_save_pages(mng->name->value,chapter,vec);

    free(url);
    Buf->free(Buf);
    vec->free(vec);

}

/** start download images */
void retrieve_and_save_pages(const char *name,char *chapter,uVector *vec){
    loop = (uv_loop_t *)malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);

    DWN_WRK_HLDER worker_data[vec->num];
    uv_work_t req[vec->num];
    int num_of_page = 0;
    foreach_uvector(i,vec){
        uString_trunchar((uString *)i->data,'"','"');
        ((uString *)(i->data))->trim_character((uString *)(i->data),'\\');
//        worker_data[num_of_page].num_of_page = num_of_page;
//        worker_data[num_of_page].name = uString_new(name);
//        worker_data[num_of_page].chapther = uString_new(chapter);
//        worker_data[num_of_page].url = uString_new(((uString *)i->data)->value);
//
//        req[num_of_page].data = (void *)&worker_data[num_of_page];
//        uv_queue_work(loop, &req[num_of_page], download, after_download);

//        num_of_page++;
        save_image(++num_of_page,name,chapter,((uString *)i->data)->value);
        printf("%s ==== Chapter : %s , Page : %d downloaded :) \n",name,chapter,num_of_page);
    }

    // start download the chapter
    uv_run(loop, UV_RUN_DEFAULT);

    // clean the uv objects
    uv_loop_close(loop);
    free(loop);
}

/** save image
 * */
void save_image(int num_page,const char *folder_name,const char *chapter, const char *url){
    CURL *curl_img;
    CURLcode res_img;
    printf("%d\n",num_page);
    char *string_ch = (char *)malloc(strlen(folder_name) + 1);
    sprintf(string_ch,"%s/%s", folder_name, chapter);
    if(ufile_mkdir(folder_name,0,NULL)){
        ufile_mkdir(string_ch, 0, NULL);
        char *full_path = (char *)malloc(strlen(string_ch) + 4);
        sprintf(full_path,"%s/%d.jpg",string_ch,num_page);
        FILE *save_file = fopen(full_path,"wb+");


        curl_global_init(CURL_GLOBAL_ALL);

        curl_img = curl_easy_init();

        printf("%s\n",url);
        if(curl_img){
            curl_easy_setopt(curl_img, CURLOPT_URL, url);
            curl_easy_setopt(curl_img, CURLOPT_WRITEFUNCTION, getBufImage);
            curl_easy_setopt(curl_img, CURLOPT_WRITEDATA, save_file);

            res = curl_easy_perform(curl);

            if(res != CURLE_OK)
                fprintf(stderr, "ERORR NUM : %d \t curl_easy_perform() failed: %s\n",
                        res,curl_easy_strerror(res));
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
        free(string_ch);
        free(full_path);
        fclose(save_file);
    }else{
        fprintf(stderr, "Error , Cannot create the directory");
    }

}

/* free uManga object */
void free_uManga(uManga *mng){
    if(mng != NULL){
        // first : free the details of manga
        free_manga_details(mng);
        // second : free the uManga object
        free(mng);
    }
}

/* free uManga properties to reuse*/
void free_manga_details(uManga *mng){
    if(mng->id != NULL){
        mng->id->free(mng->id);
        mng->name->free(mng->name);
        mng->num_of_chapters->free(mng->num_of_chapters);
        mng->author->free(mng->author);
        mng->genre->free(mng->genre);
    }
}

/* callback functions */

// WRITEFUNCTION callback
size_t getBuffers(char *in, size_t size, size_t nmemb, void *out){
    size_t res = size*nmemb;
    uString_concat((uString *)out,in);
    return res;
}

size_t getBufImage(char *in, size_t size, size_t nmemb, void *out){
    size_t res = size*nmemb;
    fwrite(in,size,nmemb,out);
    return res;
}

// UV functions
void download(uv_work_t *req){
    printf(" Name of Chapter %d\n", ((DWN_WRK_HLDER *)req->data)->num_of_page);
//    save_image(((DWN_WRK_HLDER *)req->data)->num_of_page+1,
//               ((DWN_WRK_HLDER *)req->data)->name->value,
//               ((DWN_WRK_HLDER *)req->data)->chapther->value,
//               ((DWN_WRK_HLDER *)req->data)->url->value);
}

void after_download(uv_work_t *req, int status){
    printf("Clean out of this shit\n");
    ((DWN_WRK_HLDER *)req->data)->num_of_page = 0;
    ustring_dellacate(((DWN_WRK_HLDER *)req->data)->name);
    ustring_dellacate(((DWN_WRK_HLDER *)req->data)->chapther);
    ustring_dellacate(((DWN_WRK_HLDER *)req->data)->url);
}