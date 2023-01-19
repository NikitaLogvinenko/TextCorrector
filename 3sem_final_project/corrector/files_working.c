#include "constants.h"
#include "files_working.h"
#include "helpful_functions.h"
#include <stdio.h>
#include <stdbool.h>
#include <direct.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


FILE* create_and_open_tmp(const char* tmp_file_name, const char* open_mode)
{
	bool tmp_created = file_create(tmp_file_name, NULL);
	FILE* tmp_file = NULL;
	if (tmp_created != false) // ��������� ���� ������
	{
		tmp_file = fopen(tmp_file_name, open_mode);
		if (tmp_file == NULL) // ��������� ���� ��������, �� ������-�� �� ��������; ������� ��� � ����� NULL
			delete_if_tmp(tmp_file_name);
	}
	return tmp_file;
}

void delete_if_tmp(const char* path_to_file)
{
	if (path_to_file != NULL && strstr(path_to_file, TMP_SUFFIX) != NULL) // ��� ��� ��������� ���� � ����������� ��� �������; ���� ������� ����
	{
		bool successfully_removed = file_remove(path_to_file, "������ �������� ���������� �����. ������� ��� �������������� ��� �������������:\n");
		if (successfully_removed == false)
			// �� ���������� ������� ��������� ����; ������� ������������ �� ���� � ��������� ������� ��������������, ����������� ���������� ����
			free(rel_to_abs_way(path_to_file, true, true)); 
	}
}

bool file_exists(const char* file_name)
{
	bool file_exists = false;
	FILE* existed_file = fopen(file_name, "r"); // ������� ������� �� ������, ����� ���������, ���������� �� ����
	if (existed_file != NULL)
	{
		file_exists = true;
		fclose(existed_file);
	}
	return file_exists;
}

bool file_create(const char* file_name, const char* error_msg)
{
	bool successfully_created = false;
	bool file_already_exists = file_exists(file_name);
	if (file_already_exists)  // ����� ���� ��� ����������, ����� ����� ���� �� �������� � ������������ false
		printf("������� ���������� ������������� �����. ������ �������� �������� ������ �����\n");
	else  // ���� ����� � ����� ������ �� ���������� - ����� �������
	{
		FILE* new_file = fopen(file_name, "w");
		successfully_created = (new_file == NULL) ? false : true;
		if (successfully_created == false && error_msg != NULL)
			printf("%s", error_msg);
		else if (successfully_created == true)
			fclose(new_file);
	}
	return successfully_created;
}

bool file_remove(const char* file_name, const char* error_msg)
{
	int remove_result = remove(file_name);
	bool successfully_removed = (remove_result == 0) ? true : false;
	if (successfully_removed == false && error_msg != NULL)
		printf("%s", error_msg);
	return successfully_removed;
}

bool file_write(FILE* file, unsigned strings_amount, const char** strings_for_writing, const char* shell, const char* delimeter, const char* end)
{
	shell = (shell == NULL) ? "\0" : shell;
	delimeter = (delimeter == NULL) ? "\0" : delimeter;
	end = (end == NULL) ? "\0" : end;
	bool successfully_written = (file == NULL) ? false : true;
	if (successfully_written) // ���� ������
	{
		for (; strings_amount > 1 && successfully_written; --strings_amount) // �������� ������ ���� �� ��������� ���� ��� �� ��������� ������ ������
		{
			successfully_written *= (fprintf(file, "%s%s%s%s", shell, strings_for_writing[0], shell, delimeter) >= 0) ? true : false; // ����� � ����, ���������, ��� ������� ��������
			++strings_for_writing;
		}
		if (strings_amount == 1 && successfully_written) // �� ����� �� ������ �������, ��������� ��������� ������
			successfully_written *= (fprintf(file, "%s%s%s", shell, strings_for_writing[0], shell) >= 0) ? true : false;
		if (successfully_written) // �� ����� �� ������ �������, ��������� end
			successfully_written *= (fputs(end, file) == EOF) ? false : true;
	}
	return successfully_written;
}

