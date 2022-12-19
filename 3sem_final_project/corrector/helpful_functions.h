#ifndef _HELPFUL_FUNCTIONS_
#define _HELPFUL_FUNCTIONS_

#include <stdio.h>
#include <stdbool.h>

typedef void* Pointer;

// ���������, ��� ��������� �� �������, ����� ������� ��������� error_msg
bool not_null_ptr(const Pointer ptr, const char* error_msg);

// ���������, ��� ������ ���������
bool is_equal(const Pointer data1, size_t data1_size, const Pointer data2, size_t data2_size);

#endif