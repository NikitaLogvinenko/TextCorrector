#include "constants.h"
#include "train_module.h"
#include "hash_table_lib.h"
#include "corrector_lib.h"
#include "words_handling.h"
#include "helpful_functions.h"
#include "files_working.h"
#include <stdio.h>
#include <stdlib.h>


// Обучить ИНИЦИАЛИЗИРОВАННЫЙ корректор, используя стартовый конфиг cfg. Возможно последовательное обучение на нескольких текстах
// По окончании обучения сохранить модель (в случае успешного обучения) и очистить память, выделявшуюся под хеш-таблицы
// В случае успешного обучения возвращает EXIT_SUCCESSFULLY, иначе EXIT_FILE_FAILURE (не открылся какой-то из файлов) или EXIT_USER_FAILURE (пользователь неправильно ввёл имя текста)
static int train_initialized(Corrector* corrector, const Pointer* cfg);

// Обучить ИНИЦИАЛИЗИРОВАННЫЙ корректор словам из ОТКРЫТОГО файла text_file. По окончании обучения закрывает файл
// В случае успешного обучения возвращает EXIT_SUCCESSFULLY, иначе EXIT_FILE_FAILURE (не открылся файл)
static int train_one_file(Corrector* corrector, FILE* text_file);

/*=================================================================================================================================================================================*/

int train_new_model(const Pointer* cfg)
{
	int exit_code = EXIT_SUCCESSFULLY;
	printf("\n###Обучение новой модели...\n");
	Corrector new_corrector;
	unsigned max_word_length = *(int*)cfg[3];
	exit_code = corrector_init(&new_corrector, max_word_length, NULL);  // инициализируем корректор, выделяем память под таблицы

	if (exit_code == EXIT_SUCCESSFULLY)  // вся необходимая память для начала работы выделилась
		exit_code = train_initialized(&new_corrector, cfg);  // очистка памяти корректора происходит внутри

	return exit_code;
}

int train_existed_model(const Pointer* cfg)//---------------------------------------------------------------------------------------------
{
	int exit_code = EXIT_SUCCESSFULLY;
	printf("\n###Дообучение существующей модели...\n");
	// Создать модель, открыть файл с моделью, прочитать максимальную длину, инициализировать таблицу, загрузить изученные слова, закрыть файл
	// Вызвать train_inizialized с конфигом cfg, разве что в инициализированной модели уже будут слова


	return exit_code;
}

/*=================================================================================================================================================================================*/

static int train_initialized(Corrector* corrector, const Pointer* cfg)
{
	int exit_code = EXIT_SUCCESSFULLY;
	const char* model_file_name = (const char*)cfg[1], * text_file_name = (const char*)cfg[2];
	FILE* text_file = fopen(text_file_name, "r");
	exit_code = train_one_file(corrector, text_file);  // проверка, что файл открылся, а также закрытие файла происходит внутри
	
	while (exit_code == EXIT_SUCCESSFULLY && yes_no_question("\nТекст изучен. Дообучить модель ещё на одном тексте?\n"))
	{
		printf("Текст с русскими словами должен иметь кодировку ANSI. Для английских текстов допустима UTF-8\n");
		printf("Введите полный или относительный путь к тексту для обучения в формате path\\file_name.txt: ");
		char text_file_name[BUFFER_SIZE];
		read_param_from_console(text_file_name, BUFFER_SIZE);
		if (path_is_txt(text_file_name))  // новый введённый путь - действительно путь к файлу .txt
		{
			FILE* text_file = fopen(text_file_name, "r");
			exit_code = train_one_file(corrector, text_file);
		}
		else
		{
			printf("Неверно введён путь. Обучение прервано\n");
			exit_code = EXIT_USER_FAILURE;
		}
	}
	
	if (exit_code != EXIT_SUCCESSFULLY)  // на каком-то этапе произошла ошибка, не сохраняем модель (если это новая модель - будет пустой файл, если старая, то файл не изменится)
		printf("Сожалеем, на этапе обучения произошла ошибка. Модель останется в том состоянии, в котором была до обучения. Повторите попытку\n");
	else  // сохраняем модель
	{
		FILE* model_file = fopen(model_file_name, "w");  // перезаписываем модель
		exit_code = corrector_save(corrector, model_file);  // проверка открытия и закрытие файла модели происходят внутри
		if (exit_code != EXIT_SUCCESSFULLY)
			printf("Ошибка при сохранении модели. Модель отныне не работает. Обучите новую. Приносим свои извинения!\n");
	}

	corrector_destroy(corrector);  // чистим память, выделенную под таблицы и слова
	printf("\n###Обучение завершено\n");
	return exit_code;
}

static int train_one_file(Corrector* corrector, FILE* text_file)
{
	int exit_code = EXIT_SUCCESSFULLY;
	if (not_null_ptr(text_file, "Не удалось открыть файл для обучения\n"))
	{
		char buffer_for_word[MAX_AVAILABLE_WORD_LENGTH + 1];  // +1, т.к. нужно вставить '\0'
		bool file_not_ended = true;
		char* learning_word = NULL;
		while (file_not_ended)
		{
			file_not_ended = read_train_word(text_file, buffer_for_word, corrector->max_word_length);
			learning_word = prepare_train_word(buffer_for_word);
			if (learning_word != NULL)  // слово подходит для изучения, запоминаем его, устанавливаем/обновляем счётчик
				corrector_learn(corrector, learning_word);
		}
		fclose(text_file);
	}
	else
		exit_code = EXIT_FILE_FAILURE;
	return exit_code;
}