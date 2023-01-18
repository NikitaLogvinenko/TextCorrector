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

	// ��������� ������� ������������ ������
	MList* ml = mlist_create(NULL, 10);
	assert(ml == NULL);
	// �������� ���������� ������
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

	ml = mlist_create(word1, 1);
	assert(ml != NULL);
	// ������� ����� �����
	ml = mlist_add(ml, word2, 1);
	ml = mlist_add(ml, word3, 1);
	// ���������� ������
	print_mlist(ml);
	// �������� ������� �������� �����
	assert(mlist_length(ml) == 3);
	// ��������� �������� �������� � ������������ ������, ��������, ��� ����� ������ �� ��������
	ml = mlist_add(ml, word4, 1);
	ml = mlist_add(ml, word5, 1);
	ml = mlist_add(ml, word6, 1);
	assert(mlist_length(ml) == 3);
	// ���������� ������
	print_mlist(ml);
	// �������� ������� ������ �� ����� (��������� ������������ � �������������� �����)
	assert(mlist_has(ml, "�����1"));
	assert(!mlist_has(ml, "�����20"));
	assert(mlist_get(ml, "�����100") == NULL);
	assert(*(mlist_get(ml, "�����1")) == 3);
	assert(mlist_find(ml, "������ ���") == NULL);
	assert(mlist_find(ml, "�����2")->counter == 2);
	// ������� ���������� �����
	ml = mlist_add(ml, word7, 7);
	ml = mlist_add(ml, word8, 1);
	assert(*mlist_get(ml, "�����7") == 7);
	print_mlist(ml);
	// ������ �����0 (������), �����1 (�����) � �����3 (� ��������)
	ml = mlist_remove(ml, "�����0");
	print_mlist(ml);
	assert(ml->counter == 7);  // � ������ ������ �����7
	ml = mlist_remove(ml, "�����1");
	print_mlist(ml);
	assert(!mlist_has(ml, "�����1"));  // ������� �����1
	ml = mlist_remove(ml, "�����3");
	print_mlist(ml);
	assert(mlist_length(ml) == 2);  // �������� ������ �����2 � �����7
	// �������� ������� foreach, �������� ��� �������� �� 1
	mlist_foreach_item(ml, add_one);
	assert(*mlist_get(ml, "�����2") == 3);
	print_mlist(ml);
	// �������� ������
	mlist_destroy(ml);

	printf("All MarkedList tests were passed successfully!\n");
	printf("*******************************************************************************************\n");
}

static void add_one(const char* word, unsigned* number)
{
	++*number;
	printf("Data with the key \"%s\" was increased by 1\n", word);
}