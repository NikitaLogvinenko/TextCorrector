#ifndef _CORRECTOR_MODEL_
#define _CORRECTOR_MODEL_


#include "linked_list.h"


typedef void (*Destructor)(Pointer);  // деструктор
typedef int (*WordsMetric)(const char*, const char*); // функция нахождения разности слов (метрика в пространстве слов)


/*
Редактирующая модель.
Хранит максимальную длину слова, массив связных списков (каждый список - изученные слова определённой длины), функцию вычисления разности слов, функцию очистки списков
*/
typedef struct Corrector
{
	int max_word_size;
	Linked_list** dictionary;
	WordsMetric metric;
	Destructor dtor;
} Corrector;

#endif

