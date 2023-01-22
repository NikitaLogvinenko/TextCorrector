#include "constants.h"
#include "edit_module.h"
#include "corrector_lib.h"
#include "helpful_functions.h"
#include "files_working.h"
#include "words_handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


// ��������������� ��������� ����� �� ������ ����� � ���������� � ����� �����������������
// ������ ��� ������ ���� ���������
// �� ���������� ���������� EXIT_SUCCESSFULLY � ������ ��������� ����������, ����� ��� ������:
// * EXIT_MEMORY_FAILURE - �� ���������� ������
// * EXIT_FILE_FAILURE - �� �������� ����
// * EXIT_USER_FILE - ������ �� ���� ������������
static int edit_by_loaded(Corrector* corrector, const Pointer* cfg);

// ����������� ������ corrector ����������� ���� initial_file_name � ��������� � edited_file_name
// ����������� ���������� ������� ���� ������������������ � �������������� ���� - size_tol
// ����������� ���������� ���������� ������������������ ����� �� �������������� - threshold
// ���� ����� ������� ��������������� � ���������� - ����� EXIT_SUCCESSFULLY
// ���� �� ��������� ���� ��� ������ ��� ������ - ����� EXIT_FILE_FAILURE
// ���� ����� ������ ������� ����������� - ����� EXIT_USER_FAILURE
// ��� ����� ���������� �������������� ������. ����� ����������� �� ����� � �����. ��� ������������������ �������� ����� ���� ���� �����
// ����� �������������� ����� ����� ������������ � ���� � �� ���������, �� �������� ������� ������ �� �����, ������� ��� �� ������ ����� ��������� ������ �������
// ����� ������� ������ ���������� �� ��� ������ ����� �����, � ������ ���� ��� � ������
// �.�. ������ ������ ���������� ������ �������������, �� ������ ������ ���� �� �����
static int edit_one_file(Corrector* corrector, const char* initial_file_name, const char* edited_file_name, const int size_tol, const int threshold);

// ��������� corrector ��� ������ ���� ��������, � ��������� size_tol � threshold ��� �������������� ������.
// ���� ����� ��������� ����� ���� ��� �� �������, �� ����� true, ����� false
// ����������� ���� ����� �� ����� initial_file, ��� ���� ������������ ����� initial_buffer. ���� ����, ����� ������������� � edited_buffer. � ����� ������ ��� ��������� � edited_file.
// ������ ������� ������ ���� �� ����� corrector->max_word_length + size_tol + 2:
// +1 �� ��������� ����������� ������ ����� ����� (���� ����� ����� �� ��� EOF), ������� ���� ��������� � ���� ����� �����, � +1 �� �������� ����� ������ '\0'.
// ���� ����� ���� ���������������, �� � �������� ����������������� ���� words_edited_counter ������������ 1.
// ----------------------------------------------------------------------------
// ������� �����������, ���� �� ��������� ������ ����� ����������� ����� (��. �� ���� ��������� � ����� ���������). ���� ���� - ������ ��������� � edited_file.
// ����� ��� ����������� ������� ����������� � ��������� � edited_file ��� ��������������.
// ���� ��� ������� ���������� ������� ���������� ��� space_demanding() - ��.��� ������� � words_handling - ������� �������� ������. 
// ����� �������������� ���������� ����� �� ������� ������������ ������� (����� ������ c�������� ���������).
// ���� ��������� corrector->max_word_length + size_tol �������� � ��������� ������ ��������� ��� �����, �� ����� ������� ������� � ��� �� ��������� �������� ������ �� corrector. 
// ����� ������� ��� ������� �� ������� ������������� ������������ � edited_file ��� ���������.
// ���� ����� �� ������� �������, �� ���������� ������� corrector_edit, ������ ������� ���������� ��������������, ���� ��� �����, � ����� ��������� � edited_file
// ����������� ������ ����� ����� ����� �������� � ������, ��������� � ���� � ������� corrector_edit � ������������ � ������� ������� � initial_buffer. 
// ���� ����� ����� ��� ����� EOF, �� � ������ '\0' ����� ����� ����� �����.
// ��� ����������, �.�. ��� ������ ������� ������� ������� ���������, ���� ������ ������ � initial_file ���������, � ������ ������ � initial_buffer �������� space_demanding(), 
// ������ � �������� ����� ��� �������� ������. � ����� ������ ������� �������� ������ � ���� ����� ��� ������ ��������� �������� �� initial_file.
static bool edit_one_word(Corrector* corrector, unsigned size_tol, unsigned threshold,
	FILE* initial_file, FILE* edited_file, char* initial_buffer, char* edited_buffer, unsigned* words_edited_counter);

