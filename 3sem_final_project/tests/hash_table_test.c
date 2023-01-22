#include "constants.h"
#include "hash_table_lib.h"
#include "hash_table_test.h"
#include "helpful_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


static void add_one(const char* word, unsigned* counter);


void hash_table_test()
{
	printf("*******************************************************************************************\n");
	printf("HashTable tests were launched\n");
	HashTable ht;
	ht_init(&ht, 1, NULL);
	// Создадим слова
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
	char* word9 = (char*)retry_malloc(strlen("Слово9") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word9, "Ошибка выделения памяти под слово в тестах\n"))
		strcpy(word9, "Слово9");
	char* word10 = (char*)retry_malloc(strlen("Слово10") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word10, "Ошибка выделения памяти под слово в тестах\n"))
		strcpy(word10, "Слово10");
	char* word11 = (char*)retry_malloc(strlen("Слово11") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word11, "Ошибка выделения памяти под слово в тестах\n"))
		strcpy(word11, "Слово11");
	char* word12 = (char*)retry_malloc(strlen("Слово12") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word12, "Ошибка выделения памяти под слово в тестах\n"))
		strcpy(word12, "Слово12");
	char* word13 = (char*)retry_malloc(strlen("Слово13") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word13, "Ошибка выделения памяти под слово в тестах\n"))
		strcpy(word13, "Слово13");
	char* word14 = (char*)retry_malloc(strlen("Слово14") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word14, "Ошибка выделения памяти под слово в тестах\n"))
		strcpy(word14, "Слово14");
	// Добавим несколько слов в таблицу
	unsigned c1 = 1, c2 = 1, c3 = 1, c4 = 3, c5 = 10, c6 = 2, c7 = 7, c8 = 0, c9 = 1, c10 = 10, c11 = 11, c12 = 12, c13 = 13, c14 = 14;
	assert(ht_set(&ht, word1, c1) == EXIT_SUCCESSFULLY);
	assert(ht_set(&ht, word2, c2) == EXIT_SUCCESSFULLY);
	assert(ht_set(&ht, word3, c3) == EXIT_SUCCESSFULLY);
	// Добавим повторяющиеся слова и ещё несколько новых
	assert(ht_set(&ht, word4, c4) == EXIT_SUCCESSFULLY);
	assert(ht_set(&ht, word5, c5) == EXIT_SUCCESSFULLY);
	assert(ht_set(&ht, word6, c6) == EXIT_SUCCESSFULLY);
	assert(ht_set(&ht, word7, c7) == EXIT_SUCCESSFULLY);
	assert(ht_set(&ht, word8, c8) == EXIT_SUCCESSFULLY);
	assert(ht_set(&ht, word9, c9) == EXIT_SUCCESSFULLY);
	assert(ht_set(&ht, word10, c10) == EXIT_SUCCESSFULLY);
	assert(ht_set(&ht, word11, c11) == EXIT_SUCCESSFULLY);
	assert(ht_set(&ht, word12, c12) == EXIT_SUCCESSFULLY);
	assert(ht_set(&ht, word13, c13) == EXIT_SUCCESSFULLY);
	assert(ht_set(&ht, word14, c14) == EXIT_SUCCESSFULLY);
	fprint_ht(stdout, &ht, "----test_table_initial----");
	// Проверим функцию вхождения слова в таблицу
	assert(ht_has(&ht, "Слово1"));
	assert(ht_has(&ht, "Слово3"));
	assert(!ht_has(&ht, "слово1"));
	assert(!ht_has(&ht, "Слово90"));
	// проверим функцию возвращения данных по ключу
	assert(ht_get(&ht, "слово1") == NULL);
	assert(*ht_get(&ht, "Слово2") == c2 + c6);
	assert(*ht_get(&ht, "Слово7") == c7);
	// удалим Слово0 и Слово2, а также несуществующих слово0 и Слово20
	assert(ht_delete(&ht, "Слово0") == EXIT_SUCCESSFULLY);
	assert(ht_delete(&ht, "Слово2") == EXIT_SUCCESSFULLY);
	assert(ht_delete(&ht, "Слово20") == EXIT_ABSENT);
	assert(ht_delete(&ht, "слово0") == EXIT_ABSENT);
	assert(!ht_has(&ht, "Слово2"));
	assert(!ht_has(&ht, "Слово0"));
	// увеличим таблицу
	assert(ht_resize(&ht, ht.size * 2) == EXIT_SUCCESSFULLY);
	// увеличим все значения на 1
	ht_traverse(&ht, add_one);
	assert(*ht_get(&ht, "Слово7") == c7 + 1);
	fprint_ht(stdout, &ht, "----test_table_ultimate----");

	ht_destroy(&ht);
	printf("All HashTable tests were passed successfully!\n");
	printf("*******************************************************************************************\n");
}


static void add_one(const char* word, unsigned* counter)
{
	++*counter;
	printf("Data with the key \"%s\" was increased by 1\n", word);
}