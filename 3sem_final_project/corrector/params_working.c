#include "params_working.h"
#include "constants.h"
#include "helpful_functions.h"
#include "files_working.h"
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <direct.h>


// Определить способ установки конфигурации работы программы
static int define_set_way(int argc, char** argv);


// Определить конфигурацию по параметрам при запуске. params_amount - сколько параметров из консоли попадёт в функцию (вдруг их недостаточно)
// Все параметры записываем во временный файл TMP_PARAMS_FILE и вызываем cfg_from_file(TMP_PARAMS_FILE)
// ВСЕ параметры записываем в кавычках, т.к. во всех функциях read_... кавычки удаляются, т.к. их нет ни в каких параметрах. Это может быть полезно, если путь содержит пробелы
// В случае ошибок работы с памятью возвращается NULL.
// В случае ошибок работы с файлом программа мгновенно завершается с сообщением об ошибке. Возможны две ошибки с файлами:
// 1. Ошибка создания TMP_PARAMS_FILE. Тогда временная память ещё не выделялась и чистить нечего
// 2. Ошибка открытия файла TMP_PARAMS_FILE в ходе работы cfg_from_file
// Т.к. память под параметры не выделялась до открытия, то чистить нечего, разве что надо удалить TMP_PARAMS_FILE в cfg_from_file
static Pointer* cfg_from_params(int params_amount, const char** params_for_cfg);

// Ввести конфигурацию в консоль
// В случае ошибок работы с памятью возвращается NULL
static Pointer* cfg_step_by_step();

// Считать конфигурацию из файла path_to_cfg
// Параметры в файле должны идти через пробел. ВСЕ параметры должны быть заключены в кавычки. Это сделано для удобства функций чтения и записи
// В случае ошибок работы с памятью возвращается NULL
// В случае ошибок работы с файлом программа мгновенно завершается с сообщением об ошибке. Возможна единственная ошибка при работе с файлом - ошибка открытия файла с конфигурацией
// Т.к. память под параметры не выделялась до открытия, то чистить нечего. Разве что надо удалить TMP_PARAMS_FILE, если это был временный файл (параметры получены при запуске)
static Pointer* cfg_from_file(const char* path_to_cfg);


// Проверить в set_cfg, что весь cfg заполнен (нет NULL указателей).
// Это может быть не так, если под какой-то параметр не выделилась память. В таком случае чистим остальные параметры и завершаем программу
static void check_cfg_filled(Pointer* cfg);

// Проверить что params_exist параметров не меньше, чем требуется (params_required).
// Если недостаточно параметров, написать сообщение exit_msg, вывести справку и выйти
static bool sufficiently_params_check(int params_required, int params_exist, const char* exit_msg);

// Создать файл file_name и открыть его в режиме open_mode. Используется для создания временного файла с параметрами для конфигурации в cfg_from_params.
// В случае ошибки создания или открытия выход из программы с кодом EXIT_FILE_FAILURE. Иначе возвращаем указатель на открытый файл
// file_name - ОТНОСИТЕЛЬНЫЙ ПУТЬ!!!
static FILE* create_and_open_tmp(const char* tmp_file_name, const char* open_mode);

// Пробует открыть файл с параметрами конфига
// В случае успешного открытия возвращает указатель на открытый файл
// В случае ошибки при открытии файла выходим из программы, т.к. память ещё нигде не выделялась
// Перед выходом делаем проверку, если вдруг это созданный автоматически временный файл, то сначала удалить его и только потом выйти
static FILE* cfg_file_open(const char* path_to_cfg);

// Если file_if_opened != NULL и != stdin, то закрыть файл.
// Проверить, является ли файл path_to_cfg временным файлом с конфигурацией. Если да, то удалить его.
static void delete_tmp_close_others(const char* path_to_cfg, FILE* file_if_opened);


// Считать параметр из потока file_from с помощью функции read_func, возвращающей указатель на параметр, память под который выделена динамически
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


// Считывает режим из потока file. Выделяет под него динамически память и возвращает указатель на номер режима.
// Если память не выделилась - вернёт NULL. Предполагается что файл уже открыт
static int* read_mode(FILE* file);

// Считывает путь сохранения модели из потока file. Выделяет под него динамически память и возвращает указатель на строку.
// Если память не выделилась - вернёт NULL. Предполагается что файл уже открыт
static char* read_path_to_save(FILE* file);

