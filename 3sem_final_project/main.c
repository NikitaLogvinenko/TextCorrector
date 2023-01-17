#include "constants.h"
#include "helpful_functions.h"
#include "params_working.h"
#include "train_module.h"
#include "edit_module.h"
#include <stdio.h>
#include <locale.h>
#include <windows.h>


int main(int argc, char** argv)
{
	int exit_code = EXIT_SUCCESSFULLY;
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	printf("\n********************************************************************************************************************\n");
	if (argc > 1 && strcmp("help", argv[1]) != 0) // введены параметры и не требуется помощь
		printf("# Настройка конфигурации программы...\n\n");
	// Устанавливаем конфигурацию работы программы
	// В случае неудачной установки конфигурации программа завершится внутри set_cfg через exit с очисткой памяти
	Pointer* cfg = set_cfg(argc, argv);
	printf("\n# Конфигурация успешно настроена!\n\n");
	int mode = *(int*)cfg[0];
	switch (mode)
	{
	case TRAIN_NEW_MODE:
		print_train_new_cfg(cfg);
		printf("\n# Запуск обучения новой модели\n\n");
		exit_code = train_new_model(cfg);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		break;
	case TRAIN_EXISTED_MODE:
		print_train_existed_cfg(cfg);
		printf("\n# Запуск обучения существующей модели\n\n");
		exit_code = train_existed_model(cfg);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		break;
	case EDIT_MODE:
		print_edit_cfg(cfg);
		printf("\n# Запуск редактирования текста\n\n");
		exit_code = edit_text(cfg);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		break;
	}
	delete_cfg(cfg);

	printf("\n# Программа успешно завершена. \nДонаты принимаются на карту: --CENSORED--\n\n");
	printf("Благодарим за использование нашего приложения! Оставьте свой отзыв: ");
	char wait = 0;
	scanf("%c", &wait);

	return exit_code;
}

