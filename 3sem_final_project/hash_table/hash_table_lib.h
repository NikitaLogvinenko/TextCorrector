//#ifndef _HASH_TABLE_LIB_
//#define _HASH_TABLE_LIB_
//
//
//#include "hash_table.h"
//#include <stdbool.h>
//
//
///* Инициализировать таблицу. 
//Таблица уже создана! Мы только инициализируем её
// *
// * size - размер базового массива;
// * hf   - хеш-функция;
// * dtor - деструктор. Этой функции будут передаваться data удаляемых элементов
// *        (ht_destroy, ht_delete, а также при перезаписи элементов в ht_set).
// *
// * Если hf=0, берется стандартная функция (Дженкинс).
// * Если dtor=0, деструктор отсутствует (не будет вызываться).
// */
//void ht_init(HashTable* ht, size_t size, HashFunction hf, Destructor dtor);
//
///* 
//Уничтожить таблицу 
//Очищение памяти самого объекта типа HashTable должно выполняться вручную, т.к. пользователь, возможно, выделял память под объект статически
//Память data очищается с помощью переданной при создании функции dtor
//*/
//void ht_destroy(HashTable* ht);
//
///* Записать в таблицу соответствие key -> data. Если key уже существовал,
// * соотв. поле data будет удалено (через dtor) и перезаписано */
//void ht_set(HashTable* ht, const char* key, const Pointer data);
//
///* Получить значение по ключу. Если ключа нет в таблице, вернуть NULL. */
//Pointer ht_get(const HashTable* ht, const char* key);
//
///* Проверка существования ключа key в таблице */
//bool ht_has(const HashTable* ht, const char* key);
//
///* Удалить элемент с ключом key из таблицы (если он есть) */
//void ht_delete(HashTable* ht, const char* key);
//
///* Обход таблицы с посещением всех элементов. Функция f будет вызвана для
// * всех пар (key, data) из таблицы. Ключ менять нельзя!*/
//void ht_traverse(HashTable* ht, void (*f)(const char* key, Pointer data));
//
///* Изменить размер базового массива. */
//void ht_resize(HashTable* ht, size_t new_size);
//
//
//#endif