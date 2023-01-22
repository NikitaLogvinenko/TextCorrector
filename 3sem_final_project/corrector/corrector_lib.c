#include "constants.h"
#include "marked_list_lib.h"
#include "hash_table_lib.h"
#include "corrector_lib.h"
#include "words_handling.h"
#include "helpful_functions.h"
#include "files_working.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


// ������� ������������ ���������� ����� ���� ������������ ������ �� ����� model_file � ���������� max_word_length
// � ������ ��������� ��������� ����� EXIT_SUCCESSFULLY
// � ������ ������ ������ (�������� ������ ������) ����� EXIT_USER_FAILURE
// ��������������, ��� ���� ������ ��������� � �������� �������� ����������� �����
static int load_max_word_length(FILE* model_file, int* max_word_length);

// �� ��������� ����� � ���������� ������� ������� ������� �� ������� ������ table_index+1 � ������������������, �� �� ����������� ������ corrector
// � ������ ��������� ��������� ����� EXIT_SUCCESSFULLY
// � ������ ������ ������ (�������� ������ ������) ������� ��, ��� ���� ��������� � ������ �����, � ����� EXIT_USER_FAILURE
// ���� �� ������� �������� ������ ������� � ������������ � ���������� � ����� - ������� ��, ��� ���� ��������� � ������ �����, � ����� EXIT_MEMORY_FAILURE
// ��������������, ��� ���� ������ ��������� � �������� �������� ����������� �����
static int load_table(Corrector* corrector, FILE* model_file, unsigned table_index);

// �� ��������� ����� � ���������� ������� ������� ������ row_index ���-�������. ����� ���� ���� ������ ���� word_length
// ���� �� ������� ������� - ����� EXIT_SUCCESSFULLY
// ���� �� ��������� ������ ��� ����� ����� - ������� ��� ����������� ����� �� ���� ������ � ����� EXIT_MEMORY_FAILURE
// � ������ ������ ������ (�������� ������ ������) ��� ���� ����� ����� �� word_length - ������� ��� ����������� ����� �� ���� ������ � ����� EXIT_USER_FAILURE
static int load_row(HashTable* ht, FILE* model_file, unsigned row_index, unsigned word_length);

// ������� row_length ���� �� ����� model_file � �������� �� � ������� ������ row
// ��������� ��������� �� ���������, ������ ��� ����� ����� ��������� *row �������� �� ������, ����� ������� ����� �������� � �������� ������ ����
// ��� ����� ������ ����� ����� word_length. ����� ������ row_index � ������� ���������, ����� � ������ ������������� �������� �� ������
// word_buffer - ����������� ������ char[] ��� ��������������� ������ ����. ����� �� �������� ����������� ������ ��� ������ �����, ������ ���� ������ �����
// ���������� � ���������� ��� ���������� �������� ����� word_counter
// ���� ��� ����� ������� ��������� - ����� EXIT_SUCCESSFULLY
// ���� ��������� ������ ��� ��������� ������������ ������ ��� ����� ��� ��� ���� �������� ������ - ������ ��� ����� � ������, ����������� �����, � ���������� EXIT_MEMORY_FAILURE
// ���� �� ������ ������� ����� ��� ��� �������, ���� � ����� �� ������ ����� � ������, ���� ����� ������������ ����� ��� ���� ����� ����� ������� <0 - ����� EXIT_USER_FAILURE
static int load_words(MList** row, FILE* model_file, unsigned row_length, unsigned word_length, unsigned row_index, char* word_buffer, int* word_counter);

// �������� �����, ���������� � edited_buffer. ����� ������ �������� ������ �� ��������� ���� � ������� ������ �����.
// � ������� ���������� corrector ��������� ������������� ����� �, ���� ����, ������� ��������������� � ������ size_tol � threshold.
// ���� ����� ��������������� - ���������� true, ����� false.
static bool try_edit(Corrector* corrector, unsigned size_tol, unsigned threshold, char* edited_buffer);

