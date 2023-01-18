#ifndef _HASH_TABLE_
#define _HASH_TABLE_


#include "marked_list.h"
#include <stdio.h>


typedef void* Pointer;
typedef unsigned (*HashFunction)(const char* key);


typedef struct HashTable {
    unsigned size;  // ������ ������� (����� ����� = ����������� ���������� ���������� ������� �������)
    unsigned total_words;  // ������� ���� �������� � ������� (���� ������� ����� - ��������� ������ ������� ��� ��������� ���������� ����� (����� �� ��������� ������� ������� ��������))
    MList** table;  // ������ ��������
    HashFunction hashfunc;  // ������� ��� �������� ���-�����, � ������� ������������ ���-������� ���������. ����� �������� � ����� hash_table_lib.c � ������� ht_init
} HashTable;


#endif