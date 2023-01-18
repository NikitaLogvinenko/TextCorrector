//#include "hash_table.h"
//#include "hash_table_lib.h"
//#include "hash_table_test.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <assert.h>
//#include <locale.h>
//
//
//static void print_int_ht(HashTable* ht);
//
//static void print_int_mlist(MList* ml);
//
//static void print_removed_data(int* data);
//
//static void add_one(const char* key, int* data);
//
//
//void hash_table_tests()
//{
//	printf("*******************************************************************************************\n");
//	printf("HashTable tests were launched\n");
//	HashTable ht;
//	ht_init(&ht, 5, NULL, print_removed_data);
//	// ������� ������ � �������
//	int Roma182 = 182;
//	ht_set(&ht, "����", &Roma182);
//	int Victor160 = 160;
//	ht_set(&ht, "������", &Victor160);
//	int Alex170 = 170;
//	ht_set(&ht, "���������", &Alex170);
//	int Nastya172 = 172;
//	ht_set(&ht, "�����", &Nastya172);
//	int Masha164 = 164;
//	ht_set(&ht, "����", &Masha164);
//	int Slava189 = 189;
//	ht_set(&ht, "�����", &Slava189);
//	int Saveliy179 = 179;
//	ht_set(&ht, "�������", &Saveliy179);
//	int Andrey202= 202;
//	ht_set(&ht, "������", &Andrey202);
//	int Vova140= 140;
//	ht_set(&ht, "����", &Vova140);
//	int Olya173 = 173;
//	ht_set(&ht, "���", &Olya173);
//	print_int_ht(&ht);
//	// ������� ������ �� ������ ������
//	int Slava181 = 181;
//	ht_set(&ht, "�����", &Slava181);
//	int Saveliy167 = 167;
//	ht_set(&ht, "�������", &Saveliy167);
//	int Andrey200 = 200;
//	ht_set(&ht, "������", &Andrey200);
//	print_int_ht(&ht);
//	// �������� ������� �������� ������� �����
//	assert(ht_has(&ht, "�����"));
//	assert(ht_has(&ht, "�������"));
//	assert(!ht_has(&ht, "�������"));
//	assert(!ht_has(&ht, "�������"));
//	// �������� ������� ����������� ������ �� �����
//	assert(*((int*)ht_get(&ht, "����")) == 182);
//	assert(*((int*)ht_get(&ht, "������")) == 160);
//	assert(*((int*)ht_get(&ht, "�����")) >= 181);
//	// ������� ��� �������
//	int Victor1 = 160;
//	ht_set(&ht, "������1", &Victor1);
//	int Victor2 = 160;
//	ht_set(&ht, "������2", &Victor2);
//	int Victor3 = 160;
//	ht_set(&ht, "������3", &Victor3);
//	int Victor4 = 160;
//	ht_set(&ht, "������4", &Victor4);
//	int Victor5 = 160;
//	ht_set(&ht, "������5", &Victor5);
//	int Victor6 = 160;
//	ht_set(&ht, "������6", &Victor6);
//	int Victor7 = 160;
//	ht_set(&ht, "������7", &Victor7);
//	int Victor8 = 160;
//	ht_set(&ht, "������8", &Victor8);
//	// ������ �������2 � �������6, � ����� �������������� �������10 � �������
//	assert(ht_has(&ht, "������2"));
//	assert(ht_has(&ht, "������6"));
//	ht_delete(&ht, "������2");
//	ht_delete(&ht, "������6");
//	assert(!ht_has(&ht, "������2"));
//	assert(!ht_has(&ht, "������6"));
//	ht_delete(&ht, "������10");
//	ht_delete(&ht, "�������");
//	// �������� �������
//	printf("������ ��������� �������:\n");
//	print_int_ht(&ht);
//	ht_resize(&ht, 20);
//	printf("����� ������� ��������:\n");
//	print_int_ht(&ht);
//	// �������� ��� �������� �� 1
//	ht_traverse(&ht, add_one);
//	assert(*((int*)ht_get(&ht, "����")) == 183);
//	assert(*((int*)ht_get(&ht, "������")) == 161);
//	print_int_ht(&ht);
//
//	printf("------------����� �����. �������� ���-�������------------\n");
//	ht_destroy(&ht);
//	printf("All HashTable tests were passed successfully!\n");
//	printf("*******************************************************************************************\n");
//}
//
//static void print_int_ht(HashTable* ht)
//{
//	setlocale(LC_ALL, "ru");
//	if (ht == NULL) return;
//	for (int i = 0; i < ht->size; ++i)
//	{
//		printf("[%d] ", i);
//		print_int_mlist(ht->table[i]);
//	}
//}
//
//static void print_int_mlist(MList* ml)
//{
//	setlocale(LC_ALL, "ru");
//	while (ml != NULL)
//	{
//		printf("(%s: %d)\t", ml->key, *((int*)ml->data));
//		ml = ml->next;
//	}
//	printf("\n");
//}
//
//static void print_removed_data(int* data)
//{
//	printf("������� �������� �� �������: %d\n", *data);
//}
//
//static void add_one(const char* key, int* data)
//{
//	++* data;
//	printf("Data with the key \"%s\" was increased by 1\n", key);
//}