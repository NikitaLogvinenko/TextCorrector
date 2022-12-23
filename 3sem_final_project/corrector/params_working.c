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


// ���������� ������������ �� ���������� ��� �������. params_amount - ������� ���������� �� ������� ������ � ������� (����� �� ������������)
// ��� ��������� ���������� �� ��������� ���� TMP_PARAMS_FILE � �������� cfg_from_file(1, TMP_PARAMS_FILE)
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

// ������� ������������ �� �����. params_amount - ������� ���������� �� ������� ������ � ������� (������ ���� �� ������ ������ - ���� � ������������)
// ��������� � ����� ������ ���� ����� ������. ���� ������ ������ � �������� (�����-���� ���� �������� ������), �� ��������� �������� � �������
// � ������ ������ ������ � ������� ������������ NULL
// � ������ ������ ������ � ������ ��������� ��������� ����������� � ���������� �� ������. �������� ������������ ������ ��� ������ � ������ - ������ �������� ����� � �������������
// �.�. ������ ��� ��������� �� ���������� �� ��������, �� ������� ������. ����� ��� ���� ������� TMP_PARAMS_FILE, ���� ��� ��� ��������� ���� (��������� �������� ��� �������)
static Pointer* cfg_from_file(int params_amount, const char** ptr_to_path);


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


// ��������� ������������ ������ �������� ������������ � �������
// � ������� ��������� ask_msg � �������� ������ ��������
// ����� �������� �����������. ������ ��� �������� ���������� �����������. ������ ��������� ���������� � ������� read_func �� ������� (file=stdin)
// ������������ ��������� ��������� ����������� �������� verification. �������� ������ ������, ���� �� �� ������ �����������
// ���� ������� ����������� �������� NULL, �� ��� ����� ������ ���������� true. ��� �������� ������ ������ � �������, ������� ����� ���������� � ����� ����� ��������� cfg
// � ������ ������ ������ � ������� ������������ NULL. ���� �� ������ - ���������� ��������� �� ��������
// ���� ������������ �� ���������� ����� ������� �� ���� ��������� ������ ��������, �� ����� �� ����, ������ cfg � ��������� ������ ����������
static Pointer ask_cfg_param(const char* ask_msg, Pointer (*read_func)(FILE*), bool (*verification)(Pointer), Pointer* cfg);

// ������ ������������ ��� �������� ����� ������ ����� �������. ������������ ������������ ������ ���������� �� ��������� �������
// ����� ��� ������ � �� ������ ��������� ����������. ������ ������ ��� ���� ��� �������� � ��� ���� ���� �������� � cfg
// � ������ ������ ������ � ������� ������������ NULL
static Pointer* ask_train_new_cfg(int* mode);

// ������ ������������ ��� ���������� ������������ ������ ����� �������. ������������ ������������ ������ ���������� �� ��������� �������
// ����� ��� ������ � �� ������ ��������� ����������. ������ ������ ��� ���� ��� �������� � ��� ���� ���� �������� � cfg
// � ������ ������ ������ � ������� ������������ NULL
static Pointer* ask_train_existed_cfg(int* mode);

// ������ ������������ ��� �������������� ������ ����� �������. ������������ ������������ ������ ���������� �� ��������� �������
// ����� ��� ������ � �� ������ ��������� ����������. ������ ������ ��� ���� ��� �������� � ��� ���� ���� �������� � cfg
// � ������ ������ ������ � ������� ������������ NULL
static Pointer* ask_edit_cfg(int* mode);


// ��������� ����� �� ������ file. �������� ��� ���� ����������� ������ � ���������� ��������� �� ����� ������.
// ���� ������ �� ���������� - ������ NULL. �������������� ��� ���� ��� ������
static int* read_mode(FILE* file);

// ��������� ���� ���������� ������ �� ������ file. �������� ��� ���� ����������� ������ � ���������� ��������� �� ������.
// ���� ������ �� ���������� - ������ NULL. �������������� ��� ���� ��� ������
static char* read_path_to_save(FILE* file);

// ��������� ���� � ���������� ������ �� ������ file. �������� ��� ���� ����������� ������ � ���������� ��������� �� ������.
// ���� ������ �� ���������� - ������ NULL. �������������� ��� ���� ��� ������
static char* read_path_to_text(FILE* file);

