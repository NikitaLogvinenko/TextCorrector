#ifndef _LL_LIB_
#define _LL_LIB_

#include "linked_list.h"
#include <stdio.h>
#include <stdbool.h>


// Создать связный список с данными data, которые занимают data_size байтов
// Вернуть указатель на голову списка
Linked_list* llist_create(const Pointer data, size_t data_size);

// Удалить связанный список
void llist_destroy(Linked_list* ll);

// Cкопировать список, вернуть указатель на начало списка-копии
Linked_list* llist_copy(const Linked_list* ll);

// Вернуть число элементов в списке
int llist_length(const Linked_list* ll);

// Добавить элемент в голову, вернуть указатель на новую голову
Linked_list* llist_prepend(Linked_list* ll, const Pointer data, size_t data_size);

// Вставить элемент в конец, вернуть указатель на новую голову (если передан указатель NULL, то создаётся новый список и хвост является головой)
Linked_list* llist_append(Linked_list* ll, const Pointer data, size_t data_size);

// Вернуть данные элемента по его индексу
// Возвращаем указатель на КОПИЮ данных в списке
Pointer llist_get(const Linked_list* ll, unsigned index);

// Вернуть данные последнего элемента
// Возвращаем указатель на КОПИЮ данных в списке
Pointer llist_get_last(const Linked_list* ll);

// Вернуть индекс первого элемента с данными data; если такого нет - вернуть -1
int llist_find(const Linked_list* ll, const Pointer data, size_t data_size);

// Вернуть индекс первого элемента, для данных которого функция predicate возвращает истину; 
// Записать по указателю custom_data КОПИЮ данных
// Если такого нет - вернуть -1 и ничего не делать с custom_data
int llist_find_custom(const Linked_list* ll, bool (*predicate)(const Pointer data), Pointer const custom_data);

// Удалить элемент по его индексу
// Вернуть указатель на новое начало списка
// Т.к. деструктор данных задан в модели, а не в списке, то перед удалением узла нужно чистить данные извне
Linked_list* llist_remove(Linked_list* ll, unsigned index);

// Удалить первый элемент с данными data
// Вернуть указатель на новое начало списка
// Т.к. деструктор данных задан в модели, а не в списке, то перед удалением узла нужно чистить данные извне
Linked_list* llist_remove_first_equal(Linked_list* ll, const Pointer data, size_t data_size);

// Удалить все элементы с данными data
// Вернуть указатель на новое начало списка
// Т.к. деструктор данных задан в модели, а не в списке, то перед удалением узла нужно чистить данные извне
Linked_list* llist_remove_all_equal(Linked_list* ll, const Pointer data, size_t data_size);

// Присоединить ll2 в хвост ll1
// Вернуть начало объединённого списка
// inplace операция, новых списков не создаётся
Linked_list* llist_concat(Linked_list* ll1, Linked_list* ll2);

// К данным списка применить функцию foreach_func
void llist_foreach(Linked_list* ll,
    void (*func)(Pointer data));

#endif