// �������� ����� ������ ������� ��� �������������� ����� �� edited_buffer � ������� ������� ��� �������� table_index, ���������� � corrector.
// ���������� ��������� �� ����� ������ ������� (����� ����, �� ��������� ������).
// ��������������, ��� � ������� ���������� �������, ��������������� size_tol. ������� ������ ������ threshold.
// ������� ��������� ������ ������� best_replace, ��������� �����. ��� ������������� ���������� *best_distance ����������� ����� �� �����������.
// ���� � ������� ���� ������� �����, �� �������� *best_distance � ������������ ��������� �� ����� ������ ������� ������.
// ������� ��������� �����, ���� � ���� ������ ���������� ����� ���������� � ���������� ������. ���� ���������� ����������, �� ����� ������� ���, ������� ���� ���������� ��� ��������.
// ���� �� ������ ������� ����� - ���������� ��� �� ����� best_replace. ���� best_replace == NULL, �� ����� �������, ��������������� threshold, ����� �����.
static MList* single_table_edit(Corrector* corrector, unsigned table_index, const char* edited_buffer, MList*  best_replace, unsigned* best_distance, unsigned threshold);


int corrector_init(Corrector* corrector, unsigned max_word_length, WordsMetric metric_func)
{
	int exit_code = EXIT_SUCCESSFULLY;
	corrector->max_word_length = max_word_length;
	if (metric_func == NULL)
		metric_func = hamming_distance;
	corrector->metric = metric_func;

	corrector->dictionary = (HashTable*)retry_malloc(sizeof(HashTable) * max_word_length, MAX_MALLOC_ATTEMPTS);  // �������� ������ ��� �������
	if (corrector->dictionary == NULL)  // �� ���������� ������ ��� �������
		exit_code = EXIT_MEMORY_FAILURE;
	else
	{
		for (unsigned ht_index = 0; ht_index < max_word_length && exit_code == EXIT_SUCCESSFULLY; ++ht_index)
		{
			HashTable* new_table = corrector->dictionary + ht_index;
			exit_code = ht_init(new_table, INITIAL_TABLE_SIZE, NULL);  // �������������� �������, �������� ������ ��� ������
			if (exit_code != EXIT_SUCCESSFULLY)  // �� ���������� ������ ��� ������ � �������
			{
				for (unsigned already_allocated = 0; already_allocated < ht_index; ++already_allocated)
					ht_destroy(corrector->dictionary + already_allocated);  // ������ �� ������, ������� ��� �������� ��� ������ ������
				free(corrector->dictionary);  // ������ ������, ���������� ��� �������
			}
		}
	}

	return exit_code;
}

void corrector_destroy(Corrector* corrector)
{
	if (corrector != NULL && corrector->dictionary != NULL)
	{
		for (unsigned table_index = 0; table_index < corrector->max_word_length; ++table_index)
			ht_destroy(corrector->dictionary + table_index);
		free(corrector->dictionary);
		corrector->max_word_length = 0;
	}
}

int corrector_save(Corrector* corrector, const char* model_file_name)
{
	int exit_code = EXIT_SUCCESSFULLY;
	FILE* model_file = NULL;
	if (path_is_txt(model_file_name))  // ����� �������� ���� - ������������� ���� � ����� .txt
		model_file = fopen(model_file_name, "w");  // ���������� ����� ������ ��� �������������� ������
	else
	{
		printf("������� ����� ���� � ������. �������� ��������\n");
		exit_code = EXIT_USER_FAILURE;
	}
	
	if (exit_code == EXIT_SUCCESSFULLY && not_null_ptr(model_file, "�� ������� ������� ���� ��� ����������. ���������� ��� ��� ��� �������� ���� � �����\n"))  // ������� ����
	{
		if (corrector != NULL)
		{
			fprintf(model_file, "max_word_length=%d\n", corrector->max_word_length);
			for (unsigned table_index = 0; table_index < corrector->max_word_length; ++table_index)
			{
				HashTable* current_table = corrector->dictionary + table_index;
				char table_name[LONGEST_TABLE_NAME];
				sprintf(table_name, "table_%u", table_index + 1);
				fprint_ht(model_file, current_table, table_name);
			}
		}
		fclose(model_file);
	}
	else  // ���� �� ��������
		exit_code = EXIT_FILE_FAILURE;
	return exit_code;
}

int corrector_learn(Corrector* corrector, const char* word)
{
	int exit_code = EXIT_SUCCESSFULLY;
	if (corrector != NULL && word != NULL)
	{
		unsigned word_length = strlen(word);
		if (word_length <= corrector->max_word_length)
		{
			HashTable* same_length_words = corrector->dictionary + word_length - 1;
			exit_code = ht_set(same_length_words, word, 1);
		}
		else
			exit_code = EXIT_TOO_LONG;
	}
	else
		exit_code = EXIT_USER_FAILURE;
	return exit_code;
}

