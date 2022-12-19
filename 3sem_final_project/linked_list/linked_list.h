#ifndef _LINKED_LIST_
#define _LINKED_LIST_


#include <stdio.h>


typedef void* Pointer;

// ������� ������. ������ ������ ������, ��������� �� ������, ��������� �� ��������� ������� ������
typedef struct Linked_list
{
	size_t data_size; // ����� �������� ������ �� ��������� data
	void* data; // �������� ������, ������� ������� ������ �������� � ������, ��� �� �� � ������
	struct Linked_list* next; // ��������� �� ��������� ������� ������
} Linked_list;

#endif