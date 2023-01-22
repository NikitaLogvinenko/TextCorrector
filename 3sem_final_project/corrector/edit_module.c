#include "constants.h"
#include "edit_module.h"
#include "corrector_lib.h"
#include "helpful_functions.h"
#include "files_working.h"
#include "words_handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


// Последовательно считывает текст из одного файла и записывает в новый отредактированный
// Модель уже должна быть загружена
// По заваршении возвращает EXIT_SUCCESSFULLY в случае успешного исполнения, иначе код ошибки:
// * EXIT_MEMORY_FAILURE - не выделилась память
// * EXIT_FILE_FAILURE - не открылся файл
// * EXIT_USER_FILE - ошибка по вине пользователя
static int edit_by_loaded(Corrector* corrector, const Pointer* cfg);

// Загруженная модель corrector редактирует файл initial_file_name и сохраняет в edited_file_name
// Максимально допустимая разница длин отредактированного и редактируемого слов - size_tol
// Максимально допустимое отклонение отредактированного слова от редактируемого - threshold
// Если текст успешно отредактируется и сохранится - вернёт EXIT_SUCCESSFULLY
// Если не откроется файл для чтения или записи - вернёт EXIT_FILE_FAILURE
// Если имена файлов введены некорректно - вернёт EXIT_USER_FAILURE
// Под слова выделяется автоматическая память. Слово считывается из файла в буфер. Для отредактированного варианта также есть свой буфер
// После редактирования слово сразу записывается в файл и ни начальный, ни конечный вариант больше не нужны, поэтому эти же буферы можем заполнять новыми словами
// Таким образом память выделяется не под каждое новое слово, а только один раз в начале
// Т.к. память внутри выделяется только автоматически, то ошибки памяти быть не может
static int edit_one_file(Corrector* corrector, const char* initial_file_name, const char* edited_file_name, const int size_tol, const int threshold);

// Корректор corrector уже должен быть загружен, а параметры size_tol и threshold для редактирования заданы.
// Если после прочтения слова файл ещё не окончен, то вернёт true, иначе false
// Считывается одно слово из файла initial_file, для чего используется буфер initial_buffer. Если надо, слово редактируется в edited_buffer. В конце концов оно выводится в edited_file.
// Размер буферов должен быть не менее corrector->max_word_length + size_tol + 2:
// +1 на возможный небуквенный символ после слова (если после слова не идёт EOF), который тоже выводится в файл после слова, и +1 на хранение конца строки '\0'.
// Если слово было отредактировано, то к счётчику отредактированных слов words_edited_counter прибавляется 1.
// ----------------------------------------------------------------------------
// Сначала проверяется, надо ли поставить пробел после предыдущего ввода (см. об этом пояснение в конце аннотации). Если надо - пробел выводится в edited_file.
// Затем ВСЕ небуквенные символы считываются и выводятся в edited_file без редактирования.
// Если при встрече буквенного символа предыдущий был space_demanding() - см.эту функцию в words_handling - сначала ставится пробел. 
// Далее осуществляется считывание слова до первого небуквенного символа (дефис ВНУТРИ cчитается буквенным).
// Если прочитано corrector->max_word_length + size_tol символов и следующий символ буквенный или дефис, то слово слишком длинное и его не получится заменить словом из corrector. 
// Тогда выводим все символы до первого непробельного включительно в edited_file без изменений.
// Если слово не слишком длинное, то вызывается функция corrector_edit, внутри которой происходит редактирование, если это нужно, и слово выводится в edited_file
// Небуквенный символ после слова также хранится в буфере, выводится в файл в функции corrector_edit и записывается в нулевую позицию в initial_buffer. 
// Если после слова идёт сразу EOF, то в буфере '\0' будет сразу после слова.
// Это необходимо, т.к. при чтении первого символа функция проверяет, если первый символ в initial_file БУКВЕННЫЙ, и первый символ в initial_buffer является space_demanding(), 
// значит в исходном файле был пропущен пробел. В таком случае сначала ставится пробел и лишь затем идёт запись следующих символов из initial_file.
static bool edit_one_word(Corrector* corrector, unsigned size_tol, unsigned threshold,
	FILE* initial_file, FILE* edited_file, char* initial_buffer, char* edited_buffer, unsigned* words_edited_counter);

// Предполагается, что мы дошли при чтении текста до слова и symbol_code - первый буквенный символ, а перед ним был введён prev_code.
// Если prev_code требует после себя пробел - выводим пробел в edited_file.
// Далее записываем все буквенные символы и дефисы в initial_buffer, пока не встретим небуквенный символ и не дефис.
// Если дошли при чтении до конца файла - остановится и вернёт true, иначе вернёт false.
// Если дошли до небуквенного недефисного символа, но не EOF, то записываем этот символ после слова в буффер.
// Если превышено число символов max_letters_init в редактируемом слове, то выведет все символы до первого небуквенного и не дефиса, а в initial_buffer[0] запишет последний выведенный.
// Если предел не превышен, то ничего не выводит в файл, а оставляет слово в буфере вместе с небуквенным символом после него для дальнейшей обработки.
static bool read_word_to_edit(FILE* initial_file, FILE* edited_file, char* initial_buffer, int max_letters_initial, int symbol_code, int prev_code);


