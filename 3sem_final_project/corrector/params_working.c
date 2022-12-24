#include "params_working.h"
#include "constants.h"
#include "helpful_functions.h"
#include "files_working.h"
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <direct.h>


// ���������� ������ ��������� ������������ ������ ���������
static int define_set_way(int argc, char** argv);

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// ���������� ������������ �� ���������� ��� �������. params_amount - ������� ���������� �� ������� ������ � ������� (����� �� ������������)
// ��� ��������� ���������� �� ��������� ���� TMP_PARAMS_FILE � �������� cfg_from_file(TMP_PARAMS_FILE)
// ��� ��������� ���������� � ��������, �.�. �� ���� �������� read_... ������� ���������, �.�. �� ��� �� � ����� ����������. ��� ����� ���� �������, ���� ���� �������� �������
// � ������ ������ ������ � ������� ������������ NULL.
// � ������ ������ ������ � ������ ��������� ��������� ����������� � ���������� �� ������. �������� ��� ������ � �������:
// 1. ������ �������� TMP_PARAMS_FILE. ����� ��������� ������ ��� �� ���������� � ������� ������
// 2. ������ �������� ����� TMP_PARAMS_FILE � ���� ������ cfg_from_file
// �.�. ������ ��� ��������� �� ���������� �� ��������, �� ������� ������, ����� ��� ���� ������� TMP_PARAMS_FILE � cfg_from_file
static Pointer* cfg_from_params(int params_amount, const char** params_for_cfg);

// ������ ������������ � �������
// � ������ ������ ������ � ������� ������������ NULL
static Pointer* cfg_step_by_step();

// ������� ������������ �� ����� path_to_cfg
// ��������� � ����� ������ ���� ����� ������. ��� ��������� ������ ���� ��������� � �������. ��� ������� ��� �������� ������� ������ � ������
// � ������ ������ ������ � ������� ������������ NULL
// � ������ ������ ������ � ������ ��������� ��������� ����������� � ���������� �� ������. �������� ������������ ������ ��� ������ � ������ - ������ �������� ����� � �������������
// �.�. ������ ��� ��������� �� ���������� �� ��������, �� ������� ������. ����� ��� ���� ������� TMP_PARAMS_FILE, ���� ��� ��� ��������� ���� (��������� �������� ��� �������)
static Pointer* cfg_from_file(const char* path_to_cfg);

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// ��������� � set_cfg, ��� ���� cfg �������� (��� NULL ����������).
// ��� ����� ���� �� ���, ���� ��� �����-�� �������� �� ���������� ������. � ����� ������ ������ ��������� ��������� � ��������� ���������
static void check_cfg_filled(Pointer* cfg);

// ��������� ��� params_exist ���������� �� ������, ��� ��������� (params_required).
// ���� ������������ ����������, �������� ��������� exit_msg, ������� ������� � �����
static bool sufficiently_params_check(int params_required, int params_exist, const char* exit_msg);

// ������� ���� file_name � ������� ��� � ������ open_mode. ������������ ��� �������� ���������� ����� � ����������� ��� ������������ � cfg_from_params.
// � ������ ������ �������� ��� �������� ����� �� ��������� � ����� EXIT_FILE_FAILURE. ����� ���������� ��������� �� �������� ����
// file_name - ������������� ����!!!
static FILE* create_and_open_tmp(const char* tmp_file_name, const char* open_mode);

// ������� ������� ���� � ����������� �������
// � ������ ��������� �������� ���������� ��������� �� �������� ����
// � ������ ������ ��� �������� ����� ������� �� ���������, �.�. ������ ��� ����� �� ����������
// ����� ������� ������ ��������, ���� ����� ��� ��������� ������������� ��������� ����, �� ������� ������� ��� � ������ ����� �����
static FILE* cfg_file_open(const char* path_to_cfg);

