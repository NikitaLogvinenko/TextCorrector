#include "constants.h"
#include "train_module.h"
#include "hash_table_lib.h"
#include "corrector_lib.h"
#include "words_handling.h"
#include "helpful_functions.h"
#include "files_working.h"
#include <stdio.h>
#include <stdlib.h>


// ������� ������������������ ���������, ��������� ��������� ������ cfg. �������� ���������������� �������� �� ���������� �������
// �� ��������� �������� ��������� ������ (� ������ ��������� ��������) � �������� ������, ������������ ��� ���-�������
// � ������ ��������� �������� ���������� EXIT_SUCCESSFULLY, ����� EXIT_FILE_FAILURE (�� �������� �����-�� �� ������) ��� EXIT_USER_FAILURE (������������ ����������� ��� ��� ������)
static int train_initialized(Corrector* corrector, const Pointer* cfg);

// ������� ������������������ ��������� ������ �� ��������� ����� text_file. �� ��������� �������� ��������� ����
// � ������ ��������� �������� ���������� EXIT_SUCCESSFULLY, ����� EXIT_FILE_FAILURE (�� �������� ����)
static int train_one_file(Corrector* corrector, FILE* text_file);

/*=================================================================================================================================================================================*/

int train_new_model(const Pointer* cfg)
{
	int exit_code = EXIT_SUCCESSFULLY;
	printf("\n###�������� ����� ������...\n");
	Corrector new_corrector;
	unsigned max_word_length = *(int*)cfg[3];
	exit_code = corrector_init(&new_corrector, max_word_length, NULL);  // �������������� ���������, �������� ������ ��� �������

	if (exit_code == EXIT_SUCCESSFULLY)  // ��� ����������� ������ ��� ������ ������ ����������
		exit_code = train_initialized(&new_corrector, cfg);  // ������� ������ ���������� ���������� ������

	return exit_code;
}

int train_existed_model(const Pointer* cfg)//---------------------------------------------------------------------------------------------
{
	int exit_code = EXIT_SUCCESSFULLY;
	printf("\n###���������� ������������ ������...\n");
	// ������� ������, ������� ���� � �������, ��������� ������������ �����, ���������������� �������, ��������� ��������� �����, ������� ����
	// ������� train_inizialized � �������� cfg, ����� ��� � ������������������ ������ ��� ����� �����


	return exit_code;
}

/*=================================================================================================================================================================================*/

static int train_initialized(Corrector* corrector, const Pointer* cfg)
{
	int exit_code = EXIT_SUCCESSFULLY;
	const char* model_file_name = (const char*)cfg[1], * text_file_name = (const char*)cfg[2];
	FILE* text_file = fopen(text_file_name, "r");
	exit_code = train_one_file(corrector, text_file);  // ��������, ��� ���� ��������, � ����� �������� ����� ���������� ������
	
	while (exit_code == EXIT_SUCCESSFULLY && yes_no_question("\n����� ������. ��������� ������ ��� �� ����� ������?\n"))
	{
		printf("����� � �������� ������� ������ ����� ��������� ANSI. ��� ���������� ������� ��������� UTF-8\n");
		printf("������� ������ ��� ������������� ���� � ������ ��� �������� � ������� path\\file_name.txt: ");
		char text_file_name[BUFFER_SIZE];
		read_param_from_console(text_file_name, BUFFER_SIZE);
		if (path_is_txt(text_file_name))  // ����� �������� ���� - ������������� ���� � ����� .txt
		{
			FILE* text_file = fopen(text_file_name, "r");
			exit_code = train_one_file(corrector, text_file);
		}
		else
		{
			printf("������� ����� ����. �������� ��������\n");
			exit_code = EXIT_USER_FAILURE;
		}
	}
	
	if (exit_code != EXIT_SUCCESSFULLY)  // �� �����-�� ����� ��������� ������, �� ��������� ������ (���� ��� ����� ������ - ����� ������ ����, ���� ������, �� ���� �� ���������)
		printf("��������, �� ����� �������� ��������� ������. ������ ��������� � ��� ���������, � ������� ���� �� ��������. ��������� �������\n");
	else  // ��������� ������
	{
		FILE* model_file = fopen(model_file_name, "w");  // �������������� ������
		exit_code = corrector_save(corrector, model_file);  // �������� �������� � �������� ����� ������ ���������� ������
		if (exit_code != EXIT_SUCCESSFULLY)
			printf("������ ��� ���������� ������. ������ ������ �� ��������. ������� �����. �������� ���� ���������!\n");
	}

	corrector_destroy(corrector);  // ������ ������, ���������� ��� ������� � �����
	printf("\n###�������� ���������\n");
	return exit_code;
}

static int train_one_file(Corrector* corrector, FILE* text_file)
{
	int exit_code = EXIT_SUCCESSFULLY;
	if (not_null_ptr(text_file, "�� ������� ������� ���� ��� ��������\n"))
	{
		char buffer_for_word[MAX_AVAILABLE_WORD_LENGTH + 1];  // +1, �.�. ����� �������� '\0'
		bool file_not_ended = true;
		char* learning_word = NULL;
		while (file_not_ended)
		{
			file_not_ended = read_train_word(text_file, buffer_for_word, corrector->max_word_length);
			learning_word = prepare_train_word(buffer_for_word);
			if (learning_word != NULL)  // ����� �������� ��� ��������, ���������� ���, �������������/��������� �������
				corrector_learn(corrector, learning_word);
		}
		fclose(text_file);
	}
	else
		exit_code = EXIT_FILE_FAILURE;
	return exit_code;
}