/*=================================================================================================================================================================================*/

int edit_text(const Pointer* cfg)
{
	int exit_code = EXIT_SUCCESSFULLY;
	printf("\n### Загрузка существующей модели...\n");
	Corrector existed_corrector;
	const char* model_file_name = (const char*)cfg[1];
	exit_code = corrector_load(&existed_corrector, model_file_name);  // загружаем существующую модель из файла; если не сможем загрузить, то вся прочитанная память очищается внутри
	
	if (exit_code == EXIT_SUCCESSFULLY)
	{
		printf("\n### Загрузка успешно завершена\n### Редактирование текста...\n");
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

	while (exit_code == EXIT_SUCCESSFULLY && yes_no_question("\nТекст отредактирован. Отредактировать ещё один?\n"))
	{
		printf("Текст с русскими словами должен иметь кодировку ANSI. Для английских текстов допустима UTF-8\n");
		char initial_file_name[BUFFER_SIZE], edited_file_name[BUFFER_SIZE];
		printf("Введите полный или относительный путь к тексту для редактирования в формате path\\file_name.txt: ");
		int read_path_code_1 = read_param_from_console(initial_file_name, BUFFER_SIZE);
		printf("Введите полный или относительный путь для сохранения отредактированного текста в формате path\\file_name.txt: ");
		int read_path_code_2 = read_param_from_console(edited_file_name, BUFFER_SIZE);
		if (read_path_code_1 == EXIT_USER_FAILURE || read_path_code_2 == EXIT_USER_FAILURE)
		{
			exit_code = EXIT_USER_FAILURE;
			printf("Путь %d введён неверно\n", (read_path_code_1 == EXIT_USER_FAILURE) ? 1 : 2);
		}
		else if (read_path_code_1 == EXIT_MEMORY_FAILURE || read_path_code_2 == EXIT_MEMORY_FAILURE)
		{
			exit_code = EXIT_MEMORY_FAILURE;
			printf("Путь %d слишком длинный\n", (read_path_code_1 == EXIT_MEMORY_FAILURE) ? 1 : 2);
		}
		else
			exit_code = edit_one_file(corrector, initial_file_name, edited_file_name, size_tol, threshold);
	}

	if (exit_code != EXIT_SUCCESSFULLY)
		printf("Сожалеем, на этапе редактирования произошла ошибка. Повторите попытку\n");
	else
		printf("\n### Редактирование успешно завершено\n");
	
	corrector_destroy(corrector);  // чистим память, выделенную под таблицы и слова
	return exit_code;
}

static int edit_one_file(Corrector* corrector, const char* initial_file_name, const char* edited_file_name, const int size_tol, const int threshold)
{
	int exit_code = EXIT_SUCCESSFULLY;
	FILE* initial_file = NULL, * edited_file = NULL;
	if (path_is_txt(initial_file_name) && path_is_txt(edited_file_name))  // если пути являются путями к .txt файлам - пробуем открыть в нужном режиме
	{
		initial_file = fopen(initial_file_name, "r");
		edited_file = fopen(edited_file_name, "w");
	}
	else  // пути введены неверно
	{
		printf("Путь %d не является путём к .txt файлу. Редактирование прервано\n", path_is_txt(initial_file) ? 2 : 1);
		exit_code = EXIT_USER_FAILURE;
	}

	if (exit_code == EXIT_SUCCESSFULLY && not_null_ptr(initial_file, "Не удалось открыть файл с текстом\n") && not_null_ptr(edited_file, "Не удалось открыть файл для сохранения\n"))
	{
		// файлы открылись, можем редактировать
		bool file_not_ended = true;  // индикатор, что надо продолжать редактирование
		unsigned words_edited = 0;  // счётчик отредактированных слов
		char initial_buffer[MAX_AVAILABLE_WORD_LENGTH + MAX_AVAILABLE_MISSES + 2], edited_buffer[MAX_AVAILABLE_WORD_LENGTH + MAX_AVAILABLE_MISSES + 2];
		// +2, т.к. +1 на небуквенный недефисный символ после слова, который выводится после слова и его надо запомнить, и +1 на '\0'
		initial_buffer[0] = ' ';  // так нужно, чтобы перел самым первым символом файла не стоял пробел
		// Максимальная длина редактируемого слова может на max_size_tol превышать длину самого длинного изученного слова (если у нас просто MAX_AVAILABLE_MISSES букв лишние)
		while (file_not_ended)
			file_not_ended = edit_one_word(corrector, size_tol, threshold, initial_file, edited_file, initial_buffer, edited_buffer, &words_edited);
		printf("\nОтредактировано слов: %d\n", words_edited);
	}
	else // один или оба файла не открылись
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
	int max_letters_initial = corrector->max_word_length + size_tol;  // сколько максимум букв и дефисов может быть в слове
	int prev_code = initial_buffer[0];  // предыдущий записанный в файл символ
	int symbol_code = fgetc(initial_file);  // fgetc возвращает код unsigned_char или EOF<0
	file_not_ended = (symbol_code != EOF);  // проверка, вдруг файл закончился, как только вызвали функцию
	if (file_not_ended)  // файл не закончился, продолжаем редактирование
	{
		prev_code = space_before(edited_file, prev_code, symbol_code);  // ставим пробел перед новыми символами, если надо
		while (file_not_ended && is_letter(symbol_code) == false)  // все небуквенные символы (в т.ч. дефисы ПЕРЕД словом) напрямую выводим в новый файл
		{
			fputc(symbol_code, edited_file);  // небуквенные символы выводим в файл
			prev_code = symbol_code;  // запоминаем, что вывели
			symbol_code = fgetc(initial_file);  // считываем новый символ
			file_not_ended = (symbol_code != EOF);  // проверка конца файла
		}
		if (file_not_ended)  // нашли слово в оставшемся файле, а не дошли до конца файла
			file_not_ended = read_word_to_edit(initial_file, edited_file, initial_buffer, max_letters_initial, symbol_code, prev_code);  // считываем слово в буфер
		
		// Если первый символ - буква (дефиса перед словом также быть не может) => слово поместилось в буфер => его нужно обработать и вывести отдельно
		if (is_letter(initial_buffer[0]))
		{
			prev_code = corrector_edit(corrector, initial_buffer, edited_buffer, edited_file, size_tol, threshold, words_edited_counter);  // редактируем и выводим слово в файл
			initial_buffer[0] = prev_code;  // последний символ записываем в initial_buffer[0]
		}
	}
	return file_not_ended;
}

static bool read_word_to_edit(FILE* initial_file, FILE* edited_file, char* initial_buffer, int max_letters_initial, int symbol_code, int prev_code)
{
	prev_code = space_before(edited_file, prev_code, symbol_code);  // ставим пробел перед словом, если надо
	bool file_not_ended = true;
	// Считываем слова в буфер либо пока не закончится файл, либо пока не встретим первый небуквенный символ и не дефис, либо пока не превысим лимит букв в редактируемом слове
	unsigned letter_index = 0;
	for (; letter_index < max_letters_initial && file_not_ended && (is_letter(symbol_code) || symbol_code == '-'); ++letter_index)
	{
		initial_buffer[letter_index] = symbol_code;  // записываем дефис или букву в буфер
		symbol_code = fgetc(initial_file);  // считываем новый символ
		file_not_ended = (symbol_code != EOF);  // проверка конца файла
	}
	// Делаем проверку, почему вышли из цикла
	initial_buffer[letter_index] = '\0';  // не забываем обозначить конец слова (букву мы записали, а вот символ, при котором произошёл выход из цикла - пока что нет (см. в конце))
	if (file_not_ended && (is_letter(symbol_code) || symbol_code == '-')) // остановка цикла произошла из-за того, что считали максимум букв и следующий символ опять дефис или буква
	{
		fputs(initial_buffer, edited_file);  // выписали то, что было в буфере
		while (file_not_ended && (is_letter(symbol_code) || symbol_code == '-'))  // выписываем все последующие буквы и дефисы, пока не встретим другой символ или конец файла
		{
			fputc(symbol_code, edited_file);  // выводим символ в файл (последний прочитанный в for цикле также не был записан в буфер и его надо вывести отдельно)
			symbol_code = fgetc(initial_file);  // считываем новый символ
			file_not_ended = (symbol_code != EOF);  // проверка конца файла
		}
		if (file_not_ended)  // если дошли до небуквенного недефисного символа, а не до конца файла, то тоже выводим его и записываем в initial_buffer[0]
		{
			fputc(symbol_code, edited_file);
			initial_buffer[0] = symbol_code;
		}
		// Внутри этой функции файл мог закончится до и после превышения лимита символов
		// Если файл заканчивается до превышения лимита, то слово надо обрабатывать в corrector_edit, поэтому слово остаётся в буфере и первый символ - буквенный.
		// Если же файл заканчивается при выводе символов после превышения, то больше ничего обрабатывать не надо, т.к. всё уже вывели в файл.
		// Но в initial_buffer до сих пор хранится начало длинного слова. Чтобы его не редактировать в corrector_edit, поставим в нулевой позиции пробел.
		else  // дошли до конца файла после превышения лимита символов и вывода всех лишних символов в файл
			initial_buffer[0] = ' ';
	}
	// Если даже прочитали максимум символов, но следующий EOF или небуквенный недефисный, то редактирование возможно ("на тоненького")
	else if (file_not_ended)  // если дошли до небуквенного недефисного символа раньше превышения лимита символов, то надо его записать в конец буфера
	{
		initial_buffer[letter_index] = symbol_code;
		initial_buffer[letter_index + 1] = '\0';
	}
	// если дошли до конца файла до превышения лимита, то слово всё же надо будет обработать в corrector_edit
	return file_not_ended;
}
