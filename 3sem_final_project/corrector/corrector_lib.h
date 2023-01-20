#ifndef _CORRECTOR_LIB_
#define _CORRECTOR_LIB_


#include "CorrectorModel.h"
#include <stdio.h>


// Инициализация корректора. Создание таблиц. Установка функции поиска разности слов (стандартно - расстояние Хэмминга)
// В случае успешного выделения памяти под таблицы возвращает EXIT_SUCCESSFULLY, иначе EXIT_MEMORY_FAILURE
int corrector_init(Corrector* corrector, unsigned max_word_length, WordsMetric metric_func);

// Очистка таблиц в корректоре
// Обратите внимание, чистятся только таблицы. Если память под сам корректор выделялась динамически - необходимо самостотельно выполнить free(corrector)
void corrector_destroy(Corrector* corrector);

// Сохранение ОБУЧЕННОЙ модели в файл model_file_name. Запись осуществляется в формате:
// max_word_length=mwl
// <table_1> ...
// <table_2> ...
// ...
// <table_mwl> ...
// По окончании записи закрывает файл
// В случае успешного сохранения возвращает EXIT_SUCCESSFULLY
// Если model_file_name не является путём к файлу .txt - вернёт EXIT_USER_FAILURE
// Если не открылся файл - вернёт EXIT_FILE_FAILURE
int corrector_save(Corrector* corrector, const char* model_file_name);

// Записать слово в соответствующую таблицу в корректоре. Установить счётчик слова в 1, если это новое слово, или увеличить счётчик существующего слова на 1
// Если слово успешно записано - вернёт EXIT_SUCCESSFULLY
// Если указатель на модель или на слово равны NULL - вернёт EXIT_USER_FAILURE
// Если слово слишком длинное для таблицы - вернёт EXIT_TOO_LONG
// Если не выделилась память под новое слово, то модель остаётся прежней, а функция вернёт EXIT_MEMORY_FAILURE
int corrector_learn(Corrector* corrector, const char* word);

// Загрузить обученную модель из файла model_file_name в НЕИНИЦИАЛИЗИРОВАННУЮ модель (если модель уже инициализирована - она будет перезаписана, однако вероятны утечки памяти)
// Из файла считывается max_word_length и модель инициализируется, создаётся нужное количество таблиц. Далее таблицы заполняются словами из файла
// Если модель успешно загружена - вернуть EXIT_SUCCESSFULLY
// Если не выделилась память под изначальные таблицы или под таблицы нужного размера - вернуть EXIT_MEMORY_FAILURE. В таком случае все прочитанные данные также чистятся
// Если файл не открылся - вернуть EXIT_FILE_FAILURE
// Если файл имеет неправильный формат - вернуть EXIT_USER_FAILURE
// Если модель или имя файла указывают на NULL, или имя файла не является путём к файлу .txt - вернуть EXIT_USER_FAILURE
int corrector_load(Corrector* corrector, const char* model_file_name);


#endif 

