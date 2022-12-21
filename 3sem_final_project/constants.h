#ifndef _CONSTANTS_
#define _CONSTANTS_

#define _CRT_SECURE_NO_WARNINGS

// Размер списка для тестов связных списков
#define TEST_LLIST_SIZE 10

// Программа успешно отработала
#define EXIT_SUCCESSFULLY 0
// Предусматриваемая ошибка по вине пользователя. Выводим справку и выходим из программы
#define EXIT_SOFT_FAILURE 1
// Программная ошибка (не выделилась память, не открылся файл и тп). Мгновенный выход из программы
#define EXIT_FATAL_FAILURE 2

// Неправильно введённый параметр
#define WRONG_PARAM -1
// Задаём конфигурацию через параметры
#define CFG_FROM_PARAMS 0
// Задаём конфигурацию последовательно через консоль
#define CFG_STEP_BY_STEP 1
// Задаём конфигурацию в файле
#define CFG_FROM_FILE 2
// Вывести справку
#define PRINT_HELP 3

// Неизвестный режим
#define WRONG_MODE -1
// Режим обучения новой модели
#define TRAIN_NEW_MODE 0
// Режим дообучения существующей модели
#define TRAIN_EXISTED_MODE 1
// Режим редактирования текста
#define EDIT_MODE 2

// Число параметров в конфиге в режиме TRAIN_NEW
#define TRAIN_NEW_CFG_SIZE 4
// Число параметров в конфиге в режиме TRAIN_EXISTED
#define TRAIN_EXISTED_CFG_SIZE 3
// Число параметров в конфиге в режиме EDIT
#define EDIT_CFG_SIZE 6


#endif