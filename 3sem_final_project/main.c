#define WRONG_MODE_ERROR 1


#include "linked_list_tests.h"
#include "help_func_tests.h"
#include "helpful_functions.h"
#include "specific_functions.h"
#include "train_module.h"
#include "edit_module.h"
#include <stdio.h>
#include <locale.h>

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "ru");

	// По первому параметру вызова определяем режим работы - обучение ('t'), редактирование('e'), 
	char mode = 1;//define_params(argc, argv);
	if (mode == 't')
		printf("*");
		//train_model(argv + 2);
	else if(mode == 'e')
		printf("*");
		//edit_text(argv + 2);
	else
	{
		printf("Ошибка при определении режима работы программы.\nВозможные варианты:\n[-t]\tобучение\n[-e]\tредактирование текста\nПопробуйте ещё раз\n");
		return WRONG_MODE_ERROR;
	}
	return 0;
}

