#include "params_working.h"
#include "constants.h"
#include "helpful_functions.h"
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>


// ���������� ������ ��������� ������������ ������ ���������
static int define_set_way(int argc, char** argv);

// ���������� ������������ �� ����������. params_amount - ������� ���������� �� ������� ������ � ������� (����� �� ������������)
static Pointer* cfg_from_params(int params_amount, char** params_for_cfg);

// ������ ������������ � �������
static Pointer* cfg_step_by_step();

// ������� ������������ �� �����. params_amount - ������� ���������� �� ������� ������ � ������� (����� �������� ����� from_file, �� ���� �� �����)
static Pointer* cfg_from_file(int params_amount, char** ptr_to_path);

// ��������� ��� params_exist ���������� �� ������, ��� ��������� (params_required).
// ���� ������������ ����������, �������� ��������� exit_msg, ������� ������� � �����
static void sufficiently_params_check(int params_required, int params_exist, const char* exit_msg);

// ������ ����� ����� �������
static int ask_mode();

// ������ ������������ ��� �������� ����� ������ ����� �������
static Pointer* ask_train_new_cfg();

// ������ ������������ ��� �������� ������������ ������ ����� �������
static Pointer* ask_train_existed_cfg();

// ������ ������������ ��� �������������� ������ ����� �������
static Pointer* ask_edit_cfg();

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
		switch (set_way)
		{
		case CFG_FROM_PARAMS:
			cfg = cfg_from_params(argc - 2, argv + 2);  // ���������� ���� � ��������� � ������ ������� �������
			break;
		case CFG_STEP_BY_STEP:
			cfg = cfg_step_by_step();
			break;
		case CFG_FROM_FILE:
			cfg = cfg_from_file(argc - 2, argv + 2);  // ���������� ���� � ��������� � ������ ������� �������
			break;
		}			
		return cfg;
	}
}

void delete_cfg(Pointer* cfg)
{
	int possible_cfg_sizes[] = { TRAIN_NEW_CFG_SIZE, TRAIN_EXISTED_CFG_SIZE, EDIT_CFG_SIZE };
	int mode = *(int*)cfg[0];
	int cfg_size = possible_cfg_sizes[mode];
	for (int i = 0; i < cfg_size; ++i)
		free(cfg[i]);

	free(cfg);
}


static int define_set_way(int argc, char** argv)
{
	int set_way = WRONG_PARAM;
	if (argc == 1) // ������������ �������� - ���� � ������������ �����; ������ ������������ ������ ������������ � �������
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
	if (set_way == WRONG_PARAM)  // ������ �������� ����� �����������; ��� �� ���� �����, ������� ������� � ��������� ������ ���������
		exit_soft("����� ����������� ��������� ������������ �� �������������.\n");

	return set_way;
}

static Pointer* cfg_from_params(int params_amount, char** params_for_cfg)//---------------------------------------------------------------------------------------------
{
	sufficiently_params_check(1, params_amount, "� ���������� ���������� ����� ������ ���������\n");
	/**/
	return NULL;
}

static Pointer* cfg_step_by_step()
{
	int mode = ask_mode();
	Pointer* cfg = NULL;
	switch (mode)
	{
	case TRAIN_NEW_MODE:
		cfg = ask_train_new_cfg();
		break;
	case TRAIN_EXISTED_MODE:
		cfg = ask_train_existed_cfg();
		break;
	case EDIT_MODE:
		cfg = ask_edit_cfg();
		break;
	}
	return cfg;
}

static Pointer* cfg_from_file(int params_amount, char** ptr_to_path)//---------------------------------------------------------------------------------------------
{
	sufficiently_params_check(1, params_amount, "�� ����� ���� � ����� � ������������� ���������\n");

	/**/

	return NULL;
}

static void sufficiently_params_check(int params_required, int params_exist, const char* exit_msg)
{
	if (params_exist < params_required)
		exit_soft(exit_msg);
}

static int ask_mode()//---------------------------------------------------------------------------------------------
{
	return 0;
}

static Pointer* ask_train_new_cfg()//---------------------------------------------------------------------------------------------
{
	Pointer* dummy_cfg = (Pointer*)malloc(sizeof(Pointer) * TRAIN_NEW_CFG_SIZE);
	int* mode = (int*)malloc(sizeof(int));
	*mode = 0;
	dummy_cfg[0] = mode;
	char* path_for_save = (char*)malloc(6 * sizeof(char));
	const char* hello_phrase = "hello";
	strcpy(path_for_save, hello_phrase);
	dummy_cfg[1] = path_for_save;
	char* path_to_text = (char*)malloc(6 * sizeof(char));
	strcpy(path_to_text, hello_phrase);
	dummy_cfg[2] = path_to_text;
	int* max_word_size = (int*)malloc(sizeof(int));
	*max_word_size = 20;
	dummy_cfg[3] = max_word_size;

	return dummy_cfg;
}

static Pointer* ask_train_existed_cfg()//---------------------------------------------------------------------------------------------
{
	return NULL;
}

static Pointer* ask_edit_cfg()//---------------------------------------------------------------------------------------------
{
	return NULL;
}