// ���� file_if_opened != NULL � != stdin, �� ������� ����.
// ���������, �������� �� ���� path_to_cfg ��������� ������ � �������������. ���� ��, �� ������� ���.
static void delete_tmp_close_others(const char* path_to_cfg, FILE* file_if_opened);

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// ������� �������� �� ������ file_from � ������� ������� read_func, ������������ ��������� �� ��������, ������ ��� ������� �������� �����������
// ������������ ��������� ��������� ����������� �������� verification, ����������� ��������� �� ��������
// ���� ������� ����������� �������� NULL, �� ��� ����� ������ ���������� true. ��� �������� ������ ������ � �������, ������� ����� ���������� � ����� ����� ��������� cfg
// ���� file_from == stdin (���� ����� �������), �� ������� ������������ ������� ask_msg, � ����� ���� ��� ��������� ������� ������ �������� ���������
// ���� �������� ����������� �� �����, �� �������� ����������� ���� ��� � ����� ����������� ���
// ���� �� ������� �������� ���������� ��������, �� ����� �� ���� (��� ����� ����� param_name), ������ cfg,
// ��������� ���� file_from (������� ���, ���� ��� ��� ��������� ����; �������� � ������� ������������� ����� ����� path_to_cfg) � ��������� ������ ���������
// � ������ ������ ������ � ������� ������������ NULL. ���� �� ������ - ���������� ��������� �� ��������
static Pointer ask_cfg_param(FILE* file_from, const char* path_to_cfg,
							 Pointer (*read_func)(FILE*), bool (*verification)(Pointer), 
							 Pointer* cfg, const char* param_name, const char* ask_msg); 

// �������� �� ����� file_from ������������ ��� �������� ����� ������. ��������������, ��� ���� ��� ������. ���� ���� � �������, �� file_from == stdin
// ������������ ������������ ������ ���������� �� ��������� �������
// ����� ��� ������ � �� ������ ��������� ����������. ������ ������ ��� ���� ��� �������� � ��� ���� ���� �������� � cfg
// � ������ ������ ������ � ������� ������������ NULL
// path_to_cfg - ��������� �� ������ ����������� ������ �� ���������. ���� ��� ���������������� ���� � �������������, �� ������ ������� ���, � ���� ��������� - �� �������
static Pointer* ask_train_new_cfg(FILE* file_from, const char* path_to_cfg, int* mode);

// �������� �� ����� file_from ������������ ��� ���������� ������������ ������. ��������������, ��� ���� ��� ������. ���� ���� � �������, �� file_from == stdin
// ����� ��� ������ � �� ������ ��������� ����������. ������ ������ ��� ���� ��� �������� � ��� ���� ���� �������� � cfg
// � ������ ������ ������ � ������� ������������ NULL
// path_to_cfg - ��������� �� ������ ����������� ������ �� ���������. ���� ��� ���������������� ���� � �������������, �� ������ ������� ���, � ���� ��������� - �� �������
static Pointer* ask_train_existed_cfg(FILE* file_from, const char* path_to_cfg, int* mode);

// �������� �� ����� file_from ������������ ��� �������������� ������. ��������������, ��� ���� ��� ������. ���� ���� � �������, �� file_from == stdin
// ����� ��� ������ � �� ������ ��������� ����������. ������ ������ ��� ���� ��� �������� � ��� ���� ���� �������� � cfg
// � ������ ������ ������ � ������� ������������ NULL
// path_to_cfg - ��������� �� ������ ����������� ������ �� ���������. ���� ��� ���������������� ���� � �������������, �� ������ ������� ���, � ���� ��������� - �� �������
static Pointer* ask_edit_cfg(FILE* file_from, const char* path_to_cfg, int* mode);

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// ARV - Ask, Read, Verification. ����������� ������� ��� ������ �� file_from � ����������� ������ ������
// ���� �������� ����� ��������� - ����� ��������� (������ �������� �����������, � ������ ������ � ������� - ����� NULL). ���� �������� ����� ����������� - ����� �� ���������
static Pointer ARV_mode(FILE* file_from, const char* path_to_cfg);

// ARV - Ask, Read, Verification. ����������� ������� ��� ������ �� file_from � ����������� ���� ��� ���������� ����� ������ � ������ train_new
// ���� �������� ����� ��������� - ����� ��������� (������ �������� �����������, � ������ ������ � ������� - ����� NULL). ���� �������� ����� ����������� - ����� �� ���������
// cfg - ��� ������� ������� � ������ ������������� ����� � ������������ ���������� ������ ���������
static Pointer ARV_path_to_new_model(FILE* file_from, const char* path_to_cfg, Pointer* cfg);

// ARV - Ask, Read, Verification. ����������� ������� ��� ������ �� file_from � ����������� ���� � ������������ ������ � ������� train_existed / edit
// ���� �������� ����� ��������� - ����� ��������� (������ �������� �����������, � ������ ������ � ������� - ����� NULL). ���� �������� ����� ����������� - ����� �� ���������
// cfg - ��� ������� ������� � ������ ������������� ����� � ������������ ���������� ������ ���������
static Pointer ARV_path_to_existed_model(FILE* file_from, const char* path_to_cfg, Pointer* cfg);