// Считывает путь к обучающему тексту из потока file. Выделяет под него динамически память и возвращает указатель на строку.
// Если память не выделилась - вернёт NULL. Предполагается что файл уже открыт
static char* read_path_to_text(FILE* file);

// Считывает максимальную длину изучаемых слов из потока file. Выделяет динамически память под параметр и возвращает указатель.
// Если память не выделилась - вернёт NULL. Предполагается что файл уже открыт
static int* read_max_word_length(FILE* file);

// Проверяет, что введённый пользователем режим существует
static bool verify_mode(const int* mode_by_user);

// Проверяет, что пользователь ввёл корректный путь для сохранения модели
static bool verify_path_to_save(const char* path_to_save_by_user);

// Проверяет, что пользователь ввёл корректный путь к обучающему тексту
static bool verify_path_to_text(const char* path_to_save_by_user);

// Проверяет, что пользователь ввёл корректную максимальную длину изучаемых слов
static bool verify_max_word_length(const int* path_to_save_by_user);


/*=================================================================================================================================================================================*/
Pointer* set_cfg(int argc, char** argv)
{
	setlocale(LC_ALL, "ru");
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
			cfg = cfg_from_params(argc - 2, argv + 2);  // пропускаем путь к программе и способ задания конфига
			break;
		case CFG_STEP_BY_STEP:
			cfg = cfg_step_by_step();
			break;
		case CFG_FROM_FILE:
			sufficiently_params_check(1, argc - 2, "Не задан путь к файлу с конфигурацией программы\n");  // проверка, что путь к файлу с конфигурацией был введён
			cfg = cfg_from_file(argv[2]);  // пропускаем путь к программе и способ задания конфига
			break;
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
			free(cfg[i]);
	}
	free(cfg);
}
/*=================================================================================================================================================================================*/


static int define_set_way(int argc, char** argv)
{
	int set_way = WRONG_PARAM;
	if (argc == 1) // единственный параметр - путь к исполняемой программе; просим пользователя ввести конфигурацию в консоль
		set_way = CFG_STEP_BY_STEP;
	else
	{
		if (strcmp("params", argv[1]) == 0)
			set_way = CFG_FROM_PARAMS;
		else if (strcmp("from_file", argv[1]) == 0)
			set_way = CFG_FROM_FILE;
		else if (strcmp("step_by_step", argv[1]) == 0)
			set_way = CFG_STEP_BY_STEP;
		else if (strcmp("help", argv[1]) == 0)
			set_way = PRINT_HELP;
	}
	if (set_way == WRONG_PARAM)  // первый параметр введён неправильно; даём об этом знать, выводим справку и завершаем работу программы
		exit_with_msg("Такой возможности установки конфигурации не предусмотрено.\n", EXIT_USER_FAILURE);

	return set_way;
}


static Pointer* cfg_from_params(int params_amount, const char** params_for_cfg)
{
	sufficiently_params_check(1, params_amount, "В параметрах отсуствует режим работы программы\n");
	// Если есть хоть один параметр, то создаём временный файл, записываем их туда и считываем функцией cfg_from_file
	FILE* tmp_file = create_and_open_tmp(TMP_PARAMS_FILE, "w"); // если всё прошло без ошибок - вернётся указатель на открытый файл. В случае ошибки программа завершается
	bool successfully_written = file_write(tmp_file, params_amount, params_for_cfg, "\"", " ", NULL);
	fclose(tmp_file);
	Pointer* cfg = NULL;
	if (successfully_written)
		cfg = cfg_from_file(TMP_PARAMS_FILE); // если файл не откроется, это обработается в cfg_from_file; если произойдёт ошибка памяти, вернётся NULL
	delete_tmp_close_others(TMP_PARAMS_FILE, NULL);
	return cfg;
}

