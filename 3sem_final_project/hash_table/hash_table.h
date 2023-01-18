//#ifndef _HASH_TABLE_
//#define _HASH_TABLE_
//
//
//#include "marked_list.h"
//#include <stdio.h>
//
//
//typedef void* Pointer;
//typedef unsigned (*HashFunction)(const char* key);
//typedef void (*Destructor)(Pointer);
//
//
//typedef struct HashTable {
//    size_t size;
//    MList** table;
//    HashFunction hashfunc;
//    Destructor dtor;
//} HashTable;
//
//
//#endif