// ARV - Ask, Read, Verification. ����������� ������� ��� ������ �� file_from � ����������� ���� ��� ���������� ������������������ ������ � ������ edit
// ���� �������� ����� ��������� - ����� ��������� (������ �������� �����������, � ������ ������ � ������� - ����� NULL). ���� �������� ����� ����������� - ����� �� ���������
// cfg - ��� ������� ������� � ������ ������������� ����� � ������������ ���������� ������ ���������
static Pointer ARV_path_to_new_text(FILE* file_from, const char* path_to_cfg, Pointer* cfg);

// ARV - Ask, Read, Verification. ����������� ������� ��� ������ �� file_from � ����������� ���� � ���������� (� ������ train_*) ��� �������������� (� ������ edit) ������
// ���� �������� ����� ��������� - ����� ��������� (������ �������� �����������, � ������ ������ � ������� - ����� NULL). ���� �������� ����� ����������� - ����� �� ���������
// cfg - ��� ������� ������� � ������ ������������� ����� � ������������ ���������� ������ ���������
static Pointer ARV_path_to_existed_text(FILE* file_from, const char* path_to_cfg, Pointer* cfg, int* mode);

// ARV - Ask, Read, Verification. ����������� ������� ��� ������ �� file_from � ����������� ������������ ����� ���������� ����� � ������ train_new
// ���� �������� ����� ��������� - ����� ��������� (������ �������� �����������, � ������ ������ � ������� - ����� NULL). ���� �������� ����� ����������� - ����� �� ���������
// cfg - ��� ������� ������� � ������ ������������� ����� � ������������ ���������� ������ ���������
static Pointer ARV_max_word_length(FILE* file_from, const char* path_to_cfg, Pointer* cfg);

// ARV - Ask, Read, Verification. ����������� ������� ��� ������ �� file_from � ����������� ����������� ����������� ���������� ����� ����������� ����� � ������ edit
// ���� �������� ����� ��������� - ����� ��������� (������ �������� �����������, � ������ ������ � ������� - ����� NULL). ���� �������� ����� ����������� - ����� �� ���������
// cfg - ��� ������� ������� � ������ ������������� ����� � ������������ ���������� ������ ���������
static Pointer ARV_size_tol(FILE* file_from, const char* path_to_cfg, Pointer* cfg);

// ARV - Ask, Read, Verification. ����������� ������� ��� ������ �� file_from � ����������� ����������� ���������� �������� ���� � ������ edit
// ���� �������� ����� ��������� - ����� ��������� (������ �������� �����������, � ������ ������ � ������� - ����� NULL). ���� �������� ����� ����������� - ����� �� ���������
// cfg - ��� ������� ������� � ������ ������������� ����� � ������������ ���������� ������ ���������
static Pointer ARV_threshold(FILE* file_from, const char* path_to_cfg, Pointer* cfg);

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// ��������� � ���� ������ (� �������� ��� ���) ����� �� ������ file_from. �������� ��� ���� ����������� ������ � ���������� ��������� �� ����� ������.
// ���� ������ �� ���������� - ����� NULL. �������������� ��� ���� ��� ������
static int* read_mode(FILE* file_from);

// ��������� �����-���� ���� �� ������ file_from. �������� ��� ���� ����������� ������ � ���������� ��������� �� ������ � ���� ����.
// ���� ������ �� ���������� - ����� NULL. �������������� ��� ���� ��� ������
static char* read_path_param(FILE* file_from);

// ��������� ������������� �������� (max_word_length, size_tol, threshold) �� ������ file_from. �������� ��� ���� ����������� ������ � ���������� ��������� �� int.
// ���� ������ �� ���������� - ����� NULL. �������������� ��� ���� ��� ������
// �������� ����������� ��� ������ � ������ ����� ������� ������������� ��� � �����, �.�. �� ����� ���� � ��������
static int* read_int_param(FILE* file_from);

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// ���������, ��� ������������ ��������� ��� ����� ������ (�� 0 �� (MODES_AMOUNT - 1))
static bool verify_mode(const int* mode);

