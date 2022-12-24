#include "constants.h"
#include "files_working.h"
#include "helpful_functions.h"
#include <stdio.h>
#include <locale.h>
#include <stdbool.h>
#include <direct.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


bool file_create(const char* file_name, const char* error_msg)
{
	setlocale(LC_ALL, "ru");
	FILE* new_file = fopen(file_name, "w");
	bool successfully_created = (new_file == NULL) ? false : true;
	if (successfully_created == false && error_msg == NULL)
		printf("%s", error_msg);
	else
		fclose(new_file);
	return successfully_created;
}

bool file_remove(const char* file_name, const char* error_msg)
{
	setlocale(LC_ALL, "ru");
	int remove_result = remove(file_name);
	bool successfully_removed = (remove_result == 0) ? true : false;
	if (successfully_removed == false && error_msg != NULL)
		printf("%s", error_msg);
	return successfully_removed;
}

bool file_write(FILE* file, unsigned strings_amount, const char** strings_for_writing, const char* shell, const char* delimeter, const char* end)
{
	setlocale(LC_ALL, "ru");
	shell = (shell == NULL) ? "\0" : shell;
	delimeter = (delimeter == NULL) ? "\0" : delimeter;
	end = (end == NULL) ? "\0" : end;
	bool successfully_written = (file == NULL) ? false : true;
	if (successfully_written) // ���� ������
	{
		while (strings_amount > 1 && successfully_written)  // �������� ������ ���� �� ��������� ���� ��� �� ��������� ������ ������
		{
			successfully_written *= (fprintf(file, "%s%s%s%s", shell, strings_for_writing[0], shell, delimeter) >= 0) ? true : false;
			--strings_amount;
			++strings_for_writing;
		}
		if (strings_amount == 1 && successfully_written) // �� ����� �� ������ �������, ��������� ��������� ������
			successfully_written *= (fprintf(file, "%s%s%s", shell, strings_for_writing[0], shell) >= 0) ? true : false;
		if (successfully_written) // �� ����� �� ������ �������, ��������� end
			successfully_written = (fputs(end, file) == EOF) ? false : true;
	}
	return successfully_written;
}

char* rel_to_abs_way(const char* rel_name, bool print_way, bool print_errors)
{
	char* tmp_path = _getcwd(NULL, BUFFER_SIZE);  // �������� ������� ����������, � ������ ������ ����� NULL
	if (errno == ENOMEM && print_errors) // ������������ ������ ��� ��������� BUFFER_SIZE ������ ������������ ������
		printf("\n�� ������� �������� ����� ��� ��� ������� ����������\n");
	else if (errno == ERANGE && print_errors) // ���� ������� BUFFER_SIZE ��������
		printf("\n��� ������� ���������� ������� �������\n");
	else if (tmp_path != NULL && rel_name != NULL) // ���� �������� ������������� ���� � ������� ����������, �������� ����� ������
	{
		size_t directory_length = strlen(tmp_path);
		size_t rel_name_length = strlen(rel_name) + 2;  // ���� �������� � ������� ���������� ���� \, ������������� ���� � ����� � ����-���������� \0
		size_t abs_path_length = directory_length + rel_name_length;
		char* new_memory = retry_malloc(abs_path_length * sizeof(char), MAX_MALLOC_ATTEMPTS);
		if (new_memory == NULL) // �� ������� �������� ����� ������, ������ ������ � ���������� NULL
		{
			if (print_errors)
				printf("\n�� ������� �������� ������ ��� ���������� ����\n");
			free(tmp_path);
			tmp_path = NULL;
		}
		else // �� ����������, ���������� ���������� ���� � ����� ������, ������ ������ � ������������ ��������� �� ����� ������
		{
			strcpy(new_memory, tmp_path);
			strcat(new_memory, "\\");
			strcat(new_memory, rel_name);
			free(tmp_path);
			tmp_path = new_memory;
		}
	}
	if (tmp_path != NULL && print_way)
		printf("%s\n", tmp_path);
	return tmp_path;
}

int read_param_from_file(FILE* file_from, char* buffer, size_t buffer_size)
{
	int read_result = EXIT_SUCCESSFULLY;
	int symbol_code = fgetc(file_from);
	while (symbol_code == (int)' ')
		symbol_code = fgetc(file_from);
	int stop_code = ' ';
	if (symbol_code == (int)'"')
	{
		symbol_code = fgetc(file_from);
		stop_code = (int)'"';
	}
	int last_index = 0;
	for (; last_index < buffer_size - 1 && symbol_code != EOF && symbol_code != stop_code; ++last_index) // ���� ������ ��������� ��� '\0'
	{
		buffer[last_index] = (char)symbol_code;
		symbol_code = fgetc(file_from);
	}
	if (symbol_code == EOF && stop_code == '"') // ����� �� ����� �����, �� �� ����� ����������� �������; �� ��� �������, ������������ ����!
		read_result = EXIT_USER_FAILURE;
	else if (last_index == buffer_size - 1 && symbol_code != stop_code)  // ��������� ���� �����, � �������� ��� �� ����������; ������ ��������� ������!
		read_result = EXIT_MEMORY_FAILURE;
	else // ������� ��������� ��������!
		buffer[last_index] = '\0';
	if (strchr(buffer, '"') != NULL) // � ���������� ��� �������� �������, ������ ������������ ����������� ��� ��������
		read_result = EXIT_USER_FAILURE;
	return read_result;
}