// ��������������, ��� �� ����� ��� ������ ������ �� ����� � symbol_code - ������ ��������� ������, � ����� ��� ��� ����� prev_code.
// ���� prev_code ������� ����� ���� ������ - ������� ������ � edited_file.
// ����� ���������� ��� ��������� ������� � ������ � initial_buffer, ���� �� �������� ����������� ������ � �� �����.
// ���� ����� ��� ������ �� ����� ����� - ����������� � ����� true, ����� ����� false.
// ���� ����� �� ������������ ����������� �������, �� �� EOF, �� ���������� ���� ������ ����� ����� � ������.
// ���� ��������� ����� �������� max_letters_init � ������������� �����, �� ������� ��� ������� �� ������� ������������ � �� ������, � � initial_buffer[0] ������� ��������� ����������.
// ���� ������ �� ��������, �� ������ �� ������� � ����, � ��������� ����� � ������ ������ � ����������� �������� ����� ���� ��� ���������� ���������.
static bool read_word_to_edit(FILE* initial_file, FILE* edited_file, char* initial_buffer, int max_letters_initial, int symbol_code, int prev_code);


/*=================================================================================================================================================================================*/

int edit_text(const Pointer* cfg)
{
	int exit_code = EXIT_SUCCESSFULLY;
	printf("\n### �������� ������������ ������...\n");
	Corrector existed_corrector;
	const char* model_file_name = (const char*)cfg[1];
	exit_code = corrector_load(&existed_corrector, model_file_name);  // ��������� ������������ ������ �� �����; ���� �� ������ ���������, �� ��� ����������� ������ ��������� ������
	
	if (exit_code == EXIT_SUCCESSFULLY)
	{
		printf("\n### �������� ������� ���������\n### �������������� ������...\n");
		exit_code = edit_by_loaded(&existed_corrector, cfg);
	}

	return exit_code;
}

/*=================================================================================================================================================================================*/


static int edit_by_loaded(Corrector* corrector, const Pointer* cfg)
{
	int exit_code = EXIT_SUCCESSFULLY;
	const char* initial_file_name = (const char*)cfg[2], * edited_file_name = (const char*)cfg[3];
	unsigned size_tol = *(int*)cfg[4], threshold = *(int*)cfg[5];
	exit_code = edit_one_file(corrector, initial_file_name, edited_file_name, size_tol, threshold);

	while (exit_code == EXIT_SUCCESSFULLY && yes_no_question("\n����� ��������������. ��������������� ��� ����?\n"))
	{
		printf("����� � �������� ������� ������ ����� ��������� ANSI. ��� ���������� ������� ��������� UTF-8\n");
		char initial_file_name[BUFFER_SIZE], edited_file_name[BUFFER_SIZE];
		printf("������� ������ ��� ������������� ���� � ������ ��� �������������� � ������� path\\file_name.txt: ");
		int read_path_code_1 = read_param_from_console(initial_file_name, BUFFER_SIZE);
		printf("������� ������ ��� ������������� ���� ��� ���������� ������������������ ������ � ������� path\\file_name.txt: ");
		int read_path_code_2 = read_param_from_console(edited_file_name, BUFFER_SIZE);
		if (read_path_code_1 == EXIT_USER_FAILURE || read_path_code_2 == EXIT_USER_FAILURE)
		{
			exit_code = EXIT_USER_FAILURE;
			printf("���� %d ����� �������\n", (read_path_code_1 == EXIT_USER_FAILURE) ? 1 : 2);
		}
		else if (read_path_code_1 == EXIT_MEMORY_FAILURE || read_path_code_2 == EXIT_MEMORY_FAILURE)
		{
			exit_code = EXIT_MEMORY_FAILURE;
			printf("���� %d ������� �������\n", (read_path_code_1 == EXIT_MEMORY_FAILURE) ? 1 : 2);
		}
		else
			exit_code = edit_one_file(corrector, initial_file_name, edited_file_name, size_tol, threshold);
	}

	if (exit_code != EXIT_SUCCESSFULLY)
		printf("��������, �� ����� �������������� ��������� ������. ��������� �������\n");
	else
		printf("\n### �������������� ������� ���������\n");
	
	corrector_destroy(corrector);  // ������ ������, ���������� ��� ������� � �����
	return exit_code;
}