// ���������, ��� ������������ ��������� ��� ���� ��� ���������� ����� ������/������������������ ������ (��������, ��� ���� ������������� �� .txt; ����� ������� ������� ����)
static bool verify_path_to_new(const char* path_to_new);

// ���������, ��� ������������ ��������� ��� ���� � ��������� ������/���������� ������/�������������� ������ (��������, ��� ���� ������������� �� .txt; ����� ������� ������� ����)
static bool verify_path_to_existed(const char* path_to_existed);

// ���������, ��� ������������ ��������� ��� ������������ ����� ���� (�� 1 �� (MAX_AVAILABLE_WORD_LENGTH - 1))
static bool verify_max_word_length(const int* max_word_length);

// ���������, ��� ������������ ��������� ��� size_tol (�� 0 �� MAX_AVAILABLE_MISSES)
static bool verify_size_tol(const int* size_tol);

// ���������, ��� ������������ ��������� ��� threshold (�� 0 �� MAX_AVAILABLE_ERRORS)
static bool verify_threshold(const int* threshold);


/*=================================================================================================================================================================================*/
Pointer* set_cfg(int argc, char** argv)
{
	setlocale(LC_ALL, "ru");
	// ���������� ������ ��������� ������������
	int set_way = define_set_way(argc, argv);
	//������������ �������� �������
	if (set_way == PRINT_HELP)
	{
		print_help();
		exit(EXIT_SUCCESSFULLY);
	}
	// ������������ ����� ��������������� ����������. ���������� ������������
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
			sufficiently_params_check(1, argc - 2, "�� ����� ���� � ����� � ������������� ���������\n");  // ��������, ��� ���� � ����� � ������������� ��� �����
			cfg = cfg_from_file(argv[2]);  // ���������� ���� � ��������� � ������ ������� �������
			break;
		}
		check_cfg_filled(cfg);
		return cfg;
	}
}

void delete_cfg(Pointer* cfg)
{
	// ���� ������ ��� ������ ���������� � ����� ��������, �� ���� ��������� ������ ��� ���������
	if (cfg != NULL && cfg[0] != NULL)
	{
		int possible_cfg_sizes[] = { TRAIN_NEW_CFG_SIZE, TRAIN_EXISTED_CFG_SIZE, EDIT_CFG_SIZE };
		int mode = *(int*)cfg[0];  // ����� ������ ���������
		int cfg_size = possible_cfg_sizes[mode];  // �� ������ ������ ���������� ���������� ���������� ������������
		for (int i = 0; i < cfg_size; ++i)
			free(cfg[i]);
	}
	free(cfg);
}
/*=================================================================================================================================================================================*/


