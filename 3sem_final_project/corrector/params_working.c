#include "params_working.h"
#include "constants.h"
#include "helpful_functions.h"
#include "files_working.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <direct.h>


// Определить способ установки конфигурации работы программы
static int define_set_way(int argc, char** argv);

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// Определить конфигурацию по параметрам при запуске. params_amount - сколько параметров из консоли попадёт в функцию (вдруг их недостаточно)
// Все параметры записываем во временный файл TMP_PARAMS_FILE и вызываем cfg_from_file(TMP_PARAMS_FILE)
// ВСЕ параметры записываем в кавычках, т.к. во всех функциях read_(param) кавычки удаляются, т.к. их не должно быть ни в каких параметрах. Это нужно на случай, если путь содержит пробелы
// В случае ошибок работы с памятью возвращается NULL (не выделилась память под указатели) или массив указателей, в котором содержится NULL (не выделилась память под параметр).
// Тогда память cfg чистится в check_cfg_filled()
// В случае ошибок работы с файлом программа мгновенно завершается с сообщением об ошибке. Возможны две ошибки с файлами:
// 1. Ошибка создания TMP_PARAMS_FILE. Тогда временная память ещё не выделялась и чистить нечего
// 2. Ошибка открытия файла TMP_PARAMS_FILE в ходе работы cfg_from_file
// Т.к. память под параметры не выделялась до открытия, то чистить нечего, разве что надо удалить TMP_PARAMS_FILE в cfg_from_file
static Pointer* cfg_from_params(int params_amount, const char** params_for_cfg);

// Ввести конфигурацию в консоль
// В случае ошибок работы с памятью возвращается NULL (не выделилась память под указатели) или массив указателей, в котором содержится NULL (не выделилась память под параметр).
// Тогда память cfg чистится в check_cfg_filled(). Кроме того удаляются файлы, созданные под новые модели или под отредактированный текст, т.к. исполнение программы было прервано
static Pointer* cfg_step_by_step();

// Считать конфигурацию из файла path_to_cfg
// Параметры в файле должны идти через пробел. ВСЕ параметры должны быть заключены в кавычки. Это сделано для удобства функций чтения и записи
// В случае ошибок работы с памятью возвращается NULL (не выделилась память под указатели) или массив указателей, в котором содержится NULL (не выделилась память под параметр).
// Тогда память cfg чистится в check_cfg_filled()
// В случае ошибок работы с файлом программа мгновенно завершается с сообщением об ошибке. Возможна единственная ошибка при работе с файлом - ошибка открытия файла с конфигурацией
// Т.к. память под параметры не выделялась до открытия, то чистить нечего. Разве что надо удалить TMP_PARAMS_FILE, если это был временный файл (параметры получены при запуске)
static Pointer* cfg_from_file(const char* path_to_cfg);

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// Проверить в set_cfg, что весь cfg заполнен (нет NULL указателей).
// Это может быть не так, если под какой-то параметр не выделилась память. В таком случае чистим остальные параметры и завершаем программу
static void check_cfg_filled(Pointer* cfg);

// Проверить что params_exist параметров не меньше, чем требуется (params_required).
// Если недостаточно параметров, написать сообщение exit_msg, вывести справку и выйти
static bool sufficiently_params_check(int params_required, int params_exist, const char* exit_msg);

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// Считать параметр из потока file_from с помощью функции read_func, возвращающей указатель на параметр, память под который выделена динамически (если память не выделилась, то NULL)
// Корректность введённого параметра проверяется функцией verification, принимающей указатель на параметр
// Если функция верификации получает NULL, то она также должна возвращать true. Это означает ошибку работы с памятью, которая будет обработана в самом конце настройки cfg
// Если file_from == stdin (ввод через консоль), то сначала пользователю выводят ask_msg, а потом дают ему несколько попыток ввести параметр корректно
// Если параметр считывается из файла, то параметр считывается один раз и права исправиться нет
// Если не удалось получить корректный параметр, то пишем об этом (для этого нужен param_name), чистим cfg,
// закрываем файл file_from (удаляем его, если это был временный файл; проверка с помощью передаваемого имени файла path_to_cfg) и завершаем работу программы
// В случае ошибок работы с памятью возвращается NULL. Если всё хорошо - возвращаем указатель на параметр
static Pointer ask_cfg_param(FILE* file_from, const char* path_to_cfg,
							 Pointer (*read_func)(FILE*), bool (*verification)(Pointer), 
							 Pointer* cfg, const char* param_name, const char* ask_msg); 

// Получить из файла file_from конфигурацию для обучения новой модели. Предполагается, что файл уже открыт. Если ввод с консоли, то file_from == stdin
// Возвращается динамический массив указателей на параметры конфига
// Режим уже выбран и он всегда передаётся правильный. Просто память под него уже выделена и его надо тоже записать в cfg
// В случае ошибок работы с памятью возвращается NULL
// path_to_cfg - передаётся на случай экстренного выхода из программы. Если это пользовательский файл с конфигурацией, то просто закрыть его, а если временный - то удалить
static Pointer* ask_train_new_cfg(FILE* file_from, const char* path_to_cfg, int* mode);

