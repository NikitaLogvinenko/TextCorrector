#ifndef _HELPFUL_FUNCTIONS_
#define _HELPFUL_FUNCTIONS_


#include <stdio.h>
#include <stdbool.h>


typedef void* Pointer;


// ���������, ��� ��������� �� �������. ���� ������� - ������� ��������� �� ������ error_msg (�� �������� ����, �� ���������� ������ � ��)
bool not_null_ptr(const Pointer ptr, const char* error_msg);

// ���������, ��� ������ �� ���������� ��������� ��������� (� ������ �������� ������)
bool datas_are_equal(const Pointer data1, size_t data1_size, const Pointer data2, size_t data2_size);

// ��������, ��� ��� ����� �����
bool ints_are_equal(int* number_1, int* number_2);

// �������� data �� multiplier
void multiply_int(int* data, const int* multiplier);

// ������� �������
void print_help();

// �������� ��������� exit_msg � �����
// ���� ������ �� ���� ������������ � ��� ��������������� ��� ����������, �� exit_code=EXIT_SOFT_FAILURE. ����� ��� ��������� �������.
void exit_with_msg(const char* exit_msg, int exit_code);

// ������� ������������ ������ �� ptrs_amount ���������� void*. ���������������� ��������� NULL. � ������ ������ ��������� ������ ������������ NULL
Pointer* empty_pointers_array(unsigned ptrs_amount);

// ��������� ������ �������� memory_size, ���� max_attempts �������, ���� ���� ������ �� ��������� (��� ��������� ������). � ������ ������� ����� NULL.
Pointer retry_malloc(size_t memory_size, unsigned max_attempts);


#endif