#include "constants.h"
#include "train_module.h"
#include "hash_table_lib.h"
#include "corrector_lib.h"
#include "words_handling.h"
#include "helpful_functions.h"
#include "files_working.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


// ������� ������������������ ���������, ��������� ��������� ������ cfg. �������� ���������������� �������� �� ���������� �������
// �� ��������� �������� ��������� ������ (� ������ ��������� ��������) � �������� ������, ������������ ��� ���-�������
// � ������ ��������� �������� ����� EXIT_SUCCESSFULLY
// ���� �� �������� �����-�� �� ������ - ����� EXIT_FILE_FAILURE
// ���� ������������ ���-�� ��� ����������� - ����� EXIT_USER_FAILURE
// EXIT_MEMORY_FAILURE �� ������������, �.�. ���� �� ������ �������� ������ ��� �����-���� �����, �� �� ��� ���������� � �������, �� ��� �� ��������, �.�. ������ ����� �������� � ��� ����
static int train_initialized(Corrector* corrector, const Pointer* cfg);

// ������� ������������������ ��������� ������ �� ����� text_file_name. �� ��������� �������� ��������� ����
// � ������ ��������� �������� ���������� EXIT_SUCCESSFULLY
// ���� text_file_name �� �������� ���� � ����� .txt - ����� EXIT_USER_FAILURE
// ���� �� �������� ���� - ����� EXIT_FILE_FAILURE
static int train_one_file(Corrector* corrector, const char* text_file_name);

// ��������� ����� �� ����� � �����. ����� ��������������� ��� �������� ������.
// ���������� true, ���� ����� ����� ��� �� ���������, ����� ���������� false.
// ���������� ���������� � �������������� ������� (� �.�. �����), ���� �� �������� ������ ������������ � ����������������.
// ����� ��������� �� max_word_length �������� ������������ � � ����� ������ '\0' (�.�. � ������� ������ ���� ����� ������� �� max_word_length + 1 ��������).
// ���������� ���, ���� �� ������� max_word_length �������� ��� �� ���������� ����� ����������/�������������� ������ (����� ������)/EOF. ��� ���� ������ ���������.
// ���� ����� ��������� max_word_length - ��� ������� �� ����. ��������� ��� ���������� ������� �� ������ �����������, � ����� ������� (������ '\0' � ������).
// �.�., ��� ���������� � �������������� ����� (� �.�. �����) � ������ � � ����� ����� ���������. �������� ���������������� ������� � ����� �������, � ����� ����� ������ �����.
static bool read_train_word(FILE* text_file, char* buffer, unsigned max_word_length);

/*=================================================================================================================================================================================*/

int train_new_model(const Pointer* cfg)
{
	int exit_code = EXIT_SUCCESSFULLY;
	printf("\n### �������� ����� ������...\n");
	Corrector new_corrector;
	unsigned max_word_length = *(int*)cfg[3];
	exit_code = corrector_init(&new_corrector, max_word_length, NULL);  // �������������� ���������, �������� ������ ��� �������

	if (exit_code == EXIT_SUCCESSFULLY)  // ��� ����������� ������ ��� ������ ������ ����������
		exit_code = train_initialized(&new_corrector, cfg);  // ������� ������ ���������� ���������� ������

	return exit_code;
}

int train_existed_model(const Pointer* cfg)
{
	int exit_code = EXIT_SUCCESSFULLY;
	printf("\n### �������� ������������ ������...\n");
	Corrector existed_corrector;
	const char* model_file_name = (const char*)cfg[1];
	exit_code = corrector_load(&existed_corrector, model_file_name);  // ��������� ������������ ������ �� �����; ���� �� ������ ���������, �� ��� ����������� ������ ��������� ������
	if (exit_code == EXIT_SUCCESSFULLY)
	{
		printf("\n### �������� ������� ���������\n### ���������� ������������ ������...\n");
		exit_code = train_initialized(&existed_corrector, cfg);
	}
	
	return exit_code;
}

/*=================================================================================================================================================================================*/

static int train_initialized(Corrector* corrector, const Pointer* cfg)
{
	int exit_code = EXIT_SUCCESSFULLY;
	const char* model_file_name = (const char*)cfg[1], * text_file_name = (const char*)cfg[2];
	exit_code = train_one_file(corrector, text_file_name);
	
	while (exit_code == EXIT_SUCCESSFULLY && yes_no_question("\n����� ������. ��������� ������ ��� �� ����� ������?\n"))
	{
		printf("����� � �������� ������� ������ ����� ��������� ANSI. ��� ���������� ������� ��������� UTF-8\n");
		printf("������� ������ ��� ������������� ���� � ������ ��� �������� � ������� path\\file_name.txt: ");
		char text_file_name[BUFFER_SIZE];
		exit_code = read_param_from_console(text_file_name, BUFFER_SIZE);
		if (exit_code == EXIT_SUCCESSFULLY)
			exit_code = train_one_file(corrector, text_file_name);
		else if (exit_code == EXIT_USER_FAILURE)
			printf("���� ����� �����������\n");
		else if (exit_code == EXIT_MEMORY_FAILURE)
			printf("������� ������� ���� � �����\n");
	}
	
	if (exit_code != EXIT_SUCCESSFULLY)  // �� �����-�� ����� ��������� ������, �� ��������� ������ (���� ��� ����� ������ - ����� ������ ����, ���� ������, �� ���� �� ���������)
		printf("��������, �� ����� �������� ��������� ������. ������ ��������� � ��� ���������, � ������� ���� �� ��������. ��������� �������\n");
	else  // ��������� ������
	{
		printf("\n### �������� ���������\n### ���������� ������ � ����...\n");
		exit_code = corrector_save(corrector, model_file_name);
		if (exit_code != EXIT_SUCCESSFULLY)
			printf("������ ��� ���������� ������. ������ ������ �� ��������. ������� �����. �������� ���� ���������!\n");
		else
			printf("\n### ������ ������� ���������!\n");
	}
	corrector_destroy(corrector);  // ������ ������, ���������� ��� ������� � �����
	return exit_code;
}