// Получить из файла file_from конфигурацию для дообучения существующей модели. Предполагается, что файл уже открыт. Если ввод с консоли, то file_from == stdin
// Режим уже выбран и он всегда передаётся правильный. Просто память под него уже выделена и его надо тоже записать в cfg
// В случае ошибок работы с памятью возвращается NULL
// path_to_cfg - передаётся на случай экстренного выхода из программы. Если это пользовательский файл с конфигурацией, то просто закрыть его, а если временный - то удалить
static Pointer* ask_train_existed_cfg(FILE* file_from, const char* path_to_cfg, int* mode);

// Получить из файла file_from конфигурацию для редактирования текста. Предполагается, что файл уже открыт. Если ввод с консоли, то file_from == stdin
// Режим уже выбран и он всегда передаётся правильный. Просто память под него уже выделена и его надо тоже записать в cfg
// В случае ошибок работы с памятью возвращается NULL
// path_to_cfg - передаётся на случай экстренного выхода из программы. Если это пользовательский файл с конфигурацией, то просто закрыть его, а если временный - то удалить
static Pointer* ask_edit_cfg(FILE* file_from, const char* path_to_cfg, int* mode);

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// ARV - Ask, Read, Verification. Объединённая функция для чтения из file_from и верификации режима работы
// Если параметр введён корректно - вернёт указатель (память выделена динамически, в случае ошибок с памятью вернёт NULL). Если параметр введён некорректно - выход из программы
static Pointer ARV_mode(FILE* file_from, const char* path_to_cfg);

// ARV - Ask, Read, Verification. Объединённая функция для чтения из file_from и верификации пути для сохранения новой модели в режиме train_new
// Если параметр введён корректно - вернёт указатель (память выделена динамически, в случае ошибок с памятью вернёт NULL). Если параметр введён некорректно - выход из программы
// cfg - для очистки конфига в случае некорректного ввода и последующего завершения работы программы
static Pointer ARV_path_to_new_model(FILE* file_from, const char* path_to_cfg, Pointer* cfg);

// ARV - Ask, Read, Verification. Объединённая функция для чтения из file_from и верификации пути к существующей модели в режимах train_existed / edit
// Если параметр введён корректно - вернёт указатель (память выделена динамически, в случае ошибок с памятью вернёт NULL). Если параметр введён некорректно - выход из программы
// cfg - для очистки конфига в случае некорректного ввода и последующего завершения работы программы
static Pointer ARV_path_to_existed_model(FILE* file_from, const char* path_to_cfg, Pointer* cfg);

// ARV - Ask, Read, Verification. Объединённая функция для чтения из file_from и верификации пути для сохранения отредактированного текста в режиме edit
// Если параметр введён корректно - вернёт указатель (память выделена динамически, в случае ошибок с памятью вернёт NULL). Если параметр введён некорректно - выход из программы
// cfg - для очистки конфига в случае некорректного ввода и последующего завершения работы программы
static Pointer ARV_path_to_new_text(FILE* file_from, const char* path_to_cfg, Pointer* cfg);

// ARV - Ask, Read, Verification. Объединённая функция для чтения из file_from и верификации пути к изучаемому (в режиме train_*) или редактируемому (в режиме edit) тексту
// Если параметр введён корректно - вернёт указатель (память выделена динамически, в случае ошибок с памятью вернёт NULL). Если параметр введён некорректно - выход из программы
// cfg - для очистки конфига в случае некорректного ввода и последующего завершения работы программы
static Pointer ARV_path_to_existed_text(FILE* file_from, const char* path_to_cfg, Pointer* cfg, int* mode);

// ARV - Ask, Read, Verification. Объединённая функция для чтения из file_from и верификации максимальной длины изучаемого слова в режиме train_new
// Если параметр введён корректно - вернёт указатель (память выделена динамически, в случае ошибок с памятью вернёт NULL). Если параметр введён некорректно - выход из программы
// cfg - для очистки конфига в случае некорректного ввода и последующего завершения работы программы
static Pointer ARV_max_word_length(FILE* file_from, const char* path_to_cfg, Pointer* cfg);

// ARV - Ask, Read, Verification. Объединённая функция для чтения из file_from и верификации максимально допустимого отклонения длины заменяющего слова в режиме edit
// Если параметр введён корректно - вернёт указатель (память выделена динамически, в случае ошибок с памятью вернёт NULL). Если параметр введён некорректно - выход из программы
// cfg - для очистки конфига в случае некорректного ввода и последующего завершения работы программы
static Pointer ARV_size_tol(FILE* file_from, const char* path_to_cfg, Pointer* cfg);

