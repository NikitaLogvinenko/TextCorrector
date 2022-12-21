#ifndef _TRAIN_MODULE_
#define _TRAIN_MODULE_


#include "CorrectorModel.h"


// Обучает новую модель в соответствии с конфигом и сохраняет её в файл.
// Возвращает 0 в случае успешного исполнения, иначе код ошибки
int train_new_model(const Pointer* cfg);

// Дообучает существующую модель в соответствии с конфигом и сохраняет изменения.
// Возвращает 0 в случае успешного исполнения, иначе код ошибки
int train_existed_model(const Pointer* cfg);

#endif