char* rel_to_abs_way(const char* rel_name, bool print_way, bool print_errors)
{
	char* working_directory = _getcwd(NULL, BUFFER_SIZE);  // �������� ������� ����������, � ������ ������ ����� NULL; ������ ���������� �����������
	char* abs_path = NULL;
	if (errno == ENOMEM && print_errors)  // ������������ ������ ��� ��������� BUFFER_SIZE ������ ������������ ������
		printf("\n�� ������� �������� ������ ��� ��� ������� ����������. ���������� ��������� ��������� �� �������� �����\n");
	else if (errno == ERANGE && print_errors)  // ��� ���� ������� BUFFER_SIZE ��������
		printf("\n��� ������� ���������� ������� �������. ���������� ��������� ��������� �� �������� �����\n");
	else if (rel_name == NULL)
		abs_path = working_directory;
	else  // ���� �������� ������������� ���� � ������� ����������, �������� ����� ������
	{
		size_t directory_length = strlen(working_directory);
		size_t rel_name_length = strlen(rel_name) + 2;  // ���� �������� � ������� ���������� ���� \, ������������� ���� � ����� � ����-���������� \0
		size_t abs_path_length = directory_length + rel_name_length;
		abs_path = (char*)retry_malloc(abs_path_length * sizeof(char), MAX_MALLOC_ATTEMPTS);
		if (abs_path != NULL)  // �� ����������, ���������� ���������� ���� � ����� ������
		{
			strcpy(abs_path, working_directory);
			strcat(abs_path, "\\");
			strcat(abs_path, rel_name);
		}
		else if (print_errors)  // �� ������� �������� ������ ��� ���������� ����, �������� ������������
			printf("\n�� ������� �������� ������ ��� ���������� ����\n");

		free(working_directory);  // ������ ��� ��� ������� ���������� � _getcwd ���������� �����������
	}
	if (abs_path != NULL && print_way)
		printf("%s\n", abs_path);
	return abs_path;
}

int read_param_from_file(FILE* file_from, char* buffer, size_t buffer_size)
{
	int read_result = EXIT_SUCCESSFULLY;
	int symbol_code = fgetc(file_from);
	bool in_quotes = false;
	while (isspace((unsigned char)symbol_code) != 0)  // ���������� ���������� ������� � ������
		symbol_code = fgetc(file_from);
	if (symbol_code == (int)'"')
	{
		symbol_code = fgetc(file_from);  // ���������� �������
		in_quotes = true;
	}
	int current_position = 0;
	bool continue_reading = true;
	for (; continue_reading; ++current_position)
	{
		buffer[current_position] = (char)symbol_code;
		symbol_code = fgetc(file_from);
		continue_reading *= current_position < buffer_size - 1;  // ��� ���� ����� � ������ ��� ���������� ������ (���� ������ ��������� ��� '\0')
		continue_reading *= symbol_code != EOF;  // ������� �� ����� �����
		if (in_quotes && symbol_code == (int)'"')
			continue_reading = false;  // ��������� ����������� ������� � ���� �����������
		else if (in_quotes == false)
			continue_reading *= (isspace((unsigned char)symbol_code) == 0);  // ���� �������� ������������ ������, �� ���������� ������
	}
	// ����� �� ����� �����, �� �� ����� ����������� �������; �� ��� �������, ������������ ����!
	if (symbol_code == EOF && in_quotes)
		read_result = EXIT_USER_FAILURE;
	// ��������� ���� �����, � �������� ��� �� ����������; ������ ��������� ������!
	else if (current_position == buffer_size - 1 && !((in_quotes && symbol_code == '"') || (!in_quotes && isspace((unsigned char)symbol_code))))
		read_result = EXIT_MEMORY_FAILURE;
	// ������� ��������� ��������! ����� �������� ����-���������� ������
	else
		buffer[current_position] = '\0';
	if (strchr(buffer, '"') != NULL) // � ���������� ��� �������� �������, ������ ������������ ����������� ��� ��������
		read_result = EXIT_USER_FAILURE;
	return read_result;
}

bool path_is_txt(const char* path)
{
	bool is_txt = true;
	if (path == NULL || strlen(path) <= 4)
		is_txt = false;
	else
	{
		int term_null_index = strlen(path);
		if (strcmp(".txt", path + term_null_index - 4) != 0)
			is_txt = false;
	}
	return is_txt;
}