static Pointer* cfg_step_by_step()
{
	printf("Конфигурация работы программы не задана. Введите необходимые параметры в консоль\n\n");
	// Формируем запрос на ввод режима работы
	char str_1[BUFFER_SIZE] = "Режим работы корректора:\n";
	char str_2[] = "\t\"train_new\" - обучение новой модели\n\t\"train_existed\" - дообучение существующей модели\n\t\"edit\" - редактирование текста\n";
	char str_3[] = "Введите режим: ";
	// Получаем режим работы
	int* mode = ask_cfg_param(stdin, NULL,
		read_mode, verify_mode, 
		NULL, "РЕЖИМ РАБОТЫ ПРОГРАММЫ", strcat(strcat(str_1, str_2), str_3));
	if (mode == NULL)  // не выделилась память под режим
		exit_with_msg(APOLOGY_MEMORY_MSG, EXIT_MEMORY_FAILURE);
	// В зависимости от режима проводим дальнейшую настройку конфигурации (указатель на режим передаётся, чтобы тоже записать его в cfg)
	Pointer* cfg = NULL;
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

static Pointer* cfg_from_file(const char* path_to_cfg)//---------------------------------------------------------------------------------------------
{
	FILE* cfg_file = cfg_file_open(path_to_cfg); // если файл не откроется, произойдёт выход внутри cfg_file_open
	// Получаем режим работы
	int* mode = ask_cfg_param(cfg_file, path_to_cfg,
		read_mode, verify_mode,
		NULL, "РЕЖИМ РАБОТЫ ПРОГРАММЫ", NULL);
	if (mode == NULL)  // не выделилась память под режим; закрываем файл с конфигурацией (если он временный, то ещё и удаляем) и выходим из программы
	{
		delete_tmp_close_others(path_to_cfg, cfg_file);
		exit_with_msg(APOLOGY_MEMORY_MSG, EXIT_MEMORY_FAILURE);
	}
	// В зависимости от режима проводим дальнейшую настройку конфигурации (указатель на режим передаётся, чтобы тоже записать его в cfg)
	Pointer* cfg = NULL;
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


static void check_cfg_filled(Pointer* cfg)
{
	bool cfg_filled = true;
	if (cfg == NULL || cfg[0] == NULL)
		cfg_filled = false;
	else
	{
		int possible_cfg_sizes[] = { TRAIN_NEW_CFG_SIZE, TRAIN_EXISTED_CFG_SIZE, EDIT_CFG_SIZE };
		int mode = *(int*)cfg[0];  // режим работы программы
		int cfg_size = possible_cfg_sizes[mode];  // по режиму работы определяем количество параметров конфигурации
		for (int i = 1; i < cfg_size; ++i)
		{
			if (cfg[i] == NULL)
				cfg_filled = false;
		}
	}
	if (!cfg_filled)
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

static FILE* create_and_open_tmp(const char* tmp_file_name, const char* open_mode)
{
	bool tmp_created = file_create(tmp_file_name, NULL);
	FILE* tmp_file = NULL;
	if (tmp_created == false) // память ещё нигде не выделялась, можно спокойно выйти из программы
		exit_with_msg("Ошибка создания временного файла.\nСожалеем и приносим извинения за доставленные неудобства!\nПовторите попытку\n", EXIT_FILE_FAILURE);
	else
	{
		tmp_file = fopen(tmp_file_name, open_mode);
		if (tmp_file == NULL) // временный файл создался, но почему-то не открылся; удаляем его и выходим
		{
			delete_tmp_close_others(tmp_file_name, NULL);
			exit_with_msg("Ошибка открытия временного файла.\nСожалеем и приносим извинения за доставленные неудобства!\nПовторите попытку\n", EXIT_FILE_FAILURE);
		}
	}
	return tmp_file;
}

static FILE* cfg_file_open(const char* path_to_cfg)
{
	FILE* cfg_file = fopen(path_to_cfg, "r");
	if (cfg_file == NULL) // файл с конфигурацией не открылся
	{
		delete_tmp_close_others(path_to_cfg, NULL); // удаляем, если это был временный файл
		exit_with_msg("Ошибка открытия файла с параметрами.\nСожалеем и приносим извинения за доставленные неудобства!\nПовторите попытку\n", EXIT_FILE_FAILURE);
	}
	return cfg_file;
}

static void delete_tmp_close_others(const char* path_to_cfg, FILE* file_if_opened)
{
	if (file_if_opened != NULL && file_if_opened != stdin) // закрыть файл, если нужно (консоль не закрываем)
		fclose(file_if_opened);
	if (path_to_cfg != NULL && strstr(path_to_cfg, TMP_PARAMS_FILE) != NULL) // это был временный файл с параметрами при запуске; надо удалить файл
	{
		if (file_remove(path_to_cfg, "Ошибка удаления временного файла. Удалите его сами при необходимости:\n") == false) // не получилось удалить временный файл
			free(rel_to_abs_way(TMP_PARAMS_FILE, true, true)); // напечатаем пользователю об этом и предложим удалить, предоставив абсолютный путь
	}
}


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
		delete_cfg(cfg);
		delete_tmp_close_others(path_to_cfg, file_from);
		exit_with_msg("Параметр %s введён некорректно! Прочитайте справку и повторите попытку\n", EXIT_USER_FAILURE);
	}
	return param; // пользователь справился))). Ну или не выделилась память...
}

static Pointer* ask_train_new_cfg(FILE* file_from, const char* path_to_cfg, int* mode)
{
	Pointer* train_new_cfg = empty_pointers_array(TRAIN_NEW_CFG_SIZE);
	if (train_new_cfg != NULL)  // если память под cfg выделилась, заполняем его параметрами (не забыв указать на режим)
	{
		// Запоминаем режим
		train_new_cfg[0] = mode;

		// Получаем путь для сохранения модели
		char str_1[BUFFER_SIZE] = "\nОбученная модель должна быть сохранена в файл В ФОРМАТЕ .TXT\nТекущая директория:\n";
		char* current_path = rel_to_abs_way(NULL, false, true);
		char str_2[BUFFER_SIZE] = "\nВведите абсолютный или относительный путь для сохранения модели в формате path\\model_name.txt: ";
		train_new_cfg[1] = ask_cfg_param(file_from, path_to_cfg,
			read_path_to_save, verify_path_to_save,
			train_new_cfg, "ПУТЬ ДЛЯ СОХРАНЕНИЯ МОДЕЛИ", strcat(strcat(str_1, current_path), str_2));

		// Получаем путь к обучающему тексту
		strcpy(str_1, "\nМодель умеет учить слова, состоящие ТОЛЬКО из русских и английских букв.\nТекущая директория:\n");
		strcpy(str_2, "\nВведите абсолютный или относительный путь к обучающему тексту в ФОРМАТЕ .TXT в виде path\\training_text_name.txt: ");
		train_new_cfg[2] = ask_cfg_param(file_from, path_to_cfg,
			read_path_to_text, verify_path_to_text,
			train_new_cfg, "ПУТЬ К ОБУЧАЮЩЕМУ ТЕКСТУ", strcat(strcat(str_1, current_path), str_2));

		// Получаем максимальную длину запоминаемых слов
		sprintf(str_1, "\nМодель хранит слова одинаковой длины вместе.\nВведите максимальную длину слов для запоминания (не более %d): ", MAX_AVAILABLE_WORD_LENGTH);
		train_new_cfg[3] = ask_cfg_param(file_from, path_to_cfg,
			read_max_word_length, verify_max_word_length,
			train_new_cfg, "МАКСИМАЛЬНАЯ ДЛИНА ЗАПОМИНАЕМЫХ СЛОВ", str_1);
		free(current_path);
	}
	return train_new_cfg;
}

static Pointer* ask_train_existed_cfg(FILE* file_from, const char* path_to_cfg, int* mode)//---------------------------------------------------------------------------------------------
{
	return NULL;
}

static Pointer* ask_edit_cfg(FILE* file_from, const char* path_to_cfg, int* mode)//---------------------------------------------------------------------------------------------
{
	return NULL;
}

static int* read_mode(FILE* file)//---------------------------------------------------------------------------------------------
{
	// добавить wrong_mode
	int* dummy_mode = malloc(sizeof(int));
	*dummy_mode = 0;
	return dummy_mode;
}

static char* read_path_to_save(FILE* file)//---------------------------------------------------------------------------------------------
{
	char* hel = malloc(sizeof(char) * 6);
	strcpy(hel, "hello");
	return hel;
}

static char* read_path_to_text(FILE* file)//---------------------------------------------------------------------------------------------
{
	char* hel = malloc(sizeof(char) * 6);
	strcpy(hel, "hello");
	return hel;
}

static int* read_max_word_length(FILE* file)//---------------------------------------------------------------------------------------------
{
	int* dummy_length = malloc(sizeof(int));
	*dummy_length = 10;
	return dummy_length;
}


static bool verify_mode(const int* mode_by_user)//---------------------------------------------------------------------------------------------
{
	// в случае ошибки памяти возвращаем true. тогда из функции ask_cfg_param вернёться тоже NULL и в set_cfg мы поймём, что произошла ошибка памяти
	return true;
}

static bool verify_path_to_save(const char* path_to_save_by_user)//---------------------------------------------------------------------------------------------
{
	return true;
}

static bool verify_path_to_text(const char* path_to_save_by_user)//---------------------------------------------------------------------------------------------
{
	return true;
}

static bool verify_max_word_length(const int* path_to_save_by_user)//---------------------------------------------------------------------------------------------
{
	return true;
}