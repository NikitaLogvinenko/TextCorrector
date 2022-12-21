#ifndef _CORRECTOR_MODEL_
#define _CORRECTOR_MODEL_


#include "linked_list.h"


typedef void (*Destructor)(Pointer);  // ����������
typedef int (*WordsMetric)(const char*, const char*); // ������� ���������� �������� ���� (������� � ������������ ����)


/*
������������� ������.
������ ������������ ����� �����, ������ ������� ������� (������ ������ - ��������� ����� ����������� �����), ������� ���������� �������� ����, ������� ������� �������
*/
typedef struct Corrector
{
	int max_word_size;
	Linked_list** dictionary;
	WordsMetric metric;
	Destructor dtor;
} Corrector;

#endif

