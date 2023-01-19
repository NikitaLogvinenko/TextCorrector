#include "constants.h"
#include "train_module.h"
#include "hash_table_lib.h"
#include "corrector_lib.h"
#include "words_handling.h"
#include "helpful_functions.h"
#include <stdio.h>
#include <stdlib.h>


/*=================================================================================================================================================================================*/

int train_new_model(const Pointer* cfg)//---------------------------------------------------------------------------------------------
{
	int exit_code = EXIT_SUCCESSFULLY;
	printf("\n###Обучение новой модели...\n");
	Corrector new_corrector;
	unsigned max_word_length = *(int*)cfg[3];
	exit_code = corrector_init(&new_corrector, max_word_length, NULL);  // инициализируем корректор, выделяем память под таблицы
	
	if (exit_code == EXIT_SUCCESSFULLY)  // вся необходимая память для начала работы выделилась
	{
		const char* model_file_name = (const char*)cfg[1], *text_file_name = (const char*)cfg[2];
		printf("[%s]\n", text_file_name);
		FILE* text_file = fopen(text_file_name, 'r');
		printf("*\n");
		int c = fgetc(text_file);
		while (c != EOF)
		{
			printf("%c", (char)c);
			c = fgetc(text_file);
		}
		fclose(text_file);
	}

	printf("\n###Обучение завершено\n");
	corrector_destroy(&new_corrector);  // чистим память корректора
	return exit_code;
}

int train_existed_model(const Pointer* cfg)//---------------------------------------------------------------------------------------------
{
	int exit_code = EXIT_SUCCESSFULLY;
	printf("\n###Дообучение существующей модели...\n");


	printf("\n###Обучение завершено\n");
	return exit_code;
}

/*=================================================================================================================================================================================*/
