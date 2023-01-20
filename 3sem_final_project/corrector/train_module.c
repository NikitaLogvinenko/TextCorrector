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

// Обучить ИНИЦИАЛИЗИРОВАННЫЙ корректор словам из файла text_file_name. По окончании обучения закрывает файл
// В случае успешного обучения возвращает EXIT_SUCCESSFULLY
// Если text_file_name не является путём к файлу .txt - вернёт EXIT_USER_FAILURE
// Если не открылся файл - вернёт EXIT_FILE_FAILURE
static int train_one_file(Corrector* corrector, const char* text_file_name);

/*=================================================================================================================================================================================*/

int train_new_model(const Pointer* cfg)
{
	int exit_code = EXIT_SUCCESSFULLY;
	printf("\n### Обучение новой модели...\n");
	Corrector new_corrector;
	unsigned max_word_length = *(int*)cfg[3];
	exit_code = corrector_init(&new_corrector, max_word_length, NULL);  // инициализируем корректор, выделяем память под таблицы

	if (exit_code == EXIT_SUCCESSFULLY)  // вся необходимая память для начала работы выделилась
		exit_code = train_initialized(&new_corrector, cfg);  // очистка памяти корректора происходит внутри

	return exit_code;
}

int train_existed_model(const Pointer* cfg)
{
	int exit_code = EXIT_SUCCESSFULLY;
	printf("\n### Загрузка существующей модели...\n");
	Corrector existed_corrector;
	const char* model_file_name = (const char*)cfg[1];
	exit_code = corrector_load(&existed_corrector, model_file_name);  // загружаем существующую модель из файла; если не сможем загрузить, то вся прочитанная память очищается внутри
	if (exit_code == EXIT_SUCCESSFULLY)
	{
		printf("\n### Загрузка успешно завершена\n### Дообучение существующей модели...\n");
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
	
	while (exit_code == EXIT_SUCCESSFULLY && yes_no_question("\nТекст изучен. Дообучить модель ещё на одном тексте?\n"))
	{
		printf("Текст с русскими словами должен иметь кодировку ANSI. Для английских текстов допустима UTF-8\n");
		printf("Введите полный или относительный путь к тексту для обучения в формате path\\file_name.txt: ");
		char text_file_name[BUFFER_SIZE];
		read_param_from_console(text_file_name, BUFFER_SIZE);
		exit_code = train_one_file(corrector, text_file_name);
	}
	
	if (exit_code != EXIT_SUCCESSFULLY)  // на каком-то этапе произошла ошибка, не сохраняем модель (если это новая модель - будет пустой файл, если старая, то файл не изменится)
		printf("Сожалеем, на этапе обучения произошла ошибка. Модель останется в том состоянии, в котором была до обучения. Повторите попытку\n");
	else  // сохраняем модель
	{
		printf("\n### Обучение завершено\n### Сохранение модели в файл...\n");
		exit_code = corrector_save(corrector, model_file_name);
		if (exit_code != EXIT_SUCCESSFULLY)
			printf("Ошибка при сохранении модели. Модель больше не работает. Обучите новую. Приносим свои извинения!\n");
		else
			printf("\n### Модель успешно сохранена!\n");
	}
	corrector_destroy(corrector);  // чистим память, выделенную под таблицы и слова
	return exit_code;
}

static int train_one_file(Corrector* corrector, const char* text_file_name)
{
	int exit_code = EXIT_SUCCESSFULLY;
	FILE* text_file = NULL;
	if (path_is_txt(text_file_name))  // новый введённый путь - действительно путь к файлу .txt
		text_file = fopen(text_file_name, "r");
	else
	{
		printf("Неверно введён путь к обучающему тексту. Обучение прервано\n");
		exit_code = EXIT_USER_FAILURE;
	}

	if (exit_code == EXIT_SUCCESSFULLY && not_null_ptr(text_file, "Не удалось открыть файл для обучения\n"))  // открыли файл
	{
		char buffer_for_word[MAX_AVAILABLE_WORD_LENGTH + 1];  // буфер, чтобы считать слово из текста; +1, т.к. нужно вставить '\0' в конце
		bool file_not_ended = true;
		char* learning_word = NULL;
		int memory_failures = 0;  // счётчик, сколько слов не смогли вставить
		while (file_not_ended)
		{
			file_not_ended = read_train_word(text_file, buffer_for_word, corrector->max_word_length);
			learning_word = prepare_train_word(buffer_for_word);
			if (learning_word != NULL)  // слово подходит для изучения, запоминаем его, устанавливаем/обновляем счётчик
				memory_failures += (corrector_learn(corrector, learning_word) == EXIT_MEMORY_FAILURE) ? 1 : 0;
		}
		if (memory_failures != 0)  // выучены не все слова, модель будет работать, но не на всём наборе слов
			printf("Не удалось выделить память для %d слов. Модель будет работать на остальных словах, но рекомендуем Вам повторить обучение\n", memory_failures);
		fclose(text_file);
	}
	else  // файл не открылся
		exit_code = EXIT_FILE_FAILURE;
	return exit_code;
}