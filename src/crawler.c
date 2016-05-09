/* ========================================================================== */
/* File: crawler.c - Tiny Search Engine web crawler
 *
 * Author: Victoria Taylor
 * Date: 
 *
 * Input:./crawler \
 *
 * Command line options:
 *
 * Output:
 *
 * Error Conditions:
 *
 * Special Considerations:
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           // printf

#include <curl/curl.h>                       // curl functionality
#include <sys/stat.h>                        // stat functionality
#include <string.h>                          //to use strlen()
#include <unistd.h>

// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "web.h"                             // curl and html functionality
#include "list.h"                            // webpage list functionality
#include "hashtable.h"                       // hashtable functionality
#include "utils.h"                           // utility stuffs

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables
 struct stat buf;
 char *target_Directory;
 int MaxCrawlingdepth;

// ---------------- Private prototypes
int WriteFile(WebPage *page, char *target_Directory, char *filename);
int CrawlPage(WebPage *page, HashTable *visited, List *WebPageList, int depth);
/* ========================================================================== */

int main(int argc, char* argv[])
{

    // CHECK COMMAND LINE ARGUMENTS
    if ( 4 != argc) {
        printf("Usage: ./crawler [SEED URL] [TARGET DIRECTORY TO DEPOSIT DATA] [CRAWLING DEPTH]\n");
        exit(1);
    }
    target_Directory = argv[2];
    stat(target_Directory, &buf);
    if (!S_ISDIR(buf.st_mode)) {
        printf("Usage: invalid directory\n");
        exit(1);
    }
    if (!(buf.st_mode & S_IWUSR)){
        printf("Usage: directory must have write permissions\n");
        exit(1);
    }
    if (sscanf(argv[3],"%d", &MaxCrawlingDepth) !=1){
        printf("Usage: CrawlDepth must be an int");
        exit(1);
    }
    if (MaxCrawlingDepth > MAX_DEPTH) {
        printf("Usage: CrawlDepth will not exceed 4\n");
        MaxWebPageDepth = 4;
    }
    if (strncmp(argv[1], URL_PREFIX, 41) != 0){
        printf("Usage: URL must be under the domain http://old-www.cs.dartmouth.edu/~cs50/tse\n");
        exit(1);
    }

    // init curl
    curl_global_init(CURL_GLOBAL_ALL);

    //initialize data structures/variables
    struct List *WebPageList = malloc(sizeof(List));
    struct HashTable *Visited = malloc(sizeof(HashTable));
 
    // setup seed page
    struct WebPage *page = malloc(sizeof(WebPage)); //create webpage and put in url
    page->url = argv[1];
    page->depth= 0; 

    // get seed webpage
    if (GetWebPage(page) == 0) {
        fprintf(stdin, "Webpage retrieval failed.");
        exit(1);
    }

    // write seed file
    int filenum = 1;
    char *filename = malloc(5*sizeof(char));
    sprintf(filename, "%d", filenum);
    WriteFile(page, target_Directory, filename);

    // add seed page to hashtable
    AddToHashTable(page->url, Visited);

    // extract urls from seed page and add to URLList
    CrawlPage(page, Visited, WebPageList, MaxWebPageDepth);

    while (WebPageList->head != NULL){
        struct WebPage *nextPage = PopList(WebPageList);
        if (GetWebPage(nextPage) != 0 ){
            filenum++;
            sprintf(filename, "%d", filenum);
            WriteFile(nextPage, target_Directory, filename);
            CrawlPage(nextPage, Visited, WebPageList, MaxWebPageDepth);
            free(nextPage->html);
            free(nextPage);
           // sleep(INTERVAL_PER_FETCH);
        }

    }

    // cleanup curl
    curl_global_cleanup();

    //free resources
    HashTableFree(Visited);
    free(WebPageList);
    free(page->html);
    free(page);

    return 0;
}
/* Takes a Webpage, the directory where the files will be written to, and a filename
    Writes the url, the depth, and the html of the file and saves it under filename
    in the given directory.
*/
int WriteFile(WebPage *page, char *target_Directory, char *filename) 
{
    const size_t path_size = strlen(target_Directory) + strlen("/") + strlen(filename) + 1;
    char *path = malloc(path_size);
    snprintf(path, path_size, "%s/%s", target_Directory, filename);
    FILE *fp = fopen(path, "w");
    fprintf(fp, "%s\n", page->url);
    fprintf(fp, "%d\n", page->depth);
    fprintf(fp, "%s", page->html);
    fclose(fp);
    free(path);
    return 1;

}

/* Crawls through HTML of a page and extracts any URLS found. If the URL is under
    the right domain, and isn't already in the Hashtable, it is added to the 
    Hashtable and the the WebPageList
*/
int CrawlPage(WebPage *page, HashTable *Visited, List *WebPageList, int MaxWebPageDepth){
    int pos = 0;
    char *result; 
   
    int currdepth = page->depth + 1;
    if (currdepth > MaxWebPageDepth){
        return 1;
    }
    while ((pos = GetNextURL(page->html,  pos, page->url, &result)) > 0){
        NormalizeURL(result);

        if (strncmp(result, URL_PREFIX, 41) == 0){

            if(!InHashTable(result, Visited)){
                AddToHashTable(result, Visited);
                struct WebPage *NewPage = malloc(sizeof(WebPage));
                NewPage->url = result;
                NewPage->depth = currdepth;
                AppendList(NewPage, WebPageList);
            }
        }
    }
    return 1;
}

