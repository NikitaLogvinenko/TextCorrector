#define SUCCESSFULLY_PERFORMED 0
#define WRONG_MODE_ERROR 1


#include "linked_list_tests.h"
#include "help_func_tests.h"
#include "constants.h"
#include "helpful_functions.h"
#include "params_working.h"
#include "train_module.h"
#include "edit_module.h"
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int exit_code = EXIT_SUCCESSFULLY;
	setlocale(LC_ALL, "ru");

	Pointer* cfg = set_cfg(argc, argv);
	int mode = 0;//*(int*)cfg[0];
	printf("%d\n", mode);
	delete_cfg(mode, cfg);


	return exit_code;
}

