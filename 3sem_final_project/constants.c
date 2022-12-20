#include "constants.h"

// Размер списка для тестов связных списков
const int TEST_LLIST_SIZE = 10;

// Программа успешно отработала
const int EXIT_SUCCESSFULLY = 0;
// Возникла логическая ошибка при настройке конфигурации. Удалось избежать утечек и корректно выйти, предоставив пользователю справку по пользованию программой
const int EXIT_SOFT_FAILURE = 1;
// Ошибка на этапе обучения или редактирования
const int EXIT_FATAL_FAILURE = 2;

// Неправильно введённый параметр
const int WRONG_PARAM = -1;
// Задаём конфигурацию через параметры
const int CFG_FROM_PARAMS = 0;
// Задаём конфигурацию последовательно через консоль
const int CFG_STEP_BY_STEP = 1;
// Задаём конфигурацию в файле
const int CFG_FROM_FILE = 2;
// Вывести справку
const int PRINT_HELP = 3;

// Режим обучения новой модели
const int TRAIN_NEW_MODE = 0;
// Режим дообучения существующей модели
const int TRAIN_EXISTED_MODE = 1;
// Режим редактирования текста
const int EDIT_MODE = 2;

// Число параметров в конфиге в режиме TRAIN_NEW
const int TRAIN_NEW_CFG_SIZE = 4;
// Число параметров в конфиге в режиме TRAIN_EXISTED
const int TRAIN_EXISTED_CFG_SIZE = 3;
// Число параметров в конфиге в режиме EDIT
const int EDIT_CFG_SIZE = 6;