static int edit_one_file(Corrector* corrector, const char* initial_file_name, const char* edited_file_name, const int size_tol, const int threshold)
{
	int exit_code = EXIT_SUCCESSFULLY;
	FILE* initial_file = NULL, * edited_file = NULL;
	if (path_is_txt(initial_file_name) && path_is_txt(edited_file_name))  // ���� ���� �������� ������ � .txt ������ - ������� ������� � ������ ������
	{
		initial_file = fopen(initial_file_name, "r");
		edited_file = fopen(edited_file_name, "w");
	}
	else  // ���� ������� �������
	{
		printf("���� %d �� �������� ���� � .txt �����. �������������� ��������\n", path_is_txt(initial_file) ? 2 : 1);
		exit_code = EXIT_USER_FAILURE;
	}

	if (exit_code == EXIT_SUCCESSFULLY && not_null_ptr(initial_file, "�� ������� ������� ���� � �������\n") && not_null_ptr(edited_file, "�� ������� ������� ���� ��� ����������\n"))
	{
		// ����� ���������, ����� �������������
		bool file_not_ended = true;  // ���������, ��� ���� ���������� ��������������
		unsigned words_edited = 0;  // ������� ����������������� ����
		char initial_buffer[MAX_AVAILABLE_WORD_LENGTH + MAX_AVAILABLE_MISSES + 2], edited_buffer[MAX_AVAILABLE_WORD_LENGTH + MAX_AVAILABLE_MISSES + 2];
		// +2, �.�. +1 �� ����������� ���������� ������ ����� �����, ������� ��������� ����� ����� � ��� ���� ���������, � +1 �� '\0'
		initial_buffer[0] = ' ';  // ��� �����, ����� ����� ����� ������ �������� ����� �� ����� ������
		// ������������ ����� �������������� ����� ����� �� max_size_tol ��������� ����� ������ �������� ���������� ����� (���� � ��� ������ MAX_AVAILABLE_MISSES ���� ������)
		while (file_not_ended)
			file_not_ended = edit_one_word(corrector, size_tol, threshold, initial_file, edited_file, initial_buffer, edited_buffer, &words_edited);
		printf("\n��������������� ����: %d\n", words_edited);
	}
	else // ���� ��� ��� ����� �� ���������
	{
		exit_code = EXIT_FILE_FAILURE;
		if (initial_file != NULL)
			fclose(initial_file);
		if (edited_file != NULL)
			fclose(edited_file);
	}

	return exit_code;
}

static bool edit_one_word(Corrector* corrector, unsigned size_tol, unsigned threshold,
	FILE* initial_file, FILE* edited_file, char* initial_buffer, char* edited_buffer, unsigned* words_edited_counter)
{
	bool file_not_ended = true;
	int max_letters_initial = corrector->max_word_length + size_tol;  // ������� �������� ���� � ������� ����� ���� � �����
	int prev_code = initial_buffer[0];  // ���������� ���������� � ���� ������
	int symbol_code = fgetc(initial_file);  // fgetc ���������� ��� unsigned_char ��� EOF<0
	file_not_ended = (symbol_code != EOF);  // ��������, ����� ���� ����������, ��� ������ ������� �������
	if (file_not_ended)  // ���� �� ����������, ���������� ��������������
	{
		prev_code = space_before(edited_file, prev_code, symbol_code);  // ������ ������ ����� ������ ���������, ���� ����
		while (file_not_ended && is_letter(symbol_code) == false)  // ��� ����������� ������� (� �.�. ������ ����� ������) �������� ������� � ����� ����
		{
			fputc(symbol_code, edited_file);  // ����������� ������� ������� � ����
			prev_code = symbol_code;  // ����������, ��� ������
			symbol_code = fgetc(initial_file);  // ��������� ����� ������
			file_not_ended = (symbol_code != EOF);  // �������� ����� �����
		}
		if (file_not_ended)  // ����� ����� � ���������� �����, � �� ����� �� ����� �����
			file_not_ended = read_word_to_edit(initial_file, edited_file, initial_buffer, max_letters_initial, symbol_code, prev_code);  // ��������� ����� � �����
		
		// ���� ������ ������ - ����� (������ ����� ������ ����� ���� �� �����) => ����� ����������� � ����� => ��� ����� ���������� � ������� ��������
		if (is_letter(initial_buffer[0]))
		{
			prev_code = corrector_edit(corrector, initial_buffer, edited_buffer, edited_file, size_tol, threshold, words_edited_counter);  // ����������� � ������� ����� � ����
			initial_buffer[0] = prev_code;  // ��������� ������ ���������� � initial_buffer[0]
		}
	}
	return file_not_ended;
}

