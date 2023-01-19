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
	if (tmp_created != false) // временный файл создан
	{
		tmp_file = fopen(tmp_file_name, open_mode);
		if (tmp_file == NULL) // временный файл создался, но почему-то не открылся; удалием его и вернём NULL
			delete_if_tmp(tmp_file_name);
	}
	return tmp_file;
}

void delete_if_tmp(const char* path_to_file)
{
	if (path_to_file != NULL && strstr(path_to_file, TMP_SUFFIX) != NULL) // это был временный файл с параметрами при запуске; надо удалить файл
	{
		bool successfully_removed = file_remove(path_to_file, "Ошибка удаления временного файла. Удалите его самостоятельно при необходимости:\n");
		if (successfully_removed == false)
			// Не получилось удалить временный файл; сообщим пользователю об этом и предложим удалить самостоятельно, предоставив абсолютный путь
			free(rel_to_abs_way(path_to_file, true, true)); 
	}
}

bool file_exists(const char* file_name)
{
	bool file_exists = false;
	FILE* existed_file = fopen(file_name, "r"); // пробуем открыть на чтение, чтобы проверить, существует ли файл
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
	if (file_already_exists)  // вдруг файл уже существует, тогда новый файл не создаётся и возвращается false
		printf("Попытка перезаписи существующего файла. Отмена операции создания нового файла\n");
	else  // если файла с таким именем не существует - можем создать
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
	if (successfully_written) // файл открыт
	{
		for (; strings_amount > 1 && successfully_written; --strings_amount) // печатаем строки пока не останется одна или не произойдёт ошибка записи
		{
			successfully_written *= (fprintf(file, "%s%s%s%s", shell, strings_for_writing[0], shell, delimeter) >= 0) ? true : false; // пишем в файл, проверяем, что успешно записали
			++strings_for_writing;
		}
		if (strings_amount == 1 && successfully_written) // до этого всё прошло успешно, добавляем последнюю запись
			successfully_written *= (fprintf(file, "%s%s%s", shell, strings_for_writing[0], shell) >= 0) ? true : false;
		if (successfully_written) // до этого всё прошло успешно, добавляем end
			successfully_written *= (fputs(end, file) == EOF) ? false : true;
	}
	return successfully_written;
}

char* rel_to_abs_way(const char* rel_name, bool print_way, bool print_errors)
{
	char* working_directory = _getcwd(NULL, BUFFER_SIZE);  // получаем текущую директорию, в случае ошибки вернёт NULL; память выделяется динамически
	char* abs_path = NULL;
	if (errno == ENOMEM && print_errors)  // недостаточно памяти для выделения BUFFER_SIZE байтов динамической памяти
		printf("\nНе удалось выделить память под имя рабочей директории. Попробуйте запустить программу из корневой папки\n");
	else if (errno == ERANGE && print_errors)  // имя пути длиннее BUFFER_SIZE символов
		printf("\nИмя рабочей директории слишком длинное. Попробуйте запустить программу из корневой папки\n");
	else if (rel_name == NULL)
		abs_path = working_directory;
	else  // надо добавить относительный путь к рабочей директории, выделяем новую память
	{
		size_t directory_length = strlen(working_directory);
		size_t rel_name_length = strlen(rel_name) + 2;  // надо добавить к рабочей директории слеш \, относительный путь к файлу и нуль-терминатор \0
		size_t abs_path_length = directory_length + rel_name_length;
		abs_path = (char*)retry_malloc(abs_path_length * sizeof(char), MAX_MALLOC_ATTEMPTS);
		if (abs_path != NULL)  // всё выделилось, составляем абсолютный путь в новой памяти
		{
			strcpy(abs_path, working_directory);
			strcat(abs_path, "\\");
			strcat(abs_path, rel_name);
		}
		else if (print_errors)  // не удалось выделить память под абсолютный путь, сообщаем пользователю
			printf("\nНе удалось выделить память под абсолютный путь\n");

		free(working_directory);  // память под имя рабочей директории в _getcwd выделялась динамически
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
	while (isspace((unsigned char)symbol_code) != 0)  // пропускаем пробельные символы в начале
		symbol_code = fgetc(file_from);
	if (symbol_code == (int)'"')
	{
		symbol_code = fgetc(file_from);  // пропускаем кавычку
		in_quotes = true;
	}
	int current_position = 0;
	bool continue_reading = true;
	for (; continue_reading; ++current_position)
	{
		buffer[current_position] = (char)symbol_code;
		symbol_code = fgetc(file_from);
		continue_reading *= current_position < buffer_size - 1;  // ещё есть место в буфере для дальнейшей записи (один символ оставляем под '\0')
		continue_reading *= symbol_code != EOF;  // считали не конец файла
		if (in_quotes && symbol_code == (int)'"')
			continue_reading = false;  // встречены закрывающие кавычки и были открывающие
		else if (in_quotes == false)
			continue_reading *= (isspace((unsigned char)symbol_code) == 0);  // если прочитан непробельный символ, то продолжаем чтение
	}
	// Дошли до конца файла, но не нашли закрывающую кавычку; ну что сделать, некорректный ввод!
	if (symbol_code == EOF && in_quotes)
		read_result = EXIT_USER_FAILURE;
	// Заполнили весь буфер, а параметр ещё не закончился; ошибка выделения памяти!
	else if (current_position == buffer_size - 1 && !((in_quotes && symbol_code == '"') || (!in_quotes && isspace((unsigned char)symbol_code))))
		read_result = EXIT_MEMORY_FAILURE;
	// Успешно прочитали параметр! Нужно добавить нуль-терминатор строки
	else
		buffer[current_position] = '\0';
	if (strchr(buffer, '"') != NULL) // в параметрах ещё остались кавычки, значит пользователь некорректно ввёл параметр
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