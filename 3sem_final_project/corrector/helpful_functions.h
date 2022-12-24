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

// ���������, ��� � ������ possible_int ������ ����� � ������ ������ (�������� ���, ���� �� ���������� '\0')
// ���� ������� NULL ��� ��� ����-�����������, ��� �������� ������ ����-���������� (������ ������), �� ������������ false
bool is_integer(char* possible_int);

// ������� �������� � ������� � ������� fgets � �������� � ����� �������� buffer_size.
// ���� ����� ���������� ������ ����� (buffer_size-1) (�� �������� ��� ����-����������), �� ��������� ������ ����� \0 => ��������� ������������� ������.
// ���� ������������� ������ \n, ������ ������ ������� ��� ������, ���� �� ������������� ������ �� \n, �� ������� �� �� (��������, ��� ��� \n � �� �������).
// ���� �� ������� ������, �� ������ ������� ������ � ���������� EXIT_MEMORY_FAILURE.
// ���� ������ �������, ��, ��-������, ������� \n � ����� (�������� �� \0). ��-������, ���������, ���� ������ ���������� � ������������� ���������, �� ������� ��.
// ���� ������� ��� ��� ��� ������ - ���������� EXIT_SUCCESSFULLY. ���� ������� ������ ���� - ���������� EXIT_USER_FAILURE
// ���� ����� ���� �������� � buffer ���������� ������� - ������� EXIT_USER_FAILURE
int read_param_from_console(char* buffer, size_t buffer_size);

// �������� ����� (������� ��� ������� �� stdin, ���� �� �������� \n)
void clear_buff();

// ������� ������������ ������ ���������. ��������������, ��� ��� ������
void print_train_new_cfg(Pointer* cfg);
// ������� ������������ ������ ���������. ��������������, ��� ��� ������
void print_train_existed_cfg(Pointer* cfg);
// ������� ������������ ������ ���������. ��������������, ��� ��� ������
void print_edit_cfg(Pointer* cfg);

#endif