static bool read_word_to_edit(FILE* initial_file, FILE* edited_file, char* initial_buffer, int max_letters_initial, int symbol_code, int prev_code)
{
	prev_code = space_before(edited_file, prev_code, symbol_code);  // ������ ������ ����� ������, ���� ����
	bool file_not_ended = true;
	// ��������� ����� � ����� ���� ���� �� ���������� ����, ���� ���� �� �������� ������ ����������� ������ � �� �����, ���� ���� �� �������� ����� ���� � ������������� �����
	unsigned letter_index = 0;
	for (; letter_index < max_letters_initial && file_not_ended && (is_letter(symbol_code) || symbol_code == '-'); ++letter_index)
	{
		initial_buffer[letter_index] = symbol_code;  // ���������� ����� ��� ����� � �����
		symbol_code = fgetc(initial_file);  // ��������� ����� ������
		file_not_ended = (symbol_code != EOF);  // �������� ����� �����
	}
	// ������ ��������, ������ ����� �� �����
	initial_buffer[letter_index] = '\0';  // �� �������� ���������� ����� ����� (����� �� ��������, � ��� ������, ��� ������� ��������� ����� �� ����� - ���� ��� ��� (��. � �����))
	if (file_not_ended && (is_letter(symbol_code) || symbol_code == '-')) // ��������� ����� ��������� ��-�� ����, ��� ������� �������� ���� � ��������� ������ ����� ����� ��� �����
	{
		fputs(initial_buffer, edited_file);  // �������� ��, ��� ���� � ������
		while (file_not_ended && (is_letter(symbol_code) || symbol_code == '-'))  // ���������� ��� ����������� ����� � ������, ���� �� �������� ������ ������ ��� ����� �����
		{
			fputc(symbol_code, edited_file);  // ������� ������ � ���� (��������� ����������� � for ����� ����� �� ��� ������� � ����� � ��� ���� ������� ��������)
			symbol_code = fgetc(initial_file);  // ��������� ����� ������
			file_not_ended = (symbol_code != EOF);  // �������� ����� �����
		}
		if (file_not_ended)  // ���� ����� �� ������������ ����������� �������, � �� �� ����� �����, �� ���� ������� ��� � ���������� � initial_buffer[0]
		{
			fputc(symbol_code, edited_file);
			initial_buffer[0] = symbol_code;
		}
		// ������ ���� ������� ���� ��� ���������� �� � ����� ���������� ������ ��������
		// ���� ���� ������������� �� ���������� ������, �� ����� ���� ������������ � corrector_edit, ������� ����� ������� � ������ � ������ ������ - ���������.
		// ���� �� ���� ������������� ��� ������ �������� ����� ����������, �� ������ ������ ������������ �� ����, �.�. �� ��� ������ � ����.
		// �� � initial_buffer �� ��� ��� �������� ������ �������� �����. ����� ��� �� ������������� � corrector_edit, �������� � ������� ������� ������.
		else  // ����� �� ����� ����� ����� ���������� ������ �������� � ������ ���� ������ �������� � ����
			initial_buffer[0] = ' ';
	}
	// ���� ���� ��������� �������� ��������, �� ��������� EOF ��� ����������� ����������, �� �������������� �������� ("�� ����������")
	else if (file_not_ended)  // ���� ����� �� ������������ ����������� ������� ������ ���������� ������ ��������, �� ���� ��� �������� � ����� ������
	{
		initial_buffer[letter_index] = symbol_code;
		initial_buffer[letter_index + 1] = '\0';
	}
	// ���� ����� �� ����� ����� �� ���������� ������, �� ����� �� �� ���� ����� ���������� � corrector_edit
	return file_not_ended;
}