int corrector_load(Corrector* corrector, const char* model_file_name)
{
	int exit_code = EXIT_SUCCESSFULLY;
	if (!not_null_ptr(corrector, "������� ������� ��������� �� ��������� ��� �������� ������ �� �����\n") || model_file_name == NULL || !path_is_txt(model_file_name))
	{
		printf("�� ������� ������ �������� ������ �� �����. ��������� ������������ ��������� ���� � ����� � ��������� �������\n");
		exit_code = EXIT_USER_FAILURE;
	}
	else  // �������� �������� ���������, ��������� ����
	{
		FILE* model_file = fopen(model_file_name, "r");
		if (not_null_ptr(model_file, "�� ������� ������� ���� ��� �������� ������. ���������� ��� ��� ��� �������� ���� � �����\n"))  // ���� ��������
		{
			int max_word_length;
			exit_code = load_max_word_length(model_file, &max_word_length);
			if (exit_code == EXIT_SUCCESSFULLY)  // ����� ��������� � ��� � ���������� ��������� ��������
			{
				exit_code = corrector_init(corrector, max_word_length, NULL);
				if (exit_code == EXIT_MEMORY_FAILURE)
					LOAD_MEMORY_ERROR
				for (unsigned table_index = 0; exit_code == EXIT_SUCCESSFULLY && table_index < max_word_length; ++table_index)
					exit_code = load_table(corrector, model_file, table_index);  // ��������� ������� �� �����, ���� �����-�� �� ��������� - ������� ���� ������ ������ �������
			}
			
			else if (exit_code == EXIT_USER_FAILURE)
				NOT_MODEL_FILE_MSG  // �� ������� ������� ������������ ����� ����

			fclose(model_file);
		}
		else  // ���� �� ��������
			exit_code = EXIT_FILE_FAILURE;
	}
	return exit_code;
}

int corrector_edit(Corrector* corrector, const char* initial_buffer, char* edited_buffer, FILE* edited_file, unsigned size_tol, unsigned threshold, unsigned* words_edited_counter)
{
	int last_index = strlen(initial_buffer) - 1;  // ������ ���������� ������� � ������
	int last_symbol = initial_buffer[last_index];  // ��� ���������� ������� � ������
	if (letters_in_word(initial_buffer) == 1 || is_upper_letter(initial_buffer[0]) && capital_counter(initial_buffer) > 1)  // ����� ���� ����� ��� ������������ ����������/������������
		fputs(initial_buffer, edited_file);
	else  // �����, ��������, ���� �������������
	{
		word_to_lower(initial_buffer, edited_buffer);  // ��������� ����� � ����� ��� �������������� � ��������� �� � ������ �������
		int remainder_start = erase_not_letter_remainder(edited_buffer);  // � ������������� ����� ����������� �� ������� � ������������ ����� � �����; �������� ������ ������ �������
		bool edited = try_edit(corrector, size_tol, threshold, edited_buffer);  // ����������� �����, ���� ����
		if (edited || is_lower_letter(initial_buffer[0]) && capital_counter(initial_buffer) > 1)
			*words_edited_counter += 1;  // ���� ����������������� ��� �������� ��������� ����� �� ��������� - ����������� �������
		if (is_upper_letter(initial_buffer[0]))  // ���� ������ ����� ���� ��������� - ���� � � ����������������� ����� ������� � ���������
			edited_buffer[0] = letter_to_upper(edited_buffer[0]);
		fputs(edited_buffer, edited_file);  // ���������� ����������������� �����
		fputs(initial_buffer + remainder_start, edited_file);  // ���������� ������� �� ������� �, ��������, ��� ������ �������; ���� ����� ��� - �� ������ ������� "\0", �.�. ������
		if (initial_buffer[remainder_start] == '\0')
		{
			// ���� ��� ��������� ����� � �����, �� ����� ���� ��� ������� ����������� ��������.
			// ��� �������� ����� ��������� ����� ������������������ �����, ������� �� ������ � ����, ���� ��� ��� ������ � �� ������������
			int last_index_after_edit = strlen(edited_buffer) - 1;
			last_symbol = edited_buffer[last_index_after_edit];
		}
	}
	return last_symbol;
}


