#ifndef _HASH_TABLE_
#define _HASH_TABLE_


#include "marked_list.h"
#include <stdio.h>


typedef void* Pointer;
typedef unsigned (*HashFunction)(const char* key);


typedef struct HashTable {
    unsigned size;  // размер таблицы (число строк = максимально допустимое количество связных списков)
    unsigned total_words;  // сколько слов хранится в таблице (если слишком много - увеличить размер таблицы при следующем добавлении слова (чтобы не создавать слишком длинных коллизий))
    MList** table;  // списки коллизий
    HashFunction hashfunc;  // функция для подсчёта хеш-суммы, в проекте используется хеш-функция Дженкинса. Можно поменять в файле hash_table_lib.c в функции ht_init
} HashTable;


#endif