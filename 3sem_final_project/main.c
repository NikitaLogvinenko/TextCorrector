#include "constants.h"
#include "helpful_functions.h"
#include "params_working.h"
#include "train_module.h"
#include "edit_module.h"
#include <stdio.h>
#include <windows.h>


// ��������� ������ ����� ������ � �������� cfg.
// ���������� ��� ����������.
static int launch_model(Pointer* cfg);

int main(int argc, char** argv)
{
	int exit_code = EXIT_SUCCESSFULLY;
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);	
	printf("\n********************************************************************************************************************\n");
	if (argc > 1 && strcmp("help", argv[1]) != 0) // ������� ��������� � �� ��������� ������
		printf("# ��������� ������������ ���������...\n\n");
	// ������������� ������������ ������ ���������
	// � ������ ��������� ��������� ������������ ��������� ���������� ������ set_cfg ����� exit � �������� ������
	Pointer* cfg = set_cfg(argc, argv);
	printf("\n# ������������ ������� ���������!\n\n");

	exit_code = launch_model(cfg);
	delete_cfg(cfg);

	if (exit_code == EXIT_SUCCESSFULLY)
		printf("\n# ��������� ������� ���������. \n������ ����������� �� �����: --CENSORED--\n\n");
	printf("���������� �� ������������� ������ ����������! �������� ���� �����: ");
	getchar();
	clear_buff();

	return exit_code;
}

static int launch_model(Pointer* cfg)
{
	int exit_code = EXIT_SUCCESSFULLY;
	int mode = *(int*)cfg[0];
	switch (mode)
	{
	case TRAIN_NEW_MODE:
		print_train_new_cfg(cfg);
		printf("\n# ������ �������� ����� ������\n\n");
		exit_code = train_new_model(cfg);
		break;
	case TRAIN_EXISTED_MODE:
		print_train_existed_cfg(cfg);
		printf("\n# ������ �������� ������������ ������\n\n");
		exit_code = train_existed_model(cfg);
		break;
	case EDIT_MODE:
		print_edit_cfg(cfg);
		printf("\n# ������ �������������� ������\n\n");
		exit_code = edit_text(cfg);
		break;
	}
	return exit_code;
}