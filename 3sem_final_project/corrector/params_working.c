#include "constants.h"
#include "params_working.h"
#include "helpful_functions.h"
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>


// Определить способ установки конфигурации работы программы
static int define_set_way(int argc, char** argv);

// Вывести справку
static void print_help();

// Определить конфигурацию по параметрам. params_amount - сколько параметров из консоли попадёт в функцию (вдруг их недостаточно)
static Pointer* cfg_from_params(int params_amount, char** params_for_cfg);

// Ввести конфигурацию в консоль
static Pointer* cfg_step_by_step();

// Считать конфигурацию из файла. params_amount - сколько параметров из консоли попадёт в функцию (вдруг определён режим from_file, но путь не задан)
static Pointer* cfg_from_file(int params_amount, char** ptr_to_path);

// Проверить что params_exist параметров не меньше, чем требуется (params_required).
// Если недостаточно параметров, написать сообщение exit_msg, вывести справку и выйти
static void sufficiently_params_check(int params_required, int params_exist, const char* exit_msg);

// Написать сообщение exit_msg, вывести справку и выйти
static void exit_soft(const char* exit_msg);


Pointer* set_cfg(int argc, char** argv)
{
	setlocale(LC_ALL, "ru");

	int set_way = define_set_way(argc, argv);
	if (set_way == PRINT_HELP)
	{
		print_help();
		exit(EXIT_SUCCESSFULLY);
	}
	else
	{
		Pointer* cfg = NULL;
		if (set_way == CFG_FROM_PARAMS)
			cfg = cfg_from_params(argc - 2, argv + 2);  // пропускаем путь к программе и способ задания конфига
		else if (set_way == CFG_STEP_BY_STEP)
			cfg = cfg_step_by_step();
		else if (set_way == CFG_FROM_FILE)
			cfg = cfg_from_file(argc - 2, argv + 2);  // пропускаем путь к программе и способ задания конфига
		return cfg;
	}
}

void delete_cfg(int mode, Pointer* cfg)//---------------------------------------------------------------------------------------------
{
	printf("Очистка конфига\n");
}


static int define_set_way(int argc, char** argv)
{
	int set_way = WRONG_PARAM;
	if (argc == 1) // единственный параметр - путь к исполняемому файлу; просим пользователя ввести конфигурацию в консоль
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
		exit_soft("Такой возможности установки конфигурации не предусмотрено.\n");

	return set_way;
}

static void print_help()//---------------------------------------------------------------------------------------------
{
	printf("HELP!\n");
}

static Pointer* cfg_from_params(int params_amount, char** params_for_cfg)//---------------------------------------------------------------------------------------------
{
	sufficiently_params_check(1, params_amount, "В параметрах отсуствует режим работы программы\n");
	/**/
	return NULL;
}

static Pointer* cfg_step_by_step()//---------------------------------------------------------------------------------------------
{
	return NULL;
}

static Pointer* cfg_from_file(int params_amount, char** ptr_to_path)//---------------------------------------------------------------------------------------------
{
	sufficiently_params_check(1, params_amount, "Не задан путь к файлу с конфигурацией программы\n");

	/**/

	return NULL;
}

static void sufficiently_params_check(int params_required, int params_exist, const char* exit_msg)
{
	if (params_exist < params_required)
		exit_soft(exit_msg);
}

static void exit_soft(const char* exit_msg)
{
	printf("%s\n", exit_msg);
	print_help();
	exit(EXIT_SOFT_FAILURE);
}