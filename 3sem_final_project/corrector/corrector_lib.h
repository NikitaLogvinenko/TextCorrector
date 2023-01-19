#ifndef _CORRECTOR_LIB_
#define _CORRECTOR_LIB_


#include "CorrectorModel.h"
#include <stdio.h>


// Инициализация корректора. Создание таблиц. Установка функции поиска разности слов (стандартно - расстояние Хэмминга)
// В случае успешного выделения памяти под таблицы возвращает EXIT_SUCCESSFULLY, иначе EXIT_MEMORY_FAILURE
int corrector_init(Corrector* corrector, unsigned max_word_length, WordsMetric metric_func);

// Очистка таблиц в корректоре. Обратите внимание, чистятся только таблицы. Если память под сам корректор выделялась динамически - необходимо самостотельно выполнить free(corrector)
void corrector_destroy(Corrector* corrector);

// Сохранение ОБУЧЕННОЙ модели в ОТКРЫТЫЙ файл в формате
// max_word_length=mwl
// <table_1> ...
// <table_2> ...
// ...
// <table_mwl> ...
// По окончании записи закрывает файл
// В случае успешного сохранения возвращает EXIT_SUCCESSFULLY, иначе EXIT_FILE_FAILURE (файл не открылся)
int corrector_save(Corrector* corrector, FILE* model_file);

// Записать слово в соответствующую таблицу в корректоре. Установить счётчик в 1, если это новое слово, или увеличить счётчик существующего слова на 1
void corrector_learn(Corrector* corrector, const char* word);

// Загрузить обученную модель из ОТКРЫТОГО файла в ПУСТУЮ модель
// По окончании загрузки закрыть файл с моделью
// Из файла считывается max_word_length и модель инициализируется. Далее заполняется таблицы заполняются словами из файла
// Если модель успешно загружена - вернуть EXIT_SUCCESSFULLY, если файл не открылся - вернуть EXIT_FILE_FAILURE, если файл имеет неправильный формат - вернуть EXIT_USER_FAILURE
int corrector_load(Corrector* corrector, FILE* model_file);


#endif 

