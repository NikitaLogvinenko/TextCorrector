#ifndef _LL_LIB_
#define _LL_LIB_

#include "linked_list.h"
#include <stdio.h>
#include <stdbool.h>


// ������� ������� ������ � ������� data, ������� �������� data_size ������
// ������� ��������� �� ������ ������
Linked_list* llist_create(const Pointer data, size_t data_size);

// ������� ��������� ������
void llist_destroy(Linked_list* ll);

// C���������� ������, ������� ��������� �� ������ ������-�����
Linked_list* llist_copy(const Linked_list* ll);

// ������� ����� ��������� � ������
int llist_length(const Linked_list* ll);

// �������� ������� � ������, ������� ��������� �� ����� ������
Linked_list* llist_prepend(Linked_list* ll, const Pointer data, size_t data_size);

// �������� ������� � �����, ������� ��������� �� ����� ������ (���� ������� ��������� NULL, �� �������� ����� ������ � ����� �������� �������)
Linked_list* llist_append(Linked_list* ll, const Pointer data, size_t data_size);

// ������� ������ �������� �� ��� �������
// ���������� ��������� �� ����� ������ � ������
Pointer llist_get(const Linked_list* ll, unsigned index);

// ������� ������ ���������� ��������
// ���������� ��������� �� ����� ������ � ������
Pointer llist_get_last(const Linked_list* ll);

// ������� ������ ������� �������� � ������� data; ���� ������ ��� - ������� -1
int llist_find(const Linked_list* ll, const Pointer data, size_t data_size);

// ������� ������ ������� ��������, ��� ������ �������� ������� predicate ���������� ������; 
// �������� �� ��������� custom_data ����� ������
// ���� ������ ��� - ������� -1 � ������ �� ������ � custom_data
int llist_find_custom(const Linked_list* ll, bool (*predicate)(const Pointer data), Pointer const custom_data);

// ������� ������� �� ��� �������
// ������� ��������� �� ����� ������ ������
// �.�. ���������� ������ ����� � ������, � �� � ������, �� ����� ��������� ���� ����� ������� ������ �����
Linked_list* llist_remove(Linked_list* ll, unsigned index);

// ������� ������ ������� � ������� data
// ������� ��������� �� ����� ������ ������
// �.�. ���������� ������ ����� � ������, � �� � ������, �� ����� ��������� ���� ����� ������� ������ �����
Linked_list* llist_remove_first_equal(Linked_list* ll, const Pointer data, size_t data_size);

// ������� ��� �������� � ������� data
// ������� ��������� �� ����� ������ ������
// �.�. ���������� ������ ����� � ������, � �� � ������, �� ����� ��������� ���� ����� ������� ������ �����
Linked_list* llist_remove_all_equal(Linked_list* ll, const Pointer data, size_t data_size);

// ������������ ll2 � ����� ll1
// ������� ������ ������������ ������
// inplace ��������, ����� ������� �� ��������
Linked_list* llist_concat(Linked_list* ll1, Linked_list* ll2);

// � ������ ������ ��������� ������� foreach_func
void llist_foreach(Linked_list* ll,
    void (*func)(Pointer data));

#endif