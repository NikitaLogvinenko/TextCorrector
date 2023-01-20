#include "constants.h"
#include "hash_table_lib.h"
#include "CorrectorModel.h"
#include "corrector_lib.h"
#include "words_handling.h"
#include "helpful_functions.h"
#include "files_working.h"
#include "marked_list_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


// Считать максимальную допустимую длину слов существующей модели из файла model_file в переменную max_word_length
// В случае успешного прочтения вернёт EXIT_SUCCESSFULLY
// В случае ошибки чтения (неверный формат записи) вернёт EXIT_USER_FAILURE
// Предполагается, что файл открыт правильно и проверка открытия произведена ранее
static int load_max_word_length(FILE* model_file, int* max_word_length);

// Из открытого файла с сохранённой моделью считать таблицу со словами длиной table_index+1 в инициализированную, но не заполненную модель corrector
// В случае успешного прочтения вернёт EXIT_SUCCESSFULLY
// В случае ошибки чтения (неверный формат записи) очистит всё, что было загружено в модель ранее, и вернёт EXIT_USER_FAILURE
// Если не удалось изменить размер таблицы в соответствии с записанным в файле - очистит всё, что было загружено в модель ранее, и вернёт EXIT_MEMORY_FAILURE
// Предполагается, что файл открыт правильно и проверка открытия произведена ранее
static int load_table(Corrector* corrector, FILE* model_file, unsigned table_index);

// Из открытого файла с сохранённой моделью считать строку row_index хеш-таблицы. Длина всех слов должна быть word_length
// Если всё успешно считано - вернёт EXIT_SUCCESSFULLY
// Если не выделится память под новое слово - очистит все прочитанные слова из этой строки и вернёт EXIT_MEMORY_FAILURE
// В случае ошибки чтения (неверный формат записи) или если длина слова не word_length - очистит все прочитанные слова из этой строки и вернёт EXIT_USER_FAILURE
static int load_row(HashTable* ht, FILE* model_file, unsigned row_index, unsigned word_length);

// Считать row_length слов из файла model_file и записать их в связный список row
// Передаётся указатель на указатель, потому что нужно чтобы указатель *row указывал на голову, адрес которой будет меняться в процессе чтения слов
// Все слова должны иметь длину word_length. Номер строки row_index в таблице передаётся, чтобы в случае необходимости сообщить об ошибке
// word_buffer - статический массив char[] для первоначального чтения слов. Чтобы не выделять статический массив под каждое слово, создаём один массив извне
// Аналогично с переменной для считывания счётчика слова word_counter
// Если все слова успешно прочитаны - вернёт EXIT_SUCCESSFULLY
// Если произойдёт ошибка при выделении динамической памяти под слово или под узел связного списка - чистит все слова в строке, прочитанные ранее, и возвращает EXIT_MEMORY_FAILURE
// Если не удаётся считать слово или его счётчик, если в слове не только буквы и дефисы, если слово неправильной длины или если слово имеет счётчик <0 - вернёт EXIT_USER_FAILURE
static int load_words(MList** row, FILE* model_file, unsigned row_length, unsigned word_length, unsigned row_index, char* word_buffer, int* word_counter);


