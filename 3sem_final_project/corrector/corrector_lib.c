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

// Получает слово, записанное в edited_buffer. Слово должно состоять только из МАЛЕНЬКИХ букв и дефисов ВНУТРИ слова.
// С помощью корректора corrector проверяет существование слова и, если надо, пробует отредактировать с учётом size_tol и threshold.
// Если слово отредактировано - возвращает true, иначе false.
static bool try_edit(Corrector* corrector, unsigned size_tol, unsigned threshold, char* edited_buffer);

// Пытается найти лучший вариант для редактирования слова из edited_buffer с помощью таблицы под индексом table_index, хранящейся в corrector.
// Возвращает указатель на новый лучщий вариант (может быть, он останется старым).
// Предполагается, что в функцию передаются индексы, удовлетворяющие size_tol. Остаётся только учесть threshold.
// Функция принимает лучший вариант best_replace, найденный ранее. Ему соответствует отклонение *best_distance заменяющего слова от заменяемого.
// Если в таблице есть вариант лучше, то меняется *best_distance и возвращается указатель на новый лучший вариант замены.
// Вариант считается лучше, если у него меньше расстояние между заменяемым и заменяющим словом. Если расстояние одинаковое, то лучше вариант тот, который чаще встречался при обучении.
// Если не найден вариант лучше - возвращает тот же самый best_replace. Если best_replace == NULL, то любой вариант, удовлетворяющий threshold, будет лучше.
static MList* single_table_edit(Corrector* corrector, unsigned table_index, const char* edited_buffer, MList*  best_replace, unsigned* best_distance, unsigned threshold);


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

int corrector_edit(Corrector* corrector, const char* initial_buffer, char* edited_buffer, FILE* edited_file, unsigned size_tol, unsigned threshold, unsigned* words_edited_counter)
{
	int last_index = strlen(initial_buffer) - 1;  // индекс последнего символа в буфере
	int last_symbol = initial_buffer[last_index];  // код последнего символа в буфере
	if (letters_in_word(initial_buffer) == 1 || is_upper_letter(initial_buffer[0]) && capital_counter(initial_buffer) > 1)  // всего одна буква или используется сокращение/аббревиатура
		fputs(initial_buffer, edited_file);
	else  // слово, возможно, надо редактировать
	{
		word_to_lower(initial_buffer, edited_buffer);  // переносим слово в буфер для редактирования и переводим всё в нижний регистр
		int remainder_start = erase_not_letter_remainder(edited_buffer);  // в редактируемом слове избавляемся от дефисов и небуквенного знака в конце; получаем индекс начала остатка
		bool edited = try_edit(corrector, size_tol, threshold, edited_buffer);  // редактируем слово, если надо
		if (edited || is_lower_letter(initial_buffer[0]) && capital_counter(initial_buffer) > 1)
			*words_edited_counter += 1;  // если отредактировалось или заменили заглавные буквы на маленькие - увеличиваем счётчик
		if (is_upper_letter(initial_buffer[0]))  // если первая буква была заглавной - надо и в отредактированном слове сделать её заглавной
			edited_buffer[0] = letter_to_upper(edited_buffer[0]);
		fputs(edited_buffer, edited_file);  // выписываем отредактированное слово
		fputs(initial_buffer + remainder_start, edited_file);  // выписываем остаток из дефисов и, возможно, ещё одного символа; если таких нет - то просто выпишем "\0", т.е. ничего
		if (initial_buffer[remainder_start] == '\0')
		{
			// Если это последнее слово в файле, то после него нет никаких небуквенных символов.
			// Для приличия вернём последнюю букву отредактированного слова, которую мы вывели в файл, хоть это уже дальше и не понадобиться
			int last_index_after_edit = strlen(edited_buffer) - 1;
			last_symbol = edited_buffer[last_index_after_edit];
		}
	}
	return last_symbol;
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

static bool try_edit(Corrector* corrector, unsigned size_tol, unsigned threshold, char* edited_buffer)
{
	bool edited = false, exists = false;
	int initial_length = strlen(edited_buffer);
	int min_length = max(2, initial_length - size_tol);  // минимально допустимая длина заменяющего слова
	int max_length = min(corrector->max_word_length, initial_length + size_tol);  // максимально допустимая длина заменяющего слова
	if (initial_length <= corrector->max_word_length)  // если слово не превышает максимальную длину изученных, то пытаемся его найти
	{
		HashTable* same_length_table = corrector->dictionary + initial_length - 1;  // таблица, в которой может быть наше слово (в одной таблице слова одной и той же длины)
		exists = ht_has(same_length_table, edited_buffer);
	}
	if (exists == false)  // если слова не существует, то пробуем его отредактировать
	{
		MList* best_replace = NULL;
		unsigned best_distance;  // чтобы не пересчитывать в каждой таблице
		for (unsigned table_index = min_length - 1; table_index < max_length; ++table_index)
			best_replace = single_table_edit(corrector, table_index, edited_buffer, best_replace, &best_distance, threshold);
		if (best_replace != NULL)  // нашли вариант для замены
		{
			const char* best_word = best_replace->word;
			char ask_assent[BUFFER_SIZE];
			sprintf(ask_assent, "Заменить [%s] на [%s]?\n", edited_buffer, best_word);
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
	HashTable* ht = corrector->dictionary + table_index;  // слова длины table_index+1
	int current_distance;
	if (ht != NULL)  // если в модели есть слова такой длины (такая таблица)
	{
		for (unsigned row_index = 0; row_index < ht->size; ++row_index)  // ищем лучший вариант в каждой строке
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