// ��������� ������������ ����� ��������� ���� �� ������ file. �������� ����������� ������ ��� �������� � ���������� ���������.
// ���� ������ �� ���������� - ������ NULL. �������������� ��� ���� ��� ������
static int* read_max_word_length(FILE* file);

// ���������, ��� �������� ������������� ����� ����������
static bool verify_mode(const int* mode_by_user);

// ���������, ��� ������������ ��� ���������� ���� ��� ���������� ������
static bool verify_path_to_save(const char* path_to_save_by_user);

// ���������, ��� ������������ ��� ���������� ���� � ���������� ������
static bool verify_path_to_text(const char* path_to_save_by_user);

// ���������, ��� ������������ ��� ���������� ������������ ����� ��������� ����
static bool verify_max_word_length(const int* path_to_save_by_user);


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
			cfg = cfg_from_file(argc - 2, argv + 2);  // ���������� ���� � ��������� � ������ ������� �������
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


static Pointer* cfg_from_params(int params_amount, const char** params_for_cfg)
{
	sufficiently_params_check(1, params_amount, "� ���������� ���������� ����� ������ ���������\n");
	// ���� ���� ���� ���� ��������, �� ������ ��������� ����, ���������� �� ���� � ��������� �������� cfg_from_file
	FILE* tmp_file = create_and_open_tmp(TMP_PARAMS_FILE, "w"); // ���� �� ������ ��� ������ - �������� ��������� �� �������� ����. � ������ ������ ��������� �����������
	bool successfully_written = file_write(tmp_file, params_amount, params_for_cfg, "\"", " ", NULL);
	fclose(tmp_file);
	Pointer* cfg = NULL;
	if (successfully_written)
		cfg = cfg_from_file(1, &TMP_PARAMS_FILE); // ���� ���� �� ���������, ��� ������������ � cfg_from_file; ���� ��������� ������ ������, �������� NULL
	if (file_remove(TMP_PARAMS_FILE, "������ �������� ���������� ����� � �����������. ������� ��� ���� ��� �������������:\n") == false) // �� ���������� ������� ��������� ����
		free(rel_to_abs_way(TMP_PARAMS_FILE, true, true)); // ���������� ������������ �� ���� � ��������� �������, ����������� ���������� ����
	return cfg;
}

static Pointer* cfg_step_by_step()
{
	printf("������������ ������ ��������� �� ������. ������� ����������� ��������� � �������\n\n");
	// ��������� ������ �� ���� ������ ������
	char str_1[BUFFER_SIZE] = "����� ������ ����������:\n";
	char str_2[] = "\t\"train_new\" - �������� ����� ������\n\t\"train_existed\" - ���������� ������������ ������\n\t\"edit\" - �������������� ������\n";
	char str_3[] = "������� �����: ";
	// �������� ����� ������
	int* mode = ask_cfg_param(strcat(strcat(str_1, str_2), str_3), 
		read_mode, verify_mode, NULL);
	if (mode == NULL)  // �� ���������� ������ ��� �����
		exit_with_msg(APOLOGY_MEMORY_MSG, EXIT_MEMORY_FAILURE);
	// � ����������� �� ������ �������� ���������� ��������� ������������ (��������� �� ����� ���������, ����� ���� �������� ��� � cfg)
	Pointer* cfg = NULL;
	switch (*mode)
	{
	case TRAIN_NEW_MODE:
		cfg = ask_train_new_cfg(mode);
		break;
	case TRAIN_EXISTED_MODE:
		cfg = ask_train_existed_cfg(mode);
		break;
	case EDIT_MODE:
		cfg = ask_edit_cfg(mode);
		break;
	}
	// ���� �� ���������� ������ ��� cfg, �� ������ mode �� ����������, � ������ ��� ���� ��� ��������. ������ �������!
	// ���� �� ������ ��� cfg ����������, � ��� �����-�� �������� ���, �� mode ����� ��� � cfg � ����� ����������, �������� ����� ������ �� ����
	if (cfg == NULL)
		free(mode);
	return cfg;
}

