#ifndef _LINKED_LIST_
#define _LINKED_LIST_


#include <stdio.h>


typedef void* Pointer;

// Связный список. Хранит размер данных, указатель на данные, указатель на следующий элемент списка
typedef struct Linked_list
{
	size_t data_size; // число хранимых байтов по указателю data
	void* data; // хранимые данные, функция очистки данных хранится в модели, она же их и чистит
	struct Linked_list* next; // указатель на следующий элемент списка
} Linked_list;

#endif