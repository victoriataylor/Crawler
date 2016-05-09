/* ========================================================================== */
/* File: hashtable.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * Author:
 * Date:
 *
 * You should include in this file your functionality for the hashtable as
 * described in the assignment and lecture.
 */
/* ========================================================================== */

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <string.h>                          // strlen
#include <stdio.h>
#include <stdlib.h>

// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "hashtable.h"                       // hashtable functionality

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

unsigned long JenkinsHash(const char *str, unsigned long mod)
{
    size_t len = strlen(str);
    unsigned long hash, i;

    for(hash = i = 0; i < len; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % mod;
}
/*  Takes a URL and adds it to a HashTable using the JenkinsHash
    Handles for collisions by creating singly linked lists
*/
int AddToHashTable(char *URL, HashTable *URLHash)
{
    struct HashTableNode *HashNode = malloc(sizeof(HashTableNode));
    HashNode->url = URL;

    int hashIndex = JenkinsHash(URL, MAX_HASH_SLOT); 

    if (URLHash->table[hashIndex] == NULL){ //then it is unique
        URLHash->table[hashIndex] = HashNode;
    }
    else {
        struct HashTableNode *tempNode = URLHash->table[hashIndex];
        while (tempNode->next != NULL){
            tempNode = tempNode->next;
        }
        tempNode->next = HashNode;
    }
    HashNode->next = NULL;
    return 0;
}

/* Checks to see if the URL is already in the Hashtable. Returns 0 if it
is not, and 1 if it is
*/
int InHashTable(char *URL,HashTable *URLHash)
{
    int hashIndex = JenkinsHash(URL, MAX_HASH_SLOT); //or maybe *URL?

    if (URLHash->table[hashIndex] == NULL) {
        return 0;
    }
    else{
        struct HashTableNode *tempNode = URLHash->table[hashIndex];
        while (tempNode != NULL) {
            if (strcmp(tempNode->url, URL) == 0){
                return 1;
            }
            tempNode=tempNode->next;
        }
        return 0;
    }
}
//Frees all space previously allocated to the Hashtable and HashTableNodes
int HashTableFree(HashTable *URLHash){
    for (int x = 0; x < MAX_HASH_SLOT; x++){
        if (URLHash->table[x] != NULL){
            struct HashTableNode *list = URLHash->table[x];
            while (list!= NULL){
                struct HashTableNode *temp = list;
                list = list->next;
                free(temp);
            }
        }
    }
    free(URLHash);
    return 0; 
}

