//#include "linked_list_tests.h"/////////////////////////////////////// ������� /////////////////////////////////////////////
//#include "help_func_tests.h"/////////////////////////////////////// ������� /////////////////////////////////////////////
#include "constants.h"
#include "helpful_functions.h"
#include "params_working.h"
#include "files_working.h"/////////////////////////////////////// ������� /////////////////////////////////////////////
#include "train_module.h"
#include "edit_module.h"
#include <stdio.h>
#include <locale.h>


int main(int argc, char** argv)
{
	int exit_code = EXIT_SUCCESSFULLY;
	setlocale(LC_ALL, "ru");
	printf("\n*********************************************************************************************************************\n");
	printf("# ��������� ������������ ���������...\n\n");
	Pointer* cfg = set_cfg(argc, argv);  // � ������ ��������� ��������� ������������ ��������� ���������� ������ set_cfg ����� exit � �������� ������
	printf("\n# ������������ ������� ���������!\n\n");
	int mode = *(int*)cfg[0];
	switch (mode)
	{
	case TRAIN_NEW_MODE:
		printf("\n# ������ �������� ����� ������\n\n");
		exit_code = train_new_model(cfg);
		break;
	case TRAIN_EXISTED_MODE:
		printf("\n# ������ �������� ������������ ������\n\n");
		exit_code = train_existed_model(cfg);
		break;
	case EDIT_MODE:
		printf("\n# ������ �������������� ������\n\n");
		exit_code = edit_text(cfg);
		break;
	}

	delete_cfg(cfg);
	printf("\n# ��������� ������� ���������. \n������ ����������� �� �����: --CENSORED--\n\n");
	
	printf("���������� �� ������������� ������ ����������! �������� ���� �����: ");
	char wait = 0;
	scanf("%c", &wait);
	return exit_code;
}