static int load_max_word_length(FILE* model_file, int* max_word_length)
{
	bool mwl_exists = (fscanf(model_file, "max_word_length=%d\n", max_word_length) == 1);  // ��������� �� ����� ������������ ����� ��������� ����
	int exit_code = EXIT_SUCCESSFULLY;
	if (mwl_exists == false)  // �� ������� ������� �����, ���� ������� � �������� �������
	{
		exit_code = EXIT_USER_FAILURE;
		printf("�������� ������ ������ ������������ ����� ���� ������\n");
	}
	else if (*max_word_length <= 0 || *max_word_length > MAX_AVAILABLE_WORD_LENGTH)  // ���������� ���������� ����� �����
	{
		exit_code = EXIT_USER_FAILURE;
		printf("������������ �������� ������������ ����� ���� ������\n");
	}
	return exit_code;
}

static int load_table(Corrector* corrector, FILE* model_file, unsigned table_index)
{
	int exit_code = EXIT_SUCCESSFULLY;
	int word_length, table_size, table_total_words;
	bool correct_header = (fscanf(model_file, "<table_%d> size=%d total_words=%d\n", &word_length, &table_size, &table_total_words) == 3);  // ��������� ����� �������, ����� ����� � ����
	if (correct_header == false || word_length != table_index + 1 || table_size < 1 || table_total_words < 0)  // ������������ ��������� �������, ������ ����� ����������
	{
		exit_code = EXIT_USER_FAILURE;
		printf("�������� ������ ��������� ������� %d\n", table_index + 1);
	}
	else  // ������� ������� ��������� �������, ��������� ��������� � ��� �����
	{
		HashTable* current_table = corrector->dictionary + table_index;  // ��������� ������� �� ������� ����������� �����
		exit_code = ht_resize(current_table, table_size);  // ���������� ������ ������� � ������������ �����; ���� �� ��������� ������ ��� ������� - ������� EXIT_MEMORY_FAILURE
		if (exit_code == EXIT_SUCCESSFULLY)
		{
			current_table->total_words = table_total_words;
			for (unsigned row_index = 0; row_index < table_size && exit_code == EXIT_SUCCESSFULLY; ++row_index)  // ��������� ����� � ������ �������
				exit_code = load_row(current_table, model_file, row_index, word_length);
		}
	}

	if (exit_code != EXIT_SUCCESSFULLY)  // ������� �� �������
	{
		corrector_destroy(corrector);  // �� ������� ������� ������� => ������� ��, ��� ��������� ����� � ��������� ���������� ��������
		if (exit_code == EXIT_USER_FAILURE)
			NOT_MODEL_FILE_MSG
		else if (exit_code == EXIT_MEMORY_FAILURE)
			LOAD_MEMORY_ERROR
	}

	return exit_code;
}

static int load_row(HashTable* ht, FILE* model_file, unsigned row_index, unsigned word_length)
{
	int exit_code = EXIT_SUCCESSFULLY;
	MList* current_row = NULL;  // ����� ��������� ���� ������� ������ � ����� ������ ������� ��� � ������ ����� � �������
	int written_index, row_length;
	bool correct_header = (fscanf(model_file, "[row_%d] length=%d\n", &written_index, &row_length) == 2);  // ��������� ��������� ������ (������ � ���������� ����)
	if (correct_header == false || written_index != row_index || row_length < 0)  // ������������ ��������� ������, ������ ���� ����������
	{
		exit_code = EXIT_USER_FAILURE;
		printf("�������� ������ ��������� ������ � �������� %d ������� %d\n", row_index, word_length);
	}
	
	else  // ������� ������� ��������� ������, ������ ������� ������ �� � ���� � ��������� ������ � ������ ������� � �������
	{
		char word_buffer[MAX_AVAILABLE_WORD_LENGTH + 1];  // +1, �.�. ��� ���� �������� '\0'
		int word_counter;
		exit_code = load_words(&current_row, model_file, row_length, word_length, row_index, word_buffer, &word_counter);

	}
	if (exit_code != EXIT_SUCCESSFULLY)  // �� ������� ������� �����-�� �����; ����� � ������� ��������� �� ������������ total_words => ������ ���������� ������ � �������� �� ������
	{
		mlist_destroy(current_row);
		current_row = NULL;
	}
	ht->table[row_index] = current_row;
	return exit_code;
}

