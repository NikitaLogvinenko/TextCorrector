#ifndef _HASH_TABLE_LIB_
#define _HASH_TABLE_LIB_


#include "hash_table.h"
#include <stdio.h>
#include <stdbool.h>


// Хеш-функция Дженкинса
unsigned jenkins_one_at_a_time_hash(const char* key);

// Инициализировать таблицу.
// Таблица уже должна быть создана! Мы только инициализируем её
// size - размер базового массива;
// hf   - хеш-функция;
// Если hf=NULL, берется стандартная функция (Дженкинса).
// Возвращает 0 в случае успешной инициализации. В случае проблем с выделением памяти возвращает -1
int ht_init(HashTable* ht, unsigned size, HashFunction hf);

// Уничтожить таблицу
// Очищение памяти самого объекта типа HashTable должно выполняться ВРУЧНУЮ! Т.к. пользователь, возможно, выделял память под объект статически
void ht_destroy(HashTable* ht);

// Записать в таблицу соответствие word -> counter. Если word уже существовал, прибавить counter к существующему счётчику
// Если в таблице становится слишком много слов, то сначала таблица увеличивается в несколько раз, чтобы уменьшит число коллизий (для более быстрого поиска в дальнейшем)
// Порог, во сколько раз количество слов должно превышать количество строк в таблице, чтобы началось её расширение, задан через #define WRDS_EXCEED
// Во сколько раз увеличивать таблицу задано через #define HT_INCRS
void ht_set(HashTable* ht, const char* word, unsigned counter);

// Получить УКАЗАТЕЛЬ на счётчик слова. Если слова нет в таблице, вернуть NULL
unsigned* ht_get(const HashTable* ht, const char* word);

// Проверка существования ключа word в таблице
bool ht_has(const HashTable* ht, const char* word);

// Удалить элемент с ключом word из таблицы (если он есть)
void ht_delete(HashTable* ht, const char* word);

//  Обход таблицы с посещением всех элементов. Функция func будет вызвана для всех пар (word, counter) из таблицы. Ключ менять нельзя!
void ht_traverse(HashTable* ht, void (*func)(const char* word, unsigned* counter));

// Изменить размер базового массива
void ht_resize(HashTable* ht, unsigned new_size);

// Записать таблицу в открытый файл opened_file. Формат записи:
// <table_name> size=size_of_the_table total_words=total_amount_of_words_in_the_table
// [row_0] length=length_of_the_row_0
// word1 counter1
// word2 counter2
// ...
// [row_1] length=length_of_the_row_1
// ...
void fprint_ht(FILE* opened_file, HashTable* ht, const char* table_name);

// Записать строчку (список) таблицы в открытый файл opened_file. Формат записи:
// [row_row_index] length=length_of_the_row_row_index
// word1 counter1
// word2 counter2
// ...
void fprint_row(FILE* opened_file, MList* ml, unsigned row_index);


#endif