#ifndef _CORRECTOR_MODEL_
#define _CORRECTOR_MODEL_


#include "hash_table.h"


typedef void* Pointer;
typedef int (*WordsMetric)(const char*, const char*); // функция нахождения разности слов (метрика в пространстве слов)


/*
Редактирующая модель.
Хранит максимальную длину слова, массив хеш-таблиц (каждая таблица - изученные слова определённой длины), функцию вычисления разности слов (хеш-функция Дженкинса)
Очистка таблиц реализована в библиотеке hash_table_lib
*/
typedef struct Corrector
{
	unsigned max_word_length;
	HashTable* dictionary;
	WordsMetric metric;
} Corrector;

#endif