// ARV - Ask, Read, Verification. Объединённая функция для чтения из file_from и верификации максимально допустимой разности слов в режиме edit
// Если параметр введён корректно - вернёт указатель (память выделена динамически, в случае ошибок с памятью вернёт NULL). Если параметр введён некорректно - выход из программы
// cfg - для очистки конфига в случае некорректного ввода и последующего завершения работы программы
static Pointer ARV_threshold(FILE* file_from, const char* path_to_cfg, Pointer* cfg);

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// Считывает в виде строки (в кавычках или без) режим из потока file_from. Выделяет под него динамически память и возвращает указатель на НОМЕР режима.
// Если память не выделилась - вернёт NULL. Предполагается что файл уже открыт
static int* read_mode(FILE* file_from);

// Считывает какой-либо путь из потока file_from. Выделяет под него динамически память и возвращает указатель на строку с этим путём.
// Если память не выделилась - вернёт NULL. Предполагается что файл уже открыт
static char* read_path_param(FILE* file_from);

// Считывает целочисленный параметр (max_word_length, size_tol, threshold) из потока file_from. Выделяет под него динамически память и возвращает указатель на int.
// Если память не выделилась - вернёт NULL. Предполагается что файл уже открыт
// Параметр считывается как строка и только потом пробуем преобразовать его в число, т.к. он может быть в кавычках
static int* read_int_param(FILE* file_from);

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// Проверяет, что пользователь корректно ввёл режим работы (от 0 до (MODES_AMOUNT - 1))
static bool verify_mode(const int* mode);

// Проверяет, что пользователь корректно ввёл путь для сохранения новой модели/отредактированного текста
// Проверка, что путь заканчивается на .txt; далее пробует создать файл в режиме записи
static bool verify_path_to_new(const char* path_to_new);

// Проверяет, что пользователь корректно ввёл путь к обученной модели/обучающему тексту/редактируемому тексту
// Проверка, что путь заканчивается на .txt; далее пробует открыть файл в режиме чтения
static bool verify_path_to_existed(const char* path_to_existed);

// Проверяет, что пользователь корректно ввёл максимальную длину слов (от 1 до (MAX_AVAILABLE_WORD_LENGTH - 1))
static bool verify_max_word_length(const int* max_word_length);

// Проверяет, что пользователь корректно ввёл size_tol (от 0 до MAX_AVAILABLE_MISSES)
static bool verify_size_tol(const int* size_tol);

// Проверяет, что пользователь корректно ввёл threshold (от 0 до MAX_AVAILABLE_ERRORS)
static bool verify_threshold(const int* threshold);


/*=================================================================================================================================================================================*/

Pointer* set_cfg(int argc, char** argv)
{
	// Определяем способ установки конфигурации
	int set_way = define_set_way(argc, argv);
	//Пользователь запросил справку
	if (set_way == PRINT_HELP)
	{
		print_help();
		exit(EXIT_SUCCESSFULLY);
	}
	// Пользователь хочет воспользоваться программой. Определяем конфигурацию
	else
	{
		Pointer* cfg = NULL;
		switch (set_way)
		{
		case CFG_FROM_PARAMS:
			cfg = cfg_from_params(argc - 2, argv + 2);  // пропускаем путь к программе и способ задания конфига при передаче параметров
			break;
		case CFG_STEP_BY_STEP:
			cfg = cfg_step_by_step();
			break;
		case CFG_FROM_FILE:
			sufficiently_params_check(1, argc - 2, "Не задан путь к файлу с конфигурацией программы\n");  // проверка, что путь к файлу с конфигурацией был введён
			cfg = cfg_from_file(argv[2]);  // пропускаем путь к программе и способ задания конфига при передаче параметров
			break;
		default:
			exit_with_msg("Такой возможности установки конфигурации не предусмотрено.\n", EXIT_USER_FAILURE);
		}
		check_cfg_filled(cfg);
		return cfg;
	}
}

void delete_cfg(Pointer* cfg)
{
	// Если память под конфиг выделилась и режим определён, то надо почистить память под указатели
	if (cfg != NULL && cfg[0] != NULL)
	{
		int possible_cfg_sizes[] = { TRAIN_NEW_CFG_SIZE, TRAIN_EXISTED_CFG_SIZE, EDIT_CFG_SIZE };
		int mode = *(int*)cfg[0];  // режим работы программы
		int cfg_size = possible_cfg_sizes[mode];  // по режиму работы определяем количество параметров конфигурации
		for (int i = 0; i < cfg_size; ++i)
			free(cfg[i]);  // чистим память, выделенную под параметры конфигурации
	}
	free(cfg);  // чистим память, выделенную непременно под указатели
}


/*=================================================================================================================================================================================*/


