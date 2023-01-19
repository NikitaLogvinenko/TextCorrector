#ifndef _TRAIN_MODULE_
#define _TRAIN_MODULE_


#include "CorrectorModel.h"


// Обучает новую модель в соответствии с конфигом и сохраняет её в файл.
// Возвращает EXIT_SUCCESSFULLY в случае успешного исполнения, иначе код ошибки
int train_new_model(const Pointer* cfg);

// Дообучает существующую модель в соответствии с конфигом и сохраняет изменения.
// Возвращает EXIT_SUCCESSFULLY в случае успешного исполнения, иначе код ошибки (EXIT_MEMORY_FAILURE - не выделилась память, EXIT_FILE_FAILURE - не открылся файл)
int train_existed_model(const Pointer* cfg);


#endif