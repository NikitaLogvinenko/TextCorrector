#ifndef _CORRECTOR_LIB_
#define _CORRECTOR_LIB_


#include "CorrectorModel.h"


// Инициализация корректора. Создание таблиц. Установка функции поиска разности слов (стандартно - расстояние Хэмминга)
// В случае успешного выделения памяти под таблицы возвращает EXIT_SUCCESSFULLY, иначе EXIT_MEMORY_FAILURE
int corrector_init(Corrector* corrector, unsigned max_word_length, WordsMetric metric_func);

// Очистка таблиц в корректоре. Обратите внимание, чистятся только таблицы. Если память под сам корректор выделялась динамически - необходимо самостотельно выполнить free(corrector)
void corrector_destroy(Corrector* corrector);


#endif 