static Pointer* cfg_from_file(int params_amount, const char** ptr_to_path)//---------------------------------------------------------------------------------------------
{
	sufficiently_params_check(1, params_amount, "�� ����� ���� � ����� � ������������� ���������\n");

	/**/

	return NULL;
}


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
			if (file_remove(tmp_file_name, "�� ������� ������� ��������� ����. ������� ��� ��������������:\n") == false) // ��� � ������-�� �� ������� ������� ����
				free(rel_to_abs_way(tmp_file_name, true, true));
			exit_with_msg("������ �������� ���������� �����.\n�������� � �������� ��������� �� ������������ ����������!\n��������� �������\n", EXIT_FILE_FAILURE);
		}
	}
	return tmp_file;
}


static Pointer ask_cfg_param(const char* ask_msg, Pointer(*read_func)(FILE*), bool (*verification)(Pointer), Pointer* cfg)
{
	if (ask_msg != NULL)
		printf("%s", ask_msg);
	Pointer param = read_func(stdin);
	bool correct_param = verification(param);
	// ���� ������������ ��� �������� �����������, ��� ��� ��� ��������� ������� (���� �� �� ����� ���������, ���� �� �������� ��� ������� ��� ���� �� ��������� ������ ������)
	for (unsigned attempt = 0; attempt < MAX_USER_ATTEMPTS && correct_param == false; ++attempt)
	{
		free(param);
		printf("������������ ����. ��������� �������: ");
		param = read_func(stdin);
		correct_param = verification(param);
	}
	if (correct_param == false) // ������������ �� ���������(((
	{
		free(param);
		delete_cfg(cfg);
		exit_with_msg("��������� ���������� �������. ���������� ������� � ���������� ������.\n", EXIT_USER_FAILURE);
	}
	return param; // ������������ ���������))). �� ��� �� ���������� ������...
}

static Pointer* ask_train_new_cfg(int* mode)
{
	Pointer* train_new_cfg = empty_pointers_array(TRAIN_NEW_CFG_SIZE);
	if (train_new_cfg != NULL)  // ���� ������ ��� cfg ����������, ��������� ��� ����������� (�� ����� ������� �� �����)
	{
		// ���������� �����
		train_new_cfg[0] = mode;

		// �������� ���� ��� ���������� ������
		char str_1[BUFFER_SIZE] = "\n��������� ������ ������ ���� ��������� � ���� � ������� .TXT\n������� ����������:\n";
		char* current_path = rel_to_abs_way(NULL, false, true);
		char str_2[BUFFER_SIZE] = "\n������� ���������� ��� ������������� ���� ��� ���������� ����� � ������� path\\model_name.txt: ";
		train_new_cfg[1] = ask_cfg_param(strcat(strcat(str_1, current_path), str_2), 
			read_path_to_save, verify_path_to_save, train_new_cfg);

		// �������� ���� � ���������� ������
		strcpy(str_1, "\n������ ����� ����� �����, ��������� ������ �� ������� � ���������� ����.\n������� ����������:\n");
		strcpy(str_2, "\n������� ���������� ��� ������������� ���� � ���������� ������ � ������� .TXT � ���� path\\training_text_name.txt: ");
		train_new_cfg[2] = ask_cfg_param(strcat(strcat(str_1, current_path), str_2),
			read_path_to_text, verify_path_to_text, train_new_cfg);

		// �������� ������������ ����� ������������ ����
		sprintf(str_1, "\n������ ������ ����� ���������� ����� ������.\n������� ������������ ����� ���� ��� ����������� (�� ����� %d): ", MAX_AVAILABLE_WORD_LENGTH);
		train_new_cfg[3] = ask_cfg_param(str_1, read_max_word_length, verify_max_word_length, train_new_cfg);
		free(current_path);
	}
	return train_new_cfg;
}

static Pointer* ask_train_existed_cfg(int* mode)//---------------------------------------------------------------------------------------------
{
	return NULL;
}

static Pointer* ask_edit_cfg(int* mode)//---------------------------------------------------------------------------------------------
{
	return NULL;
}

static int* read_mode(FILE* file)//---------------------------------------------------------------------------------------------
{
	// �������� wrong_mode
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
	// � ������ ������ ������ ���������� true. ����� �� ������� ask_cfg_param ��������� ���� NULL � � set_cfg �� �����, ��� ��������� ������ ������
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