#include "constants.h"
#include "marked_list.h"
#include "marked_list_lib.h"
#include "marked_list_test.h"
#include "helpful_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


static void add_one(const char* word, unsigned* number);

void mlist_tests()
{
	printf("*******************************************************************************************\n");
	printf("MarkedList tests were launched\n");

	// попробуем создать некорректный список
	MList* ml = mlist_create(NULL, 10);
	assert(ml == NULL);
	// создадим нормальный список
	char* word1 = (char*)retry_malloc(strlen("Слово1") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word1, "Ошибка выделения памяти под слово в тестах\n"))
		strcpy(word1, "Слово1");
	char* word2 = (char*)retry_malloc(strlen("Слово2") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word2, "Ошибка выделения памяти под слово в тестах\n"))
		strcpy(word2, "Слово2");
	char* word3 = (char*)retry_malloc(strlen("Слово3") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word3, "Ошибка выделения памяти под слово в тестах\n"))
		strcpy(word3, "Слово3");
	char* word4 = (char*)retry_malloc(strlen("Слово1") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word4, "Ошибка выделения памяти под слово в тестах\n"))
		strcpy(word4, "Слово1");
	char* word5 = (char*)retry_malloc(strlen("Слово1") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word5, "Ошибка выделения памяти под слово в тестах\n"))
		strcpy(word5, "Слово1");
	char* word6 = (char*)retry_malloc(strlen("Слово2") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word6, "Ошибка выделения памяти под слово в тестах\n"))
		strcpy(word6, "Слово2");
	char* word7 = (char*)retry_malloc(strlen("Слово7") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word7, "Ошибка выделения памяти под слово в тестах\n"))
		strcpy(word7, "Слово7");
	char* word8 = (char*)retry_malloc(strlen("Слово0") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word8, "Ошибка выделения памяти под слово в тестах\n"))
		strcpy(word8, "Слово0");

	ml = mlist_create(word1, 1);
	assert(ml != NULL);
	// добавим новые слова
	ml = mlist_add(ml, word2, 1);
	ml = mlist_add(ml, word3, 1);
	// напечатаем список
	print_mlist(ml);
	// проверим функцию подсчёта длины
	assert(mlist_length(ml) == 3);
	// попробуем вставить значения с существующим ключём, проверим, что длина списка не меняется
	ml = mlist_add(ml, word4, 1);
	ml = mlist_add(ml, word5, 1);
	ml = mlist_add(ml, word6, 1);
	assert(mlist_length(ml) == 3);
	// напечатаем список
	print_mlist(ml);
	// проверим функции поиска по ключу (проверяем существующие и несуществующие ключи)
	assert(mlist_has(ml, "Слово1"));
	assert(!mlist_has(ml, "Слово20"));
	assert(mlist_get(ml, "Слово100") == NULL);
	assert(*(mlist_get(ml, "Слово1")) == 3);
	assert(mlist_find(ml, "Такого нет") == NULL);
	assert(mlist_find(ml, "Слово2")->counter == 2);
	// добавим оставшиеся числа
	ml = mlist_add(ml, word7, 7);
	ml = mlist_add(ml, word8, 1);
	assert(*mlist_get(ml, "Слово7") == 7);
	print_mlist(ml);
	// удалим Слово0 (голову), слово1 (хвост) и слово3 (в середине)
	ml = mlist_remove(ml, "Слово0");
	print_mlist(ml);
	assert(ml->counter == 7);  // в голове теперь Слово7
	ml = mlist_remove(ml, "Слово1");
	print_mlist(ml);
	assert(!mlist_has(ml, "Слово1"));  // удалили Слово1
	ml = mlist_remove(ml, "Слово3");
	print_mlist(ml);
	assert(mlist_length(ml) == 2);  // остались только Слово2 и Слово7
	// проверим функцию foreach, увеличив все значения на 1
	mlist_foreach_item(ml, add_one);
	assert(*mlist_get(ml, "Слово2") == 3);
	print_mlist(ml);
	// удаление списка
	mlist_destroy(ml);

	printf("All MarkedList tests were passed successfully!\n");
	printf("*******************************************************************************************\n");
}

static void add_one(const char* word, unsigned* number)
{
	++*number;
	printf("Data with the key \"%s\" was increased by 1\n", word);
}