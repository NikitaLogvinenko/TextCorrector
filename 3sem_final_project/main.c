#define WRONG_MODE_ERROR 1


#include "linked_list_tests.h"
#include "help_func_tests.h"
#include "helpful_functions.h"
#include "train_module.h"
#include "edit_module.h"
#include <stdio.h>
#include <locale.h>

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "ru");

	// �� ���������� ������ ���������� ����� ������ - �������� ('t') ��� ��������������('e')
	char mode = '1';//define_model_mode(argc, argv);
	if (mode == 't')
		printf("*");
		//train_model(argc, argv);
	else if(mode == 'e')
		printf("*");
		//edit_text(argc, argv);
	else
	{
		printf("������ ��� ����������� ������ ������ ���������.\n��������� ��������:\n[-t]\t��������\n[-e]\t�������������� ������\n���������� ��� ���\n");
		return WRONG_MODE_ERROR;
	}
	return 0;
}

