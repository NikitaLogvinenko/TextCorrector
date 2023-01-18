#ifndef _MARKED_LIST_LIB_
#define _MARKED_LIST_LIB_


#include "marked_list.h"
#include <stdbool.h>


// Создать связный список c ключом word и счётчиком, начинающемся с counter (он не обязательно 1, если мы загружаем уже обученную модель из файла)
// Создаётся только указатель на ключ, память не копируется
MList* mlist_create(const char* word, unsigned counter);

// Очистить связный список
void mlist_destroy(MList* ml);

// Вернуть число элементов в списке
unsigned mlist_length(const MList* ml);

// Добавить элемент по ключу в список, вернуть указатель на новое начало (т.к. теперь может быть новая голова). 
// Счётчик установить в counter (т.к. можем загружать модель из файла)
// Если такой ключ уже был в списке - прибавить к счётчику существующего элемента counter
// Если такого ключа еще не было - вставить новую голову с counter
// В списке создаются указатель на динамическую память с ключом word; под counter выделяется статическая память
MList* mlist_add(MList* ml, const char* word, unsigned counter);

// Вернуть указатель на данные элемента по его ключу; если такого ключа нет - вернуть NULL
// Возвращается указатель на данные, поэтому при изменении этих данных извне поменяются также данные в списке
unsigned* mlist_get(const MList* ml, const char* word);

// Проверить, есть ли слово word в списке
bool mlist_has(const MList* ml, const char* word);

// Вернуть указатель на элемент со словом word; если такого нет - вернуть NULL
MList* mlist_find(const MList* ml, const char* word);

// Удалить элемент по ключу word; вернуть указатель на новое начало списка
MList* mlist_remove(MList* ml, const char* word);

// К счётчикам списка применить функцию func
void mlist_foreach_counter(MList* ml, void (*func)(unsigned* number));

// К ключам и данным списка применить функцию func
void mlist_foreach_item(MList* ml, void (*func)(const char* word, unsigned* number));

// Вывести список в формате `слово: счётчик`; выделить список сверху и снизу полосой из дефисов
void print_mlist(MList* ml);


#endif