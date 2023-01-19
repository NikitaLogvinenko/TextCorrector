#include "constants.h"
#include "hash_table_lib.h"
#include "CorrectorModel.h"
#include "corrector_lib.h"
#include "words_handling.h"
#include "helpful_functions.h"
#include <stdio.h>
#include <stdlib.h>


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
	for (unsigned table_index = 0; table_index < corrector->max_word_length; ++table_index)
		ht_destroy(corrector->dictionary + table_index);
	free(corrector->dictionary);
}