static int load_words(MList** row, FILE* model_file, unsigned row_length, unsigned word_length, unsigned row_index, char* word_buffer, int* word_counter)
{
	int exit_code = EXIT_SUCCESSFULLY;
	for (unsigned word_index = 0; word_index < row_length && exit_code == EXIT_SUCCESSFULLY; ++word_index)
	{
		bool correct_word = (fscanf(model_file, "%s %d\n", word_buffer, word_counter) == 2);  // ������� ����� � ��� �������
		if (correct_word == false || wrong_symbols_in_word(word_buffer) != 0 || letters_and_hyphens_in_word(word_buffer) != word_length || *word_counter < 1)
		{
			// ����� �� ���������, ����� ������������ �����, ������� �� ������ �� ���� � ������� ��� ������� ����� ������ ����
			printf("�������� ������ ����� � ������� %d � ������ row_%d\n", word_length, row_index);
			exit_code = EXIT_USER_FAILURE;
		}
		else  // �� ��������� ���������, ���������� �����
		{
			char* dynamic_word = (char*)retry_malloc(sizeof(char) * (word_length + 1), MAX_MALLOC_ATTEMPTS);  // �������� ������ ��� �������� ������ �����
			if (not_null_ptr(dynamic_word, "�� ������� �������� ������ ��� �����. �������� ������ ����������.\n�������� ���� ���������!\n��������� �������\n"))
			{
				strcpy(dynamic_word, word_buffer);  // �������� ����� �� ���������� ������ � ������������ ������
				*row = mlist_add(*row, dynamic_word, *word_counter);  // ��������� �����
				exit_code = NEW_WORD_ADDED;  // ���������, ��� ����� ���� �� ������, ���� ����������, ������
			}
			else
				exit_code = EXIT_MEMORY_FAILURE;  // �� ���������� ������ ��� dynamic_word
		}
	}
	return exit_code;
}

static bool try_edit(Corrector* corrector, unsigned size_tol, unsigned threshold, char* edited_buffer)
{
	bool edited = false, exists = false;
	int initial_length = strlen(edited_buffer);
	int min_length = max(2, initial_length - size_tol);  // ���������� ���������� ����� ����������� �����
	int max_length = min(corrector->max_word_length, initial_length + size_tol);  // ����������� ���������� ����� ����������� �����
	if (initial_length <= corrector->max_word_length)  // ���� ����� �� ��������� ������������ ����� ���������, �� �������� ��� �����
	{
		HashTable* same_length_table = corrector->dictionary + initial_length - 1;  // �������, � ������� ����� ���� ���� ����� (� ����� ������� ����� ����� � ��� �� �����)
		exists = ht_has(same_length_table, edited_buffer);
	}
	if (exists == false)  // ���� ����� �� ����������, �� ������� ��� ���������������
	{
		MList* best_replace = NULL;
		unsigned best_distance;  // ����� �� ������������� � ������ �������
		for (unsigned table_index = min_length - 1; table_index < max_length; ++table_index)
			best_replace = single_table_edit(corrector, table_index, edited_buffer, best_replace, &best_distance, threshold);
		if (best_replace != NULL)  // ����� ������� ��� ������
		{
			const char* best_word = best_replace->word;
			char ask_assent[BUFFER_SIZE];
			sprintf(ask_assent, "�������� [%s] �� [%s]?\n", edited_buffer, best_word);
			bool user_assent = yes_no_question(ask_assent);
			if (user_assent)
			{
				strcpy(edited_buffer, best_word);
				edited = true;
			}
		}
	}
	return edited;
}

static MList* single_table_edit(Corrector* corrector, unsigned table_index, const char* edited_buffer, MList* best_replace, unsigned* best_distance, unsigned threshold)
{
	HashTable* ht = corrector->dictionary + table_index;  // ����� ����� table_index+1
	int current_distance;
	if (ht != NULL)  // ���� � ������ ���� ����� ����� ����� (����� �������)
	{
		for (unsigned row_index = 0; row_index < ht->size; ++row_index)  // ���� ������ ������� � ������ ������
		{
			MList* word_in_row = ht->table[row_index];
			while (word_in_row != NULL)
			{
				current_distance = corrector->metric(edited_buffer, word_in_row->word);
				if (current_distance <= threshold)
				{
					if ((best_replace == NULL) || (*best_distance > current_distance) || (*best_distance == current_distance && word_in_row->counter > best_replace->counter))
					{
						best_replace = word_in_row;
						*best_distance = current_distance;
					}
				}
				word_in_row = word_in_row->next;
			}
		}
	}
	return best_replace;
}