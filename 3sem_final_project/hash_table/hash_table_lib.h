#ifndef _HASH_TABLE_LIB_
#define _HASH_TABLE_LIB_


#include "hash_table.h"
#include <stdio.h>
#include <stdbool.h>


// ���-������� ���������
unsigned jenkins_one_at_a_time_hash(const char* key);

// ���������������� �������.
// ������� ��� ������ ���� �������! �� ������ �������������� �
// size - ������ �������� �������;
// hf   - ���-�������;
// ���� hf=NULL, ������� ����������� ������� (���������).
// ���������� 0 � ������ �������� �������������. � ������ ������� � ���������� ������ ���������� -1
int ht_init(HashTable* ht, unsigned size, HashFunction hf);

// ���������� �������
// �������� ������ ������ ������� ���� HashTable ������ ����������� �������! �.�. ������������, ��������, ������� ������ ��� ������ ����������
void ht_destroy(HashTable* ht);

// �������� � ������� ������������ word -> counter. ���� word ��� �����������, ��������� counter � ������������� ��������
// ���� � ������� ���������� ������� ����� ����, �� ������� ������� ������������� � ��������� ���, ����� �������� ����� �������� (��� ����� �������� ������ � ����������)
// �����, �� ������� ��� ���������� ���� ������ ��������� ���������� ����� � �������, ����� �������� � ����������, ����� ����� #define WRDS_EXCEED
// �� ������� ��� ����������� ������� ������ ����� #define HT_INCRS
void ht_set(HashTable* ht, const char* word, unsigned counter);

// �������� ��������� �� ������� �����. ���� ����� ��� � �������, ������� NULL
unsigned* ht_get(const HashTable* ht, const char* word);

// �������� ������������� ����� word � �������
bool ht_has(const HashTable* ht, const char* word);

// ������� ������� � ������ word �� ������� (���� �� ����)
void ht_delete(HashTable* ht, const char* word);

//  ����� ������� � ���������� ���� ���������. ������� func ����� ������� ��� ���� ��� (word, counter) �� �������. ���� ������ ������!
void ht_traverse(HashTable* ht, void (*func)(const char* word, unsigned* counter));

// �������� ������ �������� �������
void ht_resize(HashTable* ht, unsigned new_size);

// �������� ������� � �������� ���� opened_file. ������ ������:
// <table_name> size=size_of_the_table total_words=total_amount_of_words_in_the_table
// [row_0] length=length_of_the_row_0
// word1 counter1
// word2 counter2
// ...
// [row_1] length=length_of_the_row_1
// ...
void fprint_ht(FILE* opened_file, HashTable* ht, const char* table_name);

// �������� ������� (������) ������� � �������� ���� opened_file. ������ ������:
// [row_row_index] length=length_of_the_row_row_index
// word1 counter1
// word2 counter2
// ...
void fprint_row(FILE* opened_file, MList* ml, unsigned row_index);


#endif