int corrector_init(Corrector* corrector, unsigned max_word_length, WordsMetric metric_func)
{
	int exit_code = EXIT_SUCCESSFULLY;
	corrector->max_word_length = max_word_length;
	if (metric_func == NULL)
		metric_func = hamming_distance;
	corrector->metric = metric_func;

	corrector->dictionary = (HashTable*)retry_malloc(sizeof(HashTable) * max_word_length, MAX_MALLOC_ATTEMPTS);  // выделяем память под таблицы
	if (corrector->dictionary == NULL)  // не выделилась память под таблицы
		exit_code = EXIT_MEMORY_FAILURE;
	else
	{
		for (unsigned ht_index = 0; ht_index < max_word_length && exit_code == EXIT_SUCCESSFULLY; ++ht_index)
		{
			HashTable* new_table = corrector->dictionary + ht_index;
			exit_code = ht_init(new_table, INITIAL_TABLE_SIZE, NULL);  // инициализируем таблицы, выделяем память под строки
			if (exit_code != EXIT_SUCCESSFULLY)  // не выделилась память под строки в таблице
			{
				for (unsigned already_allocated = 0; already_allocated < ht_index; ++already_allocated)
					ht_destroy(corrector->dictionary + already_allocated);  // чистим ту память, которую уже выделили под строки таблиц
				free(corrector->dictionary);  // чистим память, выделенную под таблицы
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
	if (path_is_txt(model_file_name))  // новый введённый путь - действительно путь к файлу .txt
		model_file = fopen(model_file_name, "w");  // записываем новую модель или перезаписываем старую
	else
	{
		printf("Неверно введён путь к модели. Обучение прервано\n");
		exit_code = EXIT_USER_FAILURE;
	}
	
	if (exit_code == EXIT_SUCCESSFULLY && not_null_ptr(model_file, "Не удалось открыть файл для сохранения. Попробуйте ещё раз или измените путь к файлу\n"))  // открыли файл
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
	else  // файл не открылся
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
	if (!not_null_ptr(corrector, "Передан нулевой указатель на корректор при загрузке модели из файла\n") || model_file_name == NULL || !path_is_txt(model_file_name))
	{
		printf("Не удалось начать загрузку модели из файла. Проверьте корректность введённого пути к файлу и повторите попытку\n");
		exit_code = EXIT_USER_FAILURE;
	}
	else  // переданы валидные указатели, открываем файл
	{
		FILE* model_file = fopen(model_file_name, "r");
		if (not_null_ptr(model_file, "Не удалось открыть файл для загрузки модели. Попробуйте ещё раз или измените путь к файлу\n"))  // файл открылся
		{
			int max_word_length;
			exit_code = load_max_word_length(model_file, &max_word_length);
			if (exit_code == EXIT_SUCCESSFULLY)  // длина считалась и она в допустимом диапазоне значений
			{
				exit_code = corrector_init(corrector, max_word_length, NULL);
				if (exit_code == EXIT_MEMORY_FAILURE)
					LOAD_MEMORY_ERROR
				for (unsigned table_index = 0; exit_code == EXIT_SUCCESSFULLY && table_index < max_word_length; ++table_index)
					exit_code = load_table(corrector, model_file, table_index);  // считываем таблицы по одной, если какая-то не считается - очистка всей модели внутри функции
			}
			
			else if (exit_code == EXIT_USER_FAILURE)
				NOT_MODEL_FILE_MSG  // не удалось считать максимальную длину слов

			fclose(model_file);
		}
		else  // файл не открылся
			exit_code = EXIT_FILE_FAILURE;
	}
	return exit_code;
}


static int load_max_word_length(FILE* model_file, int* max_word_length)
{
	bool mwl_exists = (fscanf(model_file, "max_word_length=%d\n", max_word_length) == 1);  // считываем из файла максимальную длину изучаемый слов
	int exit_code = EXIT_SUCCESSFULLY;
	if (mwl_exists == false)  // не удалось считать длину, файл записан в неверном формате
	{
		exit_code = EXIT_USER_FAILURE;
		printf("Неверный формат записи максимальной длины слов модели\n");
	}
	else if (*max_word_length <= 0 || *max_word_length > MAX_AVAILABLE_WORD_LENGTH)  // превышение допустимой длины слова
	{
		exit_code = EXIT_USER_FAILURE;
		printf("Недопустимое значение максимальной длины слов модели\n");
	}
	return exit_code;
}

static int load_table(Corrector* corrector, FILE* model_file, unsigned table_index)
{
	int exit_code = EXIT_SUCCESSFULLY;
	int word_length, table_size, table_total_words;
	bool correct_header = (fscanf(model_file, "<table_%d> size=%d total_words=%d\n", &word_length, &table_size, &table_total_words) == 3);  // считываем номер таблицы, число строк и слов
	if (correct_header == false || word_length != table_index + 1 || table_size < 1 || table_total_words < 0)  // неправильный заголовок таблицы, чтение строк невозможно
	{
		exit_code = EXIT_USER_FAILURE;
		printf("Неверный формат заголовка таблицы %d\n", table_index + 1);
	}
	else  // успешно считали параметры таблицы, построчно добавляем в нее слова
	{
		HashTable* current_table = corrector->dictionary + table_index;  // заполняем таблицу со словами определённой длины
		exit_code = ht_resize(current_table, table_size);  // изменяется размер таблицы и записывается новый; если не выделится память под таблицу - вернётся EXIT_MEMORY_FAILURE
		if (exit_code == EXIT_SUCCESSFULLY)
		{
			current_table->total_words = table_total_words;
			for (unsigned row_index = 0; row_index < table_size && exit_code == EXIT_SUCCESSFULLY; ++row_index)  // считываем слова в строки таблицы
				exit_code = load_row(current_table, model_file, row_index, word_length);
		}
	}

	if (exit_code != EXIT_SUCCESSFULLY)  // таблица не считана
	{
		corrector_destroy(corrector);  // не удалось считать таблицу => очищаем то, что загрузили ранее и завершаем выполнение загрузки
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
	MList* current_row = NULL;  // будем заполнять этот связный список и потом просто вставим его в нужное место в таблицу
	int written_index, row_length;
	bool correct_header = (fscanf(model_file, "[row_%d] length=%d\n", &written_index, &row_length) == 2);  // считываем заголовок строки (индекс и количество слов)
	if (correct_header == false || written_index != row_index || row_length < 0)  // неправильный заголовок строки, чтение слов невозможно
	{
		exit_code = EXIT_USER_FAILURE;
		printf("Неверный формат заголовка строки с индексом %d таблицы %d\n", row_index, word_length);
	}
	
	else  // успешно считали параметры строки, строим связный список из её слов и добавляем список в нужную позицию в таблице
	{
		char word_buffer[MAX_AVAILABLE_WORD_LENGTH + 1];  // +1, т.к. ещё надо записать '\0'
		int word_counter;
		exit_code = load_words(&current_row, model_file, row_length, word_length, row_index, word_buffer, &word_counter);

	}
	if (exit_code != EXIT_SUCCESSFULLY)  // не удалось считать какое-то слово; тогда в таблице хранилось бы неправильное total_words => просто прекращаем чтение и сообщаем об ошибке
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
		bool correct_word = (fscanf(model_file, "%s %d\n", word_buffer, word_counter) == 2);  // считали слово и его счётчик
		if (correct_word == false || wrong_symbols_in_word(word_buffer) != 0 || letters_and_hyphens_in_word(word_buffer) != word_length || *word_counter < 1)
		{
			// слово не считалось, имеет неправильную длину, состоит не только из букв и дефисов или счётчик слова меньше нуля
			printf("Неверная запись слова в таблице %d в строке row_%d\n", word_length, row_index);
			exit_code = EXIT_USER_FAILURE;
		}
		else  // всё корректно считалось, запоминаем слово
		{
			char* dynamic_word = (char*)retry_malloc(sizeof(char) * (word_length + 1), MAX_MALLOC_ATTEMPTS);  // выделяем память под хранение нового слова
			if (not_null_ptr(dynamic_word, "Не удалось выделить память под слово. Загрузка модели прекращена.\nПриносим свои извинения!\nПовторите попытку\n"))
			{
				strcpy(dynamic_word, word_buffer);  // копируем слово из временного буфера в динамическую память
				*row = mlist_add(*row, dynamic_word, *word_counter);  // сохраняем слово
				exit_code = NEW_WORD_ADDED;  // проверяем, что новый узел со словом, если необходимо, создан
			}
			else
				exit_code = EXIT_MEMORY_FAILURE;  // не выделилась память под dynamic_word
		}
	}
	return exit_code;
}