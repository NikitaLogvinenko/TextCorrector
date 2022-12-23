//#include "linked_list_tests.h"
//#include "help_func_tests.h"
#include "constants.h"
#include "helpful_functions.h"
#include "params_working.h"
#include "train_module.h"
#include "edit_module.h"
#include <stdio.h>
#include <locale.h>


int main(int argc, char** argv)
{
	int exit_code = EXIT_SUCCESSFULLY;
	setlocale(LC_ALL, "ru");

	printf("\n*********************************************************************************************************************\n");
	printf("# Настройка конфигурации программы...\n\n");
	Pointer* cfg = set_cfg(argc, argv);  // в случае неудачи установки конфигурации программа завершится внутри set_cfg через exit с очисткой памяти
	printf("\n# Конфигурация успешно настроена!\n\n");
	int mode = *(int*)cfg[0];
	switch (mode)
	{
	case TRAIN_NEW_MODE:
		exit_code = train_new_model(cfg);
		break;
	case TRAIN_EXISTED_MODE:
		exit_code = train_existed_model(cfg);
		break;
	case EDIT_MODE:
		exit_code = edit_text(cfg);
		break;
	}
	delete_cfg(cfg);
	return exit_code;
}

