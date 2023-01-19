#ifndef _CORRECTOR_MODEL_
#define _CORRECTOR_MODEL_


#include "hash_table.h"


typedef void* Pointer;
typedef int (*WordsMetric)(const char*, const char*); // ������� ���������� �������� ���� (������� � ������������ ����)


/*
������������� ������.
������ ������������ ����� �����, ������ ���-������ (������ ������� - ��������� ����� ����������� �����), ������� ���������� �������� ���� (���-������� ���������)
������� ������ ����������� � ���������� hash_table_lib
*/
typedef struct Corrector
{
	unsigned max_word_length;
	HashTable* dictionary;
	WordsMetric metric;
} Corrector;

#endif

