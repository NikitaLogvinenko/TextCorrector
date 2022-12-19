#ifndef _CORRECTOR_MODEL_
#define _CORRECTOR_MODEL_

typedef void* Pointer;
typedef void (*Destructor)(Pointer);

/*
Редактирующая модель.
Хранит максимальную длину слова, параметры редактирования(treshold, tol), массив связных списков(каждый список - слова определённой длины), функцию очистки списков
*/
typedef struct Corrector
{
	int max_word_size;
	int treshold;
	Destructor dtor;
} Corrector;

#endif