static int define_set_way(int argc, char** argv)
{
	int set_way = WRONG_PARAM;
	if (argc == 1) // параметров от пользователя нет (единственный параметр - путь к исполняемой программе); просим пользователя ввести конфигурацию в консоль
		set_way = CFG_STEP_BY_STEP;
	else
	{
		if (strcmp("params", argv[1]) == 0)
			set_way = CFG_FROM_PARAMS;
		else if (strcmp("step_by_step", argv[1]) == 0)
			set_way = CFG_STEP_BY_STEP;
		else if (strcmp("from_file", argv[1]) == 0)
			set_way = CFG_FROM_FILE;
		else if (strcmp("help", argv[1]) == 0)
			set_way = PRINT_HELP;
	}
	return set_way;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static Pointer* cfg_from_params(int params_amount, const char** params_for_cfg)
{
	sufficiently_params_check(1, params_amount, "В параметрах отсуствует режим работы программы\n");
	// Если есть хоть один параметр, то создаём временный файл, записываем их туда и считываем функцией cfg_from_file
	FILE* tmp_file = create_and_open_tmp(TMP_PARAMS_FILE, "w"); // если всё прошло без ошибок - вернётся указатель на открытый файл. В случае ошибки вернётся NULL
	// В случае ошибки создания или открытия - выход из программы с кодом EXIT_FILE_FAILURE (т.к. динамическая память ещё не выделялась)
	// Созданный, но неоткрытый файл удаляется в create_and_open_tmp()
	if (tmp_file == NULL)
		exit_with_msg("Ошибка создания временного файла.\nСожалеем и приносим извинения за доставленные неудобства!\nПовторите попытку\n", EXIT_FILE_FAILURE);
	bool successfully_written = file_write(tmp_file, params_amount, params_for_cfg, "\"", " ", NULL);
	fclose(tmp_file);
	Pointer* cfg = NULL;
	if (successfully_written)
		cfg = cfg_from_file(TMP_PARAMS_FILE); // если файл не откроется, это обработается в cfg_from_file; если произойдёт ошибка памяти, вернётся NULL
	delete_if_tmp(TMP_PARAMS_FILE);
	return cfg;
}

static Pointer* cfg_step_by_step()
{
	printf("Конфигурация работы программы не задана. Введите необходимые параметры в консоль\nПараметры можно вводить в двойных кавычках\n");
	// Вводим режим работы; если память под режим не выделится или режим введён не верно, то сразу же выйдем из программы (всё равно чистить пока нечего)
	int* mode = ARV_mode(stdin, NULL);
	Pointer* cfg = NULL;
	// В зависимости от режима работы заполняем нужный конфиг
	switch (*mode)
	{
	case TRAIN_NEW_MODE:
		cfg = ask_train_new_cfg(stdin, NULL, mode);
		break;
	case TRAIN_EXISTED_MODE:
		cfg = ask_train_existed_cfg(stdin, NULL, mode);
		break;
	case EDIT_MODE:
		cfg = ask_edit_cfg(stdin, NULL, mode);
		break;
	}
	// Если не выделилась память под cfg, то дальше mode не почистится, а память под него уже выделена. Чистим вручную!
	// Если же память под cfg выделилась, а под какой-то параметр нет, то mode будет уже в cfg и потом почистится, отдельно этого делать не надо
	if (cfg == NULL)
		free(mode);
	return cfg;
}

static Pointer* cfg_from_file(const char* path_to_cfg)
{
	FILE* cfg_file = fopen(path_to_cfg, "r");  // открываем файл с конфигурацией
	if (cfg_file == NULL) // файл с конфигурацией не открылся
	{
		// Удаляем, если это был временный файл
		delete_if_tmp(path_to_cfg);
		// Выходим (т.к. память ещё не выделялась)
		exit_with_msg("Ошибка открытия файла с параметрами.\nСожалеем и приносим извинения за доставленные неудобства!\nПовторите попытку\n", EXIT_FILE_FAILURE);
	}
	// Вводим режим работы; если память под режим не выделится или режим введён не верно, то закроем файл, удалим его (если он временный) и выйдем из программы (чистить пока нечего)
	int* mode = ARV_mode(cfg_file, path_to_cfg);
	Pointer* cfg = NULL;
	// В зависимости от режима работы заполняем нужный конфиг
	switch (*mode)
	{
	case TRAIN_NEW_MODE:
		cfg = ask_train_new_cfg(cfg_file, path_to_cfg, mode);
		break;
	case TRAIN_EXISTED_MODE:
		cfg = ask_train_existed_cfg(cfg_file, path_to_cfg, mode);
		break;
	case EDIT_MODE:
		cfg = ask_edit_cfg(cfg_file, path_to_cfg, mode);
		break;
	}
	// Если не выделилась память под cfg, то дальше mode не почистится, а память под него уже выделена. Чистим вручную!
	// Если же память под cfg выделилась, а под какой-то параметр нет, то mode будет уже в cfg и потом почистится, отдельно этого делать не надо
	if (cfg == NULL)
		free(mode);
	fclose(cfg_file);
	return cfg;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static void check_cfg_filled(Pointer* cfg)
{
	bool cfg_filled = true;
	if (cfg == NULL || cfg[0] == NULL)  // конфига нет или неопределён режим работы программы (от него зависит число параметров)
		cfg_filled = false;
	else
	{
		int mode = *(int*)cfg[0];  // режим работы программы
		int possible_cfg_sizes[] = { TRAIN_NEW_CFG_SIZE, TRAIN_EXISTED_CFG_SIZE, EDIT_CFG_SIZE };		
		int cfg_size = possible_cfg_sizes[mode];  // по режиму работы определяем количество параметров конфигурации
		for (int i = 1; i < cfg_size && cfg_filled; ++i)
		{
			if (cfg[i] == NULL)
				cfg_filled = false;
		}
	}
	if (!cfg_filled)  // не выделилась память под конфиг или под какой-то параметр конфига, надо очистить динамическую память конфига
	{
		delete_cfg(cfg);
		exit_with_msg(APOLOGY_MEMORY_MSG, EXIT_MEMORY_FAILURE);
	}
}

static bool sufficiently_params_check(int params_required, int params_exist, const char* exit_msg)
{
	if (params_exist < params_required)
		exit_with_msg(exit_msg, EXIT_USER_FAILURE);
	return true;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static Pointer ask_cfg_param(FILE* file_from, const char* path_to_cfg,
							 Pointer(*read_func)(FILE*),	bool (*verification)(Pointer),
							 Pointer* cfg, const char* param_name, const char* ask_msg)
{
	if (file_from == stdin && ask_msg != NULL)
		printf("%s", ask_msg);
	Pointer param = read_func(file_from);
	bool correct_param = verification(param);
	if (file_from == stdin)  // если ввод пользователем из консоли, но параметр некорректный, даём ещё несколько попыток
	{
		// продолжаем, пока пользователь не введёт правильно, пока не потратит все попытки или пока не произойдёт ошибка памяти
		for (unsigned attempt = 0; attempt < MAX_USER_ATTEMPTS && correct_param == false; ++attempt)
		{
			free(param);
			printf("Некорректный ввод параметра %s. Повторите попытку: ", param_name);
			param = read_func(file_from);
			correct_param = verification(param);
		}
	}
	if (correct_param == false) // пользователь не справился с настройкой параметра(((
	{
		free(param);
		delete_cfg(cfg);  // удалить то, что уже записано в конфигурации
		if (file_from != NULL && file_from != stdin) // закрыть файл, из которого считывался параметр (консоль не закрываем)
			fclose(file_from);
		delete_if_tmp(path_to_cfg); // удалить файл с конфигурацией, если он был временным (в файле есть 'temporary_file__')
		printf("Параметр %s введён некорректно! Прочитайте справку и повторите попытку\n", param_name);
		exit_with_msg(NULL, EXIT_USER_FAILURE);
	}
	return param; // пользователь справился))). Ну или не выделилась память...
}

static Pointer* ask_train_new_cfg(FILE* file_from, const char* path_to_cfg, int* mode)
{
	Pointer* train_new_cfg = empty_pointers_array(TRAIN_NEW_CFG_SIZE);
	if (train_new_cfg != NULL)  // если память под cfg выделилась, заполняем его параметрами (не забыв запомнить режим)
	{
		// Запоминаем режим
		train_new_cfg[0] = mode;
		// Получаем путь для сохранения модели
		train_new_cfg[1] = ARV_path_to_new_model(file_from, path_to_cfg, train_new_cfg);
		// Получаем путь к обучающему тексту
		train_new_cfg[2] = ARV_path_to_existed_text(file_from, path_to_cfg, train_new_cfg, mode);
		// Получаем максимальную длину запоминаемых слов
		train_new_cfg[3] = ARV_max_word_length(file_from, path_to_cfg, train_new_cfg);
	}
	return train_new_cfg;
}

static Pointer* ask_train_existed_cfg(FILE* file_from, const char* path_to_cfg, int* mode)
{
	Pointer* train_existed_cfg = empty_pointers_array(TRAIN_EXISTED_CFG_SIZE);
	if (train_existed_cfg != NULL)  // если память под cfg выделилась, заполняем его параметрами (не забыв указать на режим)
	{
		// Запоминаем режим
		train_existed_cfg[0] = mode;
		// Получаем путь к обученной модели
		train_existed_cfg[1] = ARV_path_to_existed_model(file_from, path_to_cfg, train_existed_cfg);
		// Получаем путь к обучающему тексту
		train_existed_cfg[2] = ARV_path_to_existed_text(file_from, path_to_cfg, train_existed_cfg, mode);
	}
	return train_existed_cfg;
}

static Pointer* ask_edit_cfg(FILE* file_from, const char* path_to_cfg, int* mode)
{
	Pointer* edit_cfg = empty_pointers_array(EDIT_CFG_SIZE);
	if (edit_cfg != NULL)  // если память под cfg выделилась, заполняем его параметрами (не забыв запомнить режим)
	{
		// Запоминаем режим
		edit_cfg[0] = mode;
		// Получаем путь к обученной модели
		edit_cfg[1] = ARV_path_to_existed_model(file_from, path_to_cfg, edit_cfg);
		// Получаем путь к редактируемому тексту
		edit_cfg[2] = ARV_path_to_existed_text(file_from, path_to_cfg, edit_cfg, mode);
		// Получаем путь для сохранения отредактированного текста
		edit_cfg[3] = ARV_path_to_new_text(file_from, path_to_cfg, edit_cfg);
		// Получаем максимально допустимое отклонение длины заменяющего слова от заменяемого (`size_tol`)
		edit_cfg[4] = ARV_size_tol(file_from, path_to_cfg, edit_cfg);
		// Получаем максимально допустимую разность слов (`threshold`)
		edit_cfg[5] = ARV_threshold(file_from, path_to_cfg, edit_cfg);
	}
	return edit_cfg;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static Pointer ARV_mode(FILE* file_from, const char* path_to_cfg)
{
	char* ask_msg = NULL;
	char str_1[BUFFER_SIZE] = "\nРежим работы корректора:\n";
	// приходится выделять буфер вне if'а, чтобы он сохранился и попал в ask_cfg_param
	if (file_from == stdin)
	{
		// Формируем запрос на ввод режима работы
		char str_2[] = "\t\"train_new\" - обучение новой модели\n\t\"train_existed\" - дообучение существующей модели\n\t\"edit\" - редактирование текста\n";
		char str_3[] = "Введите режим: ";
		ask_msg = strcat(strcat(str_1, str_2), str_3);
	}
	// Получаем режим работы
	int* mode = ask_cfg_param(file_from, path_to_cfg,
							  read_mode, verify_mode,
							  NULL, "РЕЖИМ РАБОТЫ ПРОГРАММЫ", ask_msg);
	if (mode == NULL)  // не выделилась память под режим; закрываем файл с конфигурацией (если он временный, то ещё и удаляем) и выходим из программы
	{
		if (file_from != NULL && file_from != stdin) // закрыть файл, из которого считывался параметр (консоль не закрываем)
			fclose(file_from);
		delete_if_tmp(path_to_cfg); // удалить файл с конфигурацией, если он был временным (в файле есть 'temporary_file__')
		exit_with_msg(APOLOGY_MEMORY_MSG, EXIT_MEMORY_FAILURE);
	}
	return mode;
}

static Pointer ARV_path_to_new_model(FILE* file_from, const char* path_to_cfg, Pointer* cfg)
{
	char* ask_msg = NULL;
	char str_1[BUFFER_SIZE] = "\nОбученная модель должна быть сохранена в файл В ФОРМАТЕ .TXT\nТекущая директория:\n";
	// приходится выделять буфер вне if'а, чтобы он сохранился и попал в ask_cfg_param
	if (file_from == stdin)
	{
		char* current_path = rel_to_abs_way(NULL, false, true);
		char str_2[BUFFER_SIZE] = "\nВведите абсолютный или относительный путь для сохранения новой модели в формате path\\model_name.txt: ";
		ask_msg = strcat(strcat(str_1, current_path), str_2);
		free(current_path);
	}
	char* path_to_new_model = ask_cfg_param(file_from, path_to_cfg,
										    read_path_param, verify_path_to_new,
											cfg, "ПУТЬ ДЛЯ СОХРАНЕНИЯ НОВОЙ МОДЕЛИ", ask_msg);
	return path_to_new_model;
}

static Pointer ARV_path_to_existed_model(FILE* file_from, const char* path_to_cfg, Pointer* cfg)
{
	char* ask_msg = NULL;
	char str_1[BUFFER_SIZE] = "\nДля дальнейшей работы нужна уже обученная модель, сохранённая в файле В ФОРМАТЕ .TXT\nТекущая директория:\n"; 
	// приходится выделять буфер вне if'а, чтобы он сохранился и попал в ask_cfg_param
	if (file_from == stdin)
	{
		char* current_path = rel_to_abs_way(NULL, false, true);
		char str_2[BUFFER_SIZE] = "\nВведите абсолютный или относительный путь к обученной модели в формате path\\model_name.txt: ";
		ask_msg = strcat(strcat(str_1, current_path), str_2);
		free(current_path);
	}
	char* path_to_existed_model = ask_cfg_param(file_from, path_to_cfg,
												read_path_param, verify_path_to_existed,
												cfg, "ПУТЬ К ОБУЧЕННОЙ МОДЕЛИ", ask_msg);
	return path_to_existed_model;
}

static Pointer ARV_path_to_new_text(FILE* file_from, const char* path_to_cfg, Pointer* cfg)
{
	char* ask_msg = NULL;
	char buffer[BUFFER_SIZE];
	// приходится выделять буфер вне if'а, чтобы он сохранился и попал в ask_cfg_param
	if (file_from == stdin)
	{
		char* current_path = rel_to_abs_way(NULL, false, true);
		sprintf(buffer, "\nТекущая директория:\n%s\nВведите абсолютный или относительный путь в формате path\\filename.txt, по которому сохранить отредактированный текст: ", 
				current_path);
		ask_msg = buffer;
		free(current_path);
	}
	char* path_to_new_text = ask_cfg_param(file_from, path_to_cfg,
										   read_path_param, verify_path_to_new,
										   cfg, "ПУТЬ ДЛЯ СОХРАНЕНИЯ ОТРЕДАКТИРОВАННОГО ТЕКСТА", ask_msg);
	return path_to_new_text;
}

static Pointer ARV_path_to_existed_text(FILE* file_from, const char* path_to_cfg, Pointer* cfg, int* mode)
{
	char* ask_msg = NULL;
	char* param_name = (*mode == EDIT_MODE) ? "ПУТЬ К ТЕКСТУ ДЛЯ РЕДАКТИРОВАНИЯ" : "ПУТЬ К ТЕКСТУ ДЛЯ ОБУЧЕНИЯ";
	char buffer[BUFFER_SIZE];
	// приходится выделять буфер вне if'а, чтобы он сохранился и попал в ask_cfg_param
	if (file_from == stdin)
	{
		char* current_path = rel_to_abs_way(NULL, false, true);
		char* purpose = (*mode == EDIT_MODE) ? "редактирования" : "обучения";
		sprintf(buffer, "\nТекущая директория:\n%s\nВведите абсолютный или относительный путь в формате path\\filename.txt к тексту для %s: ", current_path, purpose);
		ask_msg = buffer;
		free(current_path);
	}
	char* path_to_existed_text = ask_cfg_param(file_from, path_to_cfg,
											   read_path_param, verify_path_to_existed,
											   cfg, param_name, ask_msg);
	return path_to_existed_text;
}

static Pointer ARV_max_word_length(FILE* file_from, const char* path_to_cfg, Pointer* cfg)
{
	char* ask_msg = NULL;
	char buffer[BUFFER_SIZE];
	// приходится выделять буфер вне if'а, чтобы он сохранился и попал в ask_cfg_param
	if (file_from == stdin)
	{
		sprintf(buffer, "\nВведите максимальную длину слов для запоминания моделью (не менее 1, не более %d): ", MAX_AVAILABLE_WORD_LENGTH);
		ask_msg = buffer;
	}
	int* max_word_length = ask_cfg_param(file_from, path_to_cfg,
										 read_int_param, verify_max_word_length,
										 cfg, "МАКСИМАЛЬНАЯ ДЛИНА ЗАПОМИНАЕМЫХ СЛОВ", ask_msg);
	return max_word_length;
}

static Pointer ARV_size_tol(FILE* file_from, const char* path_to_cfg, Pointer* cfg)
{
	char* ask_msg = NULL;
	char buffer[BUFFER_SIZE];
	// приходится выделять буфер вне if'а, чтобы он сохранился и попал в ask_cfg_param
	if (file_from == stdin)
	{
		sprintf(buffer, "\nМодель ищет лучшую замену из выученных слов длиной +-size_tol от заменяемого слова\nВведите size_tol (не менее 0, не более %d): ", MAX_AVAILABLE_MISSES);
		ask_msg = buffer;
	}
	int* size_tol = ask_cfg_param(file_from, path_to_cfg,
								 read_int_param, verify_size_tol,
								 cfg, "МАКСИМАЛЬНОЕ ОТКЛОНЕНИЕ ДЛИНЫ ЗАМЕНЯЮЩЕГО СЛОВА", ask_msg);
	return size_tol;
}

static Pointer ARV_threshold(FILE* file_from, const char* path_to_cfg, Pointer* cfg)
{
	char* ask_msg = NULL;
	char buffer[BUFFER_SIZE];
	// приходится выделять буфер вне if'а, чтобы он сохранился и попал в ask_cfg_param
	if (file_from == stdin)
	{
		sprintf(buffer, "\nМодель заменяет слово, если лучшая замена отличается не более, чем в threshold символах.\nВведите threshold (не менее 0, не более %d): ", MAX_AVAILABLE_ERRORS);
		ask_msg = buffer;
	}
	int* threshold = ask_cfg_param(file_from, path_to_cfg,
								   read_int_param, verify_threshold,
								   cfg, "МАКСИМАЛЬНОЕ ОТКЛОНЕНИЕ В ПРАВОПИСАНИИ СЛОВА", ask_msg);
	return threshold;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static int* read_mode(FILE* file_from)
{
	char buff_for_mode[BUFFER_SIZE];
	int read_result = (file_from == stdin) ? read_param_from_console(buff_for_mode, BUFFER_SIZE) : read_param_from_file(file_from, buff_for_mode, BUFFER_SIZE);
	// Если параметр успешно получен - проверяем его на корректность с точки зрения существования такого режима. Если всё хорошо - возвращаем номер режима
	// Если параметр некорректный или произошла ошибка EXIT_USER_FAILURE, то возвращаем значение WRONG_MODE. Если произошла EXIT_MEMORY_FAILURE, возвращаем NULL
	int* mode = NULL;
	if (read_result != EXIT_MEMORY_FAILURE)  // выделилась память под буфер и этой памяти хватило, чтобы записать параметр
	{
		mode = (int*)retry_malloc(sizeof(int), MAX_MALLOC_ATTEMPTS);
		if (mode != NULL)
		{
			*mode = WRONG_MODE;
			char* available_modes[] = { "train_new", "train_existed", "edit" };
			for (int i = 0; i < MODES_AMOUNT && *mode == WRONG_MODE; ++i)  // перебираем возможные варианты режимов и ищем совпадение
				*mode = (strcmp(available_modes[i], buff_for_mode) == 0) ? i : WRONG_MODE;
		}		
	}
	return mode;
}

static char* read_path_param(FILE* file_from)//---------------------------------------------------------------------------------------------
{
	char buff_for_path[BUFFER_SIZE];
	int read_result = (file_from == stdin) ? read_param_from_console(buff_for_path, BUFFER_SIZE) : read_param_from_file(file_from, buff_for_path, BUFFER_SIZE);
	// Если параметр успешно получен - выделяем под него память. Если память выделилась - возвращаем указатель на строку
	// Если при чтении параметра произошла ошибка EXIT_USER_FAILURE, то возвращаем значение WRONG_PATH. Если произошла EXIT_MEMORY_FAILURE, возвращаем NULL
	if (read_result == EXIT_USER_FAILURE)
		strcpy(buff_for_path, WRONG_PATH);  // записать wrong_path в буффер, это дальше не пройдёт проверку path_is_txt в verify
	char* path = NULL;
	if (read_result != EXIT_MEMORY_FAILURE)
	{
		size_t path_size = strlen(buff_for_path) + 1; // не забываем про терминирующий нуль
		path = (char*)retry_malloc(sizeof(char) * path_size, MAX_MALLOC_ATTEMPTS);
		if (path != NULL)
			strcpy(path, buff_for_path);
	}
	return path;
}

static int* read_int_param(FILE* file_from)
{
	char buff_for_int[BUFFER_SIZE];
	int read_result = (file_from == stdin) ? read_param_from_console(buff_for_int, BUFFER_SIZE) : read_param_from_file(file_from, buff_for_int, BUFFER_SIZE);
	// Если параметр успешно получен - проверяем его на корректность (что это действительно int). Если всё хорошо - возвращаем указатель на число
	// Если параметр некорректный или произошла ошибка EXIT_USER_FAILURE, то возвращаем значение WRONG_PARAM. Если произошла EXIT_MEMORY_FAILURE, возвращаем NULL
	int* number = NULL;
	if (read_result != EXIT_MEMORY_FAILURE)  // буфера хватило
	{
		number = (int*)retry_malloc(sizeof(int), MAX_MALLOC_ATTEMPTS);
		if (number != NULL && (read_int_param == EXIT_USER_FAILURE || is_integer(buff_for_int) == false))
			*number = WRONG_PARAM;
		else if (number != NULL)
			*number = atoi(buff_for_int);
	}
	return number;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static bool verify_mode(const int* mode)
{
	return (mode == NULL || (*mode >= 0 && *mode <= (MODES_AMOUNT - 1))) ? true : false;
}

static bool verify_path_to_new(const char* path_to_new)
{
	bool correct_path = path_is_txt(path_to_new);
	if (correct_path)
	{
		bool file_created = file_create(path_to_new, NULL);  // файла с таким именем ещё не существует и мы создаём новый
		correct_path *= file_created;
	}
	correct_path += (path_to_new == NULL);
	return correct_path;
}

static bool verify_path_to_existed(const char* path_to_existed)
{
	bool correct_path = path_is_txt(path_to_existed);
	if (correct_path)
		correct_path = file_exists(path_to_existed);
	correct_path += (path_to_existed == NULL);
	return correct_path;
}

static bool verify_max_word_length(const int* max_word_length)
{
	return (max_word_length == NULL || (*max_word_length >= 1 && *max_word_length <= MAX_AVAILABLE_WORD_LENGTH)) ? true : false;
}

static bool verify_size_tol(const int* size_tol)
{
	return (size_tol == NULL || (*size_tol >= 0 && *size_tol <= MAX_AVAILABLE_MISSES)) ? true : false;
}

static bool verify_threshold(const int* threshold)
{
	return (threshold == NULL || (*threshold >= 0 && *threshold <= MAX_AVAILABLE_ERRORS)) ? true : false;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/