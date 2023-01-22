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
	// �������� �����
	char* word1 = (char*)retry_malloc(strlen("�����1") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word1, "������ ��������� ������ ��� ����� � ������\n"))
		strcpy(word1, "�����1");
	char* word2 = (char*)retry_malloc(strlen("�����2") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word2, "������ ��������� ������ ��� ����� � ������\n"))
		strcpy(word2, "�����2");
	char* word3 = (char*)retry_malloc(strlen("�����3") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word3, "������ ��������� ������ ��� ����� � ������\n"))
		strcpy(word3, "�����3");
	char* word4 = (char*)retry_malloc(strlen("�����1") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word4, "������ ��������� ������ ��� ����� � ������\n"))
		strcpy(word4, "�����1");
	char* word5 = (char*)retry_malloc(strlen("�����1") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word5, "������ ��������� ������ ��� ����� � ������\n"))
		strcpy(word5, "�����1");
	char* word6 = (char*)retry_malloc(strlen("�����2") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word6, "������ ��������� ������ ��� ����� � ������\n"))
		strcpy(word6, "�����2");
	char* word7 = (char*)retry_malloc(strlen("�����7") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word7, "������ ��������� ������ ��� ����� � ������\n"))
		strcpy(word7, "�����7");
	char* word8 = (char*)retry_malloc(strlen("�����0") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word8, "������ ��������� ������ ��� ����� � ������\n"))
		strcpy(word8, "�����0");
	char* word9 = (char*)retry_malloc(strlen("�����9") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word9, "������ ��������� ������ ��� ����� � ������\n"))
		strcpy(word9, "�����9");
	char* word10 = (char*)retry_malloc(strlen("�����10") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word10, "������ ��������� ������ ��� ����� � ������\n"))
		strcpy(word10, "�����10");
	char* word11 = (char*)retry_malloc(strlen("�����11") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word11, "������ ��������� ������ ��� ����� � ������\n"))
		strcpy(word11, "�����11");
	char* word12 = (char*)retry_malloc(strlen("�����12") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word12, "������ ��������� ������ ��� ����� � ������\n"))
		strcpy(word12, "�����12");
	char* word13 = (char*)retry_malloc(strlen("�����13") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word13, "������ ��������� ������ ��� ����� � ������\n"))
		strcpy(word13, "�����13");
	char* word14 = (char*)retry_malloc(strlen("�����14") + 1, MAX_MALLOC_ATTEMPTS);
	if (not_null_ptr(word14, "������ ��������� ������ ��� ����� � ������\n"))
		strcpy(word14, "�����14");
	// ������� ��������� ���� � �������
	unsigned c1 = 1, c2 = 1, c3 = 1, c4 = 3, c5 = 10, c6 = 2, c7 = 7, c8 = 0, c9 = 1, c10 = 10, c11 = 11, c12 = 12, c13 = 13, c14 = 14;
	assert(ht_set(&ht, word1, c1) == EXIT_SUCCESSFULLY);
	assert(ht_set(&ht, word2, c2) == EXIT_SUCCESSFULLY);
	assert(ht_set(&ht, word3, c3) == EXIT_SUCCESSFULLY);
	// ������� ������������� ����� � ��� ��������� �����
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
	// �������� ������� ��������� ����� � �������
	assert(ht_has(&ht, "�����1"));
	assert(ht_has(&ht, "�����3"));
	assert(!ht_has(&ht, "�����1"));
	assert(!ht_has(&ht, "�����90"));
	// �������� ������� ����������� ������ �� �����
	assert(ht_get(&ht, "�����1") == NULL);
	assert(*ht_get(&ht, "�����2") == c2 + c6);
	assert(*ht_get(&ht, "�����7") == c7);
	// ������ �����0 � �����2, � ����� �������������� �����0 � �����20
	assert(ht_delete(&ht, "�����0") == EXIT_SUCCESSFULLY);
	assert(ht_delete(&ht, "�����2") == EXIT_SUCCESSFULLY);
	assert(ht_delete(&ht, "�����20") == EXIT_ABSENT);
	assert(ht_delete(&ht, "�����0") == EXIT_ABSENT);
	assert(!ht_has(&ht, "�����2"));
	assert(!ht_has(&ht, "�����0"));
	// �������� �������
	assert(ht_resize(&ht, ht.size * 2) == EXIT_SUCCESSFULLY);
	// �������� ��� �������� �� 1
	ht_traverse(&ht, add_one);
	assert(*ht_get(&ht, "�����7") == c7 + 1);
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