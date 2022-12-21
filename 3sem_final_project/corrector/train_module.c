#include "train_module.h"
#include "constants.h"
#include <stdio.h>
#include <locale.h>

int train_new_model(const Pointer* cfg)//---------------------------------------------------------------------------------------------
{
	setlocale(LC_ALL, "ru");
	printf("train_new\n");
	return EXIT_SUCCESSFULLY;
}

int train_existed_model(const Pointer* cfg)//---------------------------------------------------------------------------------------------
{
	setlocale(LC_ALL, "ru");
	printf("train_existed\n");
	return EXIT_SUCCESSFULLY;
}