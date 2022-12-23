#ifndef _FILES_WORKING_
#define _FILES_WORKING_


#include <stdio.h>
#include <stdbool.h>


// ������� ����
// file_name ������ ���� ������������� ��� ���������� ���� � �������� � ���� ��� ������������ �����
// � ������ ������� ������������ false � ��������� error_msg
bool file_create(const char* file_name, const char* error_msg);

// ������� ����
// file_name ������ ���� ������������� ��� ���������� ���� � �������� � ���� ��� ���������� �����
// � ������ ������� ������������ false � ��������� error_msg
bool file_remove(const char* file_name, const char* error_msg);

// �������� � ���� file strings_amount ������� strings_for_writing. ��������������, ��� ���� ������ � ������ ������ (������ ��� ��������). ���� �� ����������� �� ��������� ������!!!
// ���� file == NULL, �� ������ ������������ �� �����, �� �������� false
// � ������ � ����� ������ ������ ����������� shell (���� NULL, �� ������ �� ����������� ("\0"))
// ������-����������� ����� �������� - delimiter (���� NULL, �� ��� ������ �������� ������ ("\0"))
// � ����� ���� ������� ������ end (���� NULL, �� � ����� �� ����������� ������ ("\0")).
// ����� ������ ������� �� �������� DELIMETER! ���� ����� �������� ���������� ������ �� �����, ���� �������� ����������� � ������ ������.
// ����� END �� �������� DELIMETER! ���� ����� ��������� ��������� ���� � end ������������, ���� �������� ����������� � end
// � ������ ������� ������ false
bool file_write(FILE* file, unsigned strings_amount, const char** strings_for_writing, const char* shell, const char* delimeter, const char* end);

// �������� ������������� ���� � ����� (������ � ������) rel_name. ���������� ���������� ����. ������ ���������� �����������!
// ���� rel_name == NULL, �� ���������� ���� � ������� ����������. � ������ ������ ���������� NULL
// ���� �� ������ ��� ������ � print_way == true, �� ��� � ���������� ���� ���� � �������
// ���� print_errors == true, �� �������� �� ������� � �������, ����� ������ �� �������� ��� ������� � ���������� NULL
char* rel_to_abs_way(const char* rel_name, bool print_way, bool print_errors);


#endif