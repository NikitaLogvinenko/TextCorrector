#ifndef _MARKED_LIST_LIB_
#define _MARKED_LIST_LIB_


#include "marked_list.h"
#include <stdbool.h>


// ������� ������� ������ c ������ word � ���������, ������������ � counter (�� �� ����������� 1, ���� �� ��������� ��� ��������� ������ �� �����)
// �������� ������ ��������� �� ����, ������ �� ����������
MList* mlist_create(const char* word, unsigned counter);

// �������� ������� ������
void mlist_destroy(MList* ml);

// ������� ����� ��������� � ������
unsigned mlist_length(const MList* ml);

// �������� ������� �� ����� � ������, ������� ��������� �� ����� ������ (�.�. ������ ����� ���� ����� ������). 
// ������� ���������� � counter (�.�. ����� ��������� ������ �� �����)
// ���� ����� ���� ��� ��� � ������ - ��������� � �������� ������������� �������� counter
// ���� ������ ����� ��� �� ���� - �������� ����� ������ � counter
// � ������ ��������� ��������� �� ������������ ������ � ������ word; ��� counter ���������� ����������� ������
MList* mlist_add(MList* ml, const char* word, unsigned counter);

// ������� ��������� �� ������ �������� �� ��� �����; ���� ������ ����� ��� - ������� NULL
// ������������ ��������� �� ������, ������� ��� ��������� ���� ������ ����� ���������� ����� ������ � ������
unsigned* mlist_get(const MList* ml, const char* word);

// ���������, ���� �� ����� word � ������
bool mlist_has(const MList* ml, const char* word);

// ������� ��������� �� ������� �� ������ word; ���� ������ ��� - ������� NULL
MList* mlist_find(const MList* ml, const char* word);

// ������� ������� �� ����� word; ������� ��������� �� ����� ������ ������
MList* mlist_remove(MList* ml, const char* word);

// � ��������� ������ ��������� ������� func
void mlist_foreach_counter(MList* ml, void (*func)(unsigned* number));

// � ������ � ������ ������ ��������� ������� func
void mlist_foreach_item(MList* ml, void (*func)(const char* word, unsigned* number));

// ������� ������ � ������� `�����: �������`; �������� ������ ������ � ����� ������� �� �������
void print_mlist(MList* ml);


#endif