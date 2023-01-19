#include "constants.h"
#include "helpful_functions.h"
#include "params_working.h"
#include "train_module.h"
#include "edit_module.h"
#include <stdio.h>
#include <windows.h>

#include <string.h>


int main(int argc, char** argv)
{
	int exit_code = EXIT_SUCCESSFULLY;
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Pointer* pp = empty_pointers_array(4);
	const char* model_name = "B:\\���������, ����������� � ��\\���������\\������\\����������������\\������� Visual Studio 2022\\����������� ����������������, 3 �������\\3sem_final_project\\3sem_final_project\\tests\\new_files\\new_model.txt";
	const char* text_name = "B:\\���������, ����������� � ��\\���������\\������\\����������������\\������� Visual Studio 2022\\����������� ����������������, 3 �������\\3sem_final_project\\3sem_final_project\\tests\\existed_files\\text_for_learning.txt";
	pp[1] = malloc(sizeof(char) * (strlen(model_name) + 1));
	pp[2] = malloc(sizeof(char) * (strlen(text_name) + 1));
	pp[3] = malloc(sizeof(int));
	*(int*)pp[3] = 10;
	strcpy(pp[1], model_name);
	strcpy(pp[2], text_name);
	exit_code = train_new_model(pp);
	
	//printf("\n********************************************************************************************************************\n");
	//if (argc > 1 && strcmp("help", argv[1]) != 0) // ������� ��������� � �� ��������� ������
	//	printf("# ��������� ������������ ���������...\n\n");
	//// ������������� ������������ ������ ���������
	//// � ������ ��������� ��������� ������������ ��������� ���������� ������ set_cfg ����� exit � �������� ������
	//Pointer* cfg = set_cfg(argc, argv);
	//printf("\n# ������������ ������� ���������!\n\n");
	//int mode = *(int*)cfg[0];
	//switch (mode)
	//{
	//case TRAIN_NEW_MODE:
	//	print_train_new_cfg(cfg);
	//	printf("\n# ������ �������� ����� ������\n\n");
	//	exit_code = train_new_model(cfg);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	break;
	//case TRAIN_EXISTED_MODE:
	//	print_train_existed_cfg(cfg);
	//	printf("\n# ������ �������� ������������ ������\n\n");
	//	exit_code = train_existed_model(cfg);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	break;
	//case EDIT_MODE:
	//	print_edit_cfg(cfg);
	//	printf("\n# ������ �������������� ������\n\n");
	//	exit_code = edit_text(cfg);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	break;
	//}
	//delete_cfg(cfg);

	//printf("\n# ��������� ������� ���������. \n������ ����������� �� �����: --CENSORED--\n\n");
	//printf("���������� �� ������������� ������ ����������! �������� ���� �����: ");
	//char wait = 0;
	//scanf("%c", &wait);

	return exit_code;
}