static int define_set_way(int argc, char** argv)
{
	int set_way = WRONG_PARAM;
	if (argc == 1) // ������������ �������� - ���� � ����������� ���������; ������ ������������ ������ ������������ � �������
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
		exit_with_msg("����� ����������� ��������� ������������ �� �������������.\n", EXIT_USER_FAILURE);

	return set_way;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static Pointer* cfg_from_params(int params_amount, const char** params_for_cfg)
{
	sufficiently_params_check(1, params_amount, "� ���������� ���������� ����� ������ ���������\n");
	// ���� ���� ���� ���� ��������, �� ������ ��������� ����, ���������� �� ���� � ��������� �������� cfg_from_file
	FILE* tmp_file = create_and_open_tmp(TMP_PARAMS_FILE, "w"); // ���� �� ������ ��� ������ - ������� ��������� �� �������� ����. � ������ ������ ��������� �����������
	bool successfully_written = file_write(tmp_file, params_amount, params_for_cfg, "\"", " ", NULL);
	fclose(tmp_file);
	Pointer* cfg = NULL;
	if (successfully_written)
		cfg = cfg_from_file(TMP_PARAMS_FILE); // ���� ���� �� ���������, ��� ������������ � cfg_from_file; ���� ��������� ������ ������, ������� NULL
	delete_tmp_close_others(TMP_PARAMS_FILE, NULL);
	return cfg;
}

static Pointer* cfg_step_by_step()
{
	printf("������������ ������ ��������� �� ������. ������� ����������� ��������� � �������\n\n");
	// ������ ����� ������; ���� ������ ��� ����� �� ��������� ��� ����� ����� �� �����, �� ����� �� ������ �� ��������� (�� ����� ������� ���� ������)
	int* mode = ARV_mode(stdin, NULL);
	Pointer* cfg = NULL;
	// � ����������� �� ������ ������ ��������� ������ ������
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
	// ���� �� ���������� ������ ��� cfg, �� ������ mode �� ����������, � ������ ��� ���� ��� ��������. ������ �������!
	// ���� �� ������ ��� cfg ����������, � ��� �����-�� �������� ���, �� mode ����� ��� � cfg � ����� ����������, �������� ����� ������ �� ����
	if (cfg == NULL)
		free(mode);
	return cfg;
}

static Pointer* cfg_from_file(const char* path_to_cfg)
{
	FILE* cfg_file = cfg_file_open(path_to_cfg); // ���� ���� �� ���������, ��������� ����� ������ cfg_file_open
	// ������ ����� ������; ���� ������ ��� ����� �� ��������� ��� ����� ����� �� �����, �� ������� ����, ������ ��� (���� �� ���������) � ������ �� ��������� (������� ���� ������)
	int* mode = ARV_mode(cfg_file, path_to_cfg);
	Pointer* cfg = NULL;
	// � ����������� �� ������ ������ ��������� ������ ������
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
	// ���� �� ���������� ������ ��� cfg, �� ������ mode �� ����������, � ������ ��� ���� ��� ��������. ������ �������!
	// ���� �� ������ ��� cfg ����������, � ��� �����-�� �������� ���, �� mode ����� ��� � cfg � ����� ����������, �������� ����� ������ �� ����
	if (cfg == NULL)
		free(mode);
	fclose(cfg_file);
	return cfg;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static void check_cfg_filled(Pointer* cfg)
{
	bool cfg_filled = true;
	if (cfg == NULL || cfg[0] == NULL)
		cfg_filled = false;
	else
	{
		int possible_cfg_sizes[] = { TRAIN_NEW_CFG_SIZE, TRAIN_EXISTED_CFG_SIZE, EDIT_CFG_SIZE };
		int mode = *(int*)cfg[0];  // ����� ������ ���������
		int cfg_size = possible_cfg_sizes[mode];  // �� ������ ������ ���������� ���������� ���������� ������������
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
	if (tmp_created == false) // ������ ��� ����� �� ����������, ����� �������� ����� �� ���������
		exit_with_msg("������ �������� ���������� �����.\n�������� � �������� ��������� �� ������������ ����������!\n��������� �������\n", EXIT_FILE_FAILURE);
	else
	{
		tmp_file = fopen(tmp_file_name, open_mode);
		if (tmp_file == NULL) // ��������� ���� ��������, �� ������-�� �� ��������; ������� ��� � �������
		{
			delete_tmp_close_others(tmp_file_name, NULL);
			exit_with_msg("������ �������� ���������� �����.\n�������� � �������� ��������� �� ������������ ����������!\n��������� �������\n", EXIT_FILE_FAILURE);
		}
	}
	return tmp_file;
}

static FILE* cfg_file_open(const char* path_to_cfg)
{
	FILE* cfg_file = fopen(path_to_cfg, "r");
	if (cfg_file == NULL) // ���� � ������������� �� ��������
	{
		delete_tmp_close_others(path_to_cfg, NULL); // �������, ���� ��� ��� ��������� ����
		exit_with_msg("������ �������� ����� � �����������.\n�������� � �������� ��������� �� ������������ ����������!\n��������� �������\n", EXIT_FILE_FAILURE);
	}
	return cfg_file;
}

static void delete_tmp_close_others(const char* path_to_cfg, FILE* file_if_opened)
{
	if (file_if_opened != NULL && file_if_opened != stdin) // ������� ����, ���� ����� (������� �� ���������)
		fclose(file_if_opened);
	if (path_to_cfg != NULL && strstr(path_to_cfg, TMP_PARAMS_FILE) != NULL) // ��� ��� ��������� ���� � ����������� ��� �������; ���� ������� ����
	{
		if (file_remove(path_to_cfg, "������ �������� ���������� �����. ������� ��� ���� ��� �������������:\n") == false) // �� ���������� ������� ��������� ����
			free(rel_to_abs_way(TMP_PARAMS_FILE, true, true)); // ���������� ������������ �� ���� � ��������� �������, ����������� ���������� ����
	}
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
	if (file_from == stdin)  // ���� ���� ������������� �� �������, �� �������� ������������, ��� ��� ��������� �������
	{
		// ����������, ���� ������������ �� ����� ���������, ���� �� �������� ��� ������� ��� ���� �� ��������� ������ ������
		for (unsigned attempt = 0; attempt < MAX_USER_ATTEMPTS && correct_param == false; ++attempt)
		{
			free(param);
			printf("������������ ���� ��������� %s. ��������� �������: ", param_name);
			param = read_func(file_from);
			correct_param = verification(param);
		}
	}
	if (correct_param == false) // ������������ �� ��������� � ���������� ���������(((
	{
		free(param);
		delete_cfg(cfg);
		delete_tmp_close_others(path_to_cfg, file_from);
		printf("�������� %s ����� �����������! ���������� ������� � ��������� �������\n", param_name);
		exit_with_msg(NULL, EXIT_USER_FAILURE);
	}
	return param; // ������������ ���������))). �� ��� �� ���������� ������...
}

static Pointer* ask_train_new_cfg(FILE* file_from, const char* path_to_cfg, int* mode)
{
	Pointer* train_new_cfg = empty_pointers_array(TRAIN_NEW_CFG_SIZE);
	if (train_new_cfg != NULL)  // ���� ������ ��� cfg ����������, ��������� ��� ����������� (�� ����� ��������� �����)
	{
		// ���������� �����
		train_new_cfg[0] = mode;
		// �������� ���� ��� ���������� ������
		train_new_cfg[1] = ARV_path_to_new_model(file_from, path_to_cfg, train_new_cfg);
		// �������� ���� � ���������� ������
		train_new_cfg[2] = ARV_path_to_existed_text(file_from, path_to_cfg, train_new_cfg, mode);
		// �������� ������������ ����� ������������ ����
		train_new_cfg[3] = ARV_max_word_length(file_from, path_to_cfg, train_new_cfg);
	}
	return train_new_cfg;
}

static Pointer* ask_train_existed_cfg(FILE* file_from, const char* path_to_cfg, int* mode)//---------------------------------------------------------------------------------------------
{
	Pointer* train_existed_cfg = empty_pointers_array(TRAIN_EXISTED_CFG_SIZE);
	if (train_existed_cfg != NULL)  // ���� ������ ��� cfg ����������, ��������� ��� ����������� (�� ����� ������� �� �����)
	{
		// ���������� �����
		train_existed_cfg[0] = mode;

		//////////////// �������� ���� ��� ���������� ������
		//////////////char str_1[BUFFER_SIZE] = "\n��������� ������ ������ ���� ��������� � ���� � ������� .TXT\n������� ����������:\n";
		//////////////char* current_path = rel_to_abs_way(NULL, false, true);
		//////////////char str_2[BUFFER_SIZE] = "\n������� ���������� ��� ������������� ���� ��� ���������� ������ � ������� path\\model_name.txt: ";
		//////////////train_new_cfg[1] = ask_cfg_param(file_from, path_to_cfg,
		//////////////	read_path_to_save, verify_path_to_save,
		//////////////	train_new_cfg, "���� ��� ���������� ������", strcat(strcat(str_1, current_path), str_2));

		//////////////// �������� ���� � ���������� ������
		//////////////strcpy(str_1, "\n������ ����� ����� �����, ��������� ������ �� ������� � ���������� ����.\n������� ����������:\n");
		//////////////strcpy(str_2, "\n������� ���������� ��� ������������� ���� � ���������� ������ � ������� .TXT � ���� path\\training_text_name.txt: ");
		//////////////train_new_cfg[2] = ask_cfg_param(file_from, path_to_cfg,
		//////////////	read_path_to_text, verify_path_to_text,
		//////////////	train_new_cfg, "���� � ���������� ������", strcat(strcat(str_1, current_path), str_2));

		//////////////// �������� ������������ ����� ������������ ����
		//////////////sprintf(str_1, "\n������ ������ ����� ���������� ����� ������.\n������� ������������ ����� ���� ��� ����������� (�� ����� %d): ", MAX_AVAILABLE_WORD_LENGTH);
		//////////////train_new_cfg[3] = ask_cfg_param(file_from, path_to_cfg,
		//////////////	read_max_word_length, verify_max_word_length,
		//////////////	train_new_cfg, "������������ ����� ������������ ����", str_1);
		//////////////free(current_path);
	}
	return train_existed_cfg;
}

static Pointer* ask_edit_cfg(FILE* file_from, const char* path_to_cfg, int* mode)//---------------------------------------------------------------------------------------------
{
	return NULL;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static Pointer ARV_mode(FILE* file_from, const char* path_to_cfg)
{
	char* ask_msg = NULL;
	char str_1[BUFFER_SIZE] = "\n����� ������ ����������:\n";
	// ���������� �������� ����� ��� if'�, ����� �� ���������� � ����� � ask_cfg_param
	if (file_from == stdin)
	{
		// ��������� ������ �� ���� ������ ������
		char str_2[] = "\t\"train_new\" - �������� ����� ������\n\t\"train_existed\" - ���������� ������������ ������\n\t\"edit\" - �������������� ������\n";
		char str_3[] = "������� �����: ";
		ask_msg = strcat(strcat(str_1, str_2), str_3);
	}
	// �������� ����� ������
	int* mode = ask_cfg_param(file_from, path_to_cfg,
							  read_mode, verify_mode,
							  NULL, "����� ������ ���������", ask_msg);
	if (mode == NULL)  // �� ���������� ������ ��� �����; ��������� ���� � ������������� (���� �� ���������, �� ��� � �������) � ������� �� ���������
	{
		delete_tmp_close_others(path_to_cfg, file_from);
		exit_with_msg(APOLOGY_MEMORY_MSG, EXIT_MEMORY_FAILURE);
	}
	return mode;
}

static Pointer ARV_path_to_new_model(FILE* file_from, const char* path_to_cfg, Pointer* cfg)
{
	char* ask_msg = NULL;
	char str_1[BUFFER_SIZE] = "\n��������� ������ ������ ���� ��������� � ���� � ������� .TXT\n������� ����������:\n";
	// ���������� �������� ����� ��� if'�, ����� �� ���������� � ����� � ask_cfg_param
	if (file_from == stdin)
	{
		char* current_path = rel_to_abs_way(NULL, false, true);
		char str_2[BUFFER_SIZE] = "\n������� ���������� ��� ������������� ���� ��� ���������� ����� ������ � ������� path\\model_name.txt: ";
		ask_msg = strcat(strcat(str_1, current_path), str_2);
		free(current_path);
	}
	char* path_to_new_model = ask_cfg_param(file_from, path_to_cfg,
										    read_path_param, verify_path_to_new,
											cfg, "���� ��� ���������� ����� ������", ask_msg);
	return path_to_new_model;
}

static Pointer ARV_path_to_existed_model(FILE* file_from, const char* path_to_cfg, Pointer* cfg)
{
	char* ask_msg = NULL;
	char str_1[BUFFER_SIZE] = "\n��� ���������� ������ ����� ��� ��������� ������, ���������� � ����� � ������� .TXT\n������� ����������:\n"; 
	// ���������� �������� ����� ��� if'�, ����� �� ���������� � ����� � ask_cfg_param
	if (file_from == stdin)
	{
		char* current_path = rel_to_abs_way(NULL, false, true);
		char str_2[BUFFER_SIZE] = "\n������� ���������� ��� ������������� ���� � ��������� ������ � ������� path\\model_name.txt: ";
		ask_msg = strcat(strcat(str_1, current_path), str_2);
		free(current_path);
	}
	char* path_to_existed_model = ask_cfg_param(file_from, path_to_cfg,
												read_path_param, verify_path_to_existed,
												cfg, "���� � ��������� ������", ask_msg);
	return path_to_existed_model;
}

static Pointer ARV_path_to_new_text(FILE* file_from, const char* path_to_cfg, Pointer* cfg)
{
	char* ask_msg = NULL;
	char buffer[BUFFER_SIZE];
	// ���������� �������� ����� ��� if'�, ����� �� ���������� � ����� � ask_cfg_param
	if (file_from == stdin)
	{
		char* current_path = rel_to_abs_way(NULL, false, true);
		sprintf(buffer, "\n������� ����������:\n%s\n������� ���������� ��� ������������� ���� � ������� path\\filename.txt, �� �������� ��������� ����������������� �����: ", 
				current_path);
		ask_msg = buffer;
		free(current_path);
	}
	char* path_to_new_text = ask_cfg_param(file_from, path_to_cfg,
										   read_path_param, verify_path_to_new,
										   cfg, "���� ��� ���������� ������������������ ������", ask_msg);
	return path_to_new_text;
}

static Pointer ARV_path_to_existed_text(FILE* file_from, const char* path_to_cfg, Pointer* cfg, int* mode)
{
	char* ask_msg = NULL;
	char* param_name = (*mode == EDIT_MODE) ? "���� � ������ ��� ��������������" : "���� � ������ ��� ��������";
	char buffer[BUFFER_SIZE];
	// ���������� �������� ����� ��� if'�, ����� �� ���������� � ����� � ask_cfg_param
	if (file_from == stdin)
	{
		char* current_path = rel_to_abs_way(NULL, false, true);
		char* purpose = (*mode == EDIT_MODE) ? "��������������" : "��������";
		sprintf(buffer, "\n������� ����������:\n%s\n������� ���������� ��� ������������� ���� � ������� path\\filename.txt � ������ ��� %s: ", current_path, purpose);
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
	// ���������� �������� ����� ��� if'�, ����� �� ���������� � ����� � ask_cfg_param
	if (file_from == stdin)
	{
		sprintf(buffer, "\n������ ������ ����� ���������� ����� ������.\n������� ������������ ����� ���� ��� ����������� (�� ����� 1, �� ����� %d): ", MAX_AVAILABLE_WORD_LENGTH);
		ask_msg = buffer;
	}
	int* max_word_length = ask_cfg_param(file_from, path_to_cfg,
										 read_int_param, verify_max_word_length,
										 cfg, "������������ ����� ������������ ����", ask_msg);
	return max_word_length;
}

static Pointer ARV_size_tol(FILE* file_from, const char* path_to_cfg, Pointer* cfg)
{
	char* ask_msg = NULL;
	char buffer[BUFFER_SIZE];
	// ���������� �������� ����� ��� if'�, ����� �� ���������� � ����� � ask_cfg_param
	if (file_from == stdin)
	{
		sprintf(buffer, "\n������ ���� ������ ������ �� ��������� ���� ������ +-size_tol �� ����������� �����\n������� size_tol (�� ����� 0, �� ����� %d): ", MAX_AVAILABLE_MISSES);
		ask_msg = buffer;
	}
	int* size_tol = ask_cfg_param(file_from, path_to_cfg,
								 read_int_param, verify_size_tol,
								 cfg, "������������ ���������� ����� ����������� �����", ask_msg);
	return size_tol;
}

static Pointer ARV_threshold(FILE* file_from, const char* path_to_cfg, Pointer* cfg)
{
	char* ask_msg = NULL;
	char buffer[BUFFER_SIZE];
	// ���������� �������� ����� ��� if'�, ����� �� ���������� � ����� � ask_cfg_param
	if (file_from == stdin)
	{
		sprintf(buffer, "\n������ �������� �����, ���� ������ ������ ���������� �� �����, ��� � threshold ��������.\n������� threshold (�� ����� 0, �� ����� %d): ", MAX_AVAILABLE_ERRORS);
		ask_msg = buffer;
	}
	int* threshold = ask_cfg_param(file_from, path_to_cfg,
								   read_int_param, verify_threshold,
								   cfg, "������������ ���������� � ������������ �����", ask_msg);
	return threshold;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static int* read_mode(FILE* file_from)//---------------------------------------------------------------------------------------------
{
	// ������ ������, ���������� int
	// �������� wrong_mode
	int* dummy_mode = malloc(sizeof(int));
	*dummy_mode = 0;
	return dummy_mode;
}

static char* read_path_param(FILE* file_from)//---------------------------------------------------------------------------------------------
{
	char* hel = malloc(sizeof(char) * 6);
	strcpy(hel, "hello");
	return hel;
}

static int* read_int_param(FILE* file_from)//---------------------------------------------------------------------------------------------
{
	int* dummy_length = malloc(sizeof(int));
	*dummy_length = 10;
	return dummy_length;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static bool verify_mode(const int* mode)//---------------------------------------------------------------------------------------------
{
	return true;
}

static bool verify_path_to_new(const char* path_to_new)//---------------------------------------------------------------------------------------------
{
	return true;
}

static bool verify_path_to_existed(const char* path_to_existed)//---------------------------------------------------------------------------------------------
{
	return true;
}

static bool verify_max_word_length(const int* max_word_length)//---------------------------------------------------------------------------------------------
{
	return true;
}

static bool verify_size_tol(const int* size_tol)//---------------------------------------------------------------------------------------------
{
	return true;
}

static bool verify_threshold(const int* threshold)//---------------------------------------------------------------------------------------------
{
	return true;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/