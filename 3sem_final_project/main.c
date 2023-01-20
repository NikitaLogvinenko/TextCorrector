#include "constants.h"
#include "helpful_functions.h"
#include "params_working.h"
#include "train_module.h"
#include "edit_module.h"
#include <stdio.h>
#include <windows.h>

#include <string.h>
#include <ctype.h>
#include "words_handling.h"
#include "hash_table_test.h"


int main(int argc, char** argv)
{
	int exit_code = EXIT_SUCCESSFULLY;
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Pointer* pp = empty_pointers_array(4);
	const char* model_name = "B:\\Документы, изображения и тп\\Документы\\Никита\\Программирование\\Проекты Visual Studio 2022\\Структурное программирование, 3 семестр\\3sem_final_project\\3sem_final_project\\tests\\new_files\\new_model.txt";
	const char* text_name = "B:\\Документы, изображения и тп\\Документы\\Никита\\Программирование\\Проекты Visual Studio 2022\\Структурное программирование, 3 семестр\\3sem_final_project\\3sem_final_project\\tests\\existed_files\\texts\\Иван Гончаров - Обломов.txt";
	pp[1] = malloc(sizeof(char) * (strlen(model_name) + 1));
	pp[2] = malloc(sizeof(char) * (strlen(text_name) + 1));
	pp[3] = malloc(sizeof(int));
	*(int*)pp[3] = 10;
	strcpy(pp[1], model_name);
	strcpy(pp[2], text_name);
	exit_code = train_existed_model(pp);
	
	//const char* new_name = "B:\\Документы, изображения и тп\\Документы\\Никита\\Программирование\\Проекты Visual Studio 2022\\Структурное программирование, 3 семестр\\3sem_final_project\\3sem_final_project\\tests\\new_files\\new_model.txt";
	/*FILE* file = fopen(new_name, "w");
	fputc(-14, file);
	fputc(242, file);
	fclose(file);
	file = fopen(new_name, "r");
	int symbol = fgetc(file);
	while (symbol != EOF)
	{
		printf("%c %d %u\n", symbol, symbol, symbol);
		symbol = fgetc(file);
	}
	printf("----------[%d]-------------\n", EOF);*/

	



	//printf("\n********************************************************************************************************************\n");
	//if (argc > 1 && strcmp("help", argv[1]) != 0) // введены параметры и не требуется помощь
	//	printf("# Настройка конфигурации программы...\n\n");
	//// Устанавливаем конфигурацию работы программы
	//// В случае неудачной установки конфигурации программа завершится внутри set_cfg через exit с очисткой памяти
	//Pointer* cfg = set_cfg(argc, argv);
	//printf("\n# Конфигурация успешно настроена!\n\n");
	//int mode = *(int*)cfg[0];
	//switch (mode)
	//{
	//case TRAIN_NEW_MODE:
	//	print_train_new_cfg(cfg);
	//	printf("\n# Запуск обучения новой модели\n\n");
	//	exit_code = train_new_model(cfg);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	break;
	//case TRAIN_EXISTED_MODE:
	//	print_train_existed_cfg(cfg);
	//	printf("\n# Запуск обучения существующей модели\n\n");
	//	exit_code = train_existed_model(cfg);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	break;
	//case EDIT_MODE:
	//	print_edit_cfg(cfg);
	//	printf("\n# Запуск редактирования текста\n\n");
	//	exit_code = edit_text(cfg);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	break;
	//}
	//delete_cfg(cfg);

	//if (exit_code == EXIT_SUCCESSFULLY)
	//	printf("\n# Программа успешно завершена. \nДонаты принимаются на карту: --CENSORED--\n\n");
	//printf("Благодарим за использование нашего приложения! Оставьте свой отзыв: ");
	//char wait = 0;
	//scanf("%c", &wait);

	return exit_code;
}