static int train_one_file(Corrector* corrector, const char* text_file_name)
{
	int exit_code = EXIT_SUCCESSFULLY;
	FILE* text_file = NULL;
	if (path_is_txt(text_file_name))  // ����� �������� ���� - ������������� ���� � ����� .txt
		text_file = fopen(text_file_name, "r");
	else
	{
		printf("������� ����� ���� � ���������� ������. �������� ��������\n");
		exit_code = EXIT_USER_FAILURE;
	}

	if (exit_code == EXIT_SUCCESSFULLY && not_null_ptr(text_file, "�� ������� ������� ���� ��� ��������\n"))  // ������� ����
	{
		char buffer_for_word[MAX_AVAILABLE_WORD_LENGTH + 1];  // �����, ����� ������� ����� �� ������; +1, �.�. ����� �������� '\0' � �����
		bool file_not_ended = true;
		char* learning_word = NULL;
		int memory_failures = 0;  // �������, ������� ���� �� ������ ��������
		while (file_not_ended)
		{
			file_not_ended = read_train_word(text_file, buffer_for_word, corrector->max_word_length);  // ��������� �������������� �����, ��������� ����������� � ��������������� ���������
			learning_word = prepare_train_word(buffer_for_word);  // ���������, ��� ��� �������� ��� ��������; ���� �������� - �������� ��� ���� ������ � ���������� ���������
			if (learning_word != NULL)  // ����� �������� ��� ��������, ���������� ���, �������������/��������� �������
				memory_failures += (corrector_learn(corrector, learning_word) == EXIT_MEMORY_FAILURE) ? 1 : 0;
		}
		if (memory_failures != 0)  // ������� �� ��� �����, ������ ����� ��������, �� �� �� ��� ������ ����
			printf("�� ������� �������� ������ ��� %d ����. ������ ����� �������� �� ��������� ������, �� ����������� ��� ��������� ��������\n", memory_failures);
		fclose(text_file);
	}
	else  // ���� �� ��������
		exit_code = EXIT_FILE_FAILURE;
	return exit_code;
}

static bool read_train_word(FILE* text_file, char* buffer, unsigned max_word_length)
{
	// ��������, ��� ���� ������, ��������� �����, � train_one_file, ������ � ���������� ������� read_train_word
	bool file_not_ended = true;
	int new_symbol = fgetc(text_file);  // fgetc ���������� �� 0 �� 255, �.�. ��� unsigned char. � ������ ����� ����� ���������� -1. � ������� char ������������ �� -128 �� 127
	while (file_not_ended && (isspace(new_symbol) != 0 || ispunct(new_symbol) != 0))  // ������� �� ������ <ctype.h> �������� ��� ��� � unsigned char
	{
		new_symbol = fgetc(text_file);  // ���������� ��� ���������� ������� � ����� ���������� (� �.�. ������), ���� �� �������� ����� ������ ������ ��� ����� �����
		file_not_ended = (new_symbol != EOF);
	}
	
	if (file_not_ended == false)  // ����� �� ����� ����� � �� ��������� �����
		buffer[0] = '\0';  // ������� �����

	else  // ����� ������������ ���������������� ������, �������� ���������� ����� ������� � ����� (����� ����� ������� ������ ���� ������������, �� ����� - ���)
	{
		int counter = 0;  // ������� ���������� ��������
		for (; counter < max_word_length && file_not_ended && isspace(new_symbol) == 0 && (ispunct(new_symbol) == 0 || new_symbol == (int)'-'); ++counter)
		{
			// ���������� ����� ������� � �����, ���� �� �������� ����� �� ��������, ���� �� ���������� ���� ��� ���� �� �������� ���������� ��� �������������� ������ (����� ������)
			buffer[counter] = new_symbol;  // ���������� ������
			new_symbol = fgetc(text_file);  // ��������� ����� ������
			file_not_ended = (new_symbol != EOF);  // �������� �� ����� �����
		}
		// ���� ����� �� ����� �����, ����� �� �������� (�������� counter = max_word_length, �� ��� ���������� ���������� ������) => ������ ����-���������� ����� �����
		// ���� ������� ��������� ����� (�� max_word_length �������� ������������) � ���������� ���������� ��� �������������� ���������� ������ => ������ ����-���������� ����� �����
		if (file_not_ended == false || isspace(new_symbol) != 0 || (ispunct(new_symbol) != 0 && new_symbol != (int)'-'))
			buffer[counter] = '\0';  // ��������� �����
		else  // ����� �� �����, ������ ��� ��������� max_word_length ��������, � ����� ����� ���������������� ������������ ��� ����� => ����� ��� �� ����������� => ��� ������� �������
		{
			buffer[0] = '\0';  // �������� ����������� �����
			while (isspace(new_symbol) == 0 && file_not_ended)  // ���������� ���������� ������� �� ����������� ��� �� ����� �����, ����� � ��������� ��� ������ � ������ �����
			{
				new_symbol = fgetc(text_file);
				file_not_ended = (new_symbol != EOF);
			}
		}
		erase_punct_remainder(buffer); // ������� ������ � ����� �����, ���� ��� ����
	}
	return file_not_ended;
}