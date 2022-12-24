//#include "linked_list_tests.h"/////////////////////////////////////// удалить /////////////////////////////////////////////
//#include "help_func_tests.h"/////////////////////////////////////// удалить /////////////////////////////////////////////
#include "constants.h"
#include "helpful_functions.h"
#include "params_working.h"
#include "files_working.h"/////////////////////////////////////// удалить /////////////////////////////////////////////
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
	Pointer* cfg = set_cfg(argc, argv);  // в случае неудачной установки конфигурации программа завершится внутри set_cfg через exit с очисткой памяти
	printf("\n# Конфигурация успешно настроена!\n\n");
	int mode = *(int*)cfg[0];
	switch (mode)
	{
	case TRAIN_NEW_MODE:
		printf("\n# Запуск обучения новой модели\n\n");
		exit_code = train_new_model(cfg);
		break;
	case TRAIN_EXISTED_MODE:
		printf("\n# Запуск обучения существующей модели\n\n");
		exit_code = train_existed_model(cfg);
		break;
	case EDIT_MODE:
		printf("\n# Запуск редактирования текста\n\n");
		exit_code = edit_text(cfg);
		break;
	}

	delete_cfg(cfg);
	printf("\n# Программа успешно завершена. \nДонаты принимаются на карту: --CENSORED--\n\n");
	
	printf("Благодарим за использование нашего приложения! Оставьте свой отзыв: ");
	char wait = 0;
	scanf("%c", &wait);
	return exit_code;
}

