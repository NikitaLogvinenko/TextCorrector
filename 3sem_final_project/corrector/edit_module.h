#ifndef _EDIT_MODULE_
#define _EDIT_MODULE_


#include "CorrectorModel.h"


// Редактирует текст в соответствии с конфигом и сохраняет отредактированный текст в новом файле
// Возвращает EXIT_SUCCESSFULLY в случае успешного исполнения, иначе код ошибки:
// * EXIT_MEMORY_FAILURE - не выделилась память
// * EXIT_FILE_FAILURE - не открылся файл
// * EXIT_USER_FILE - ошибка по вине пользователя
int edit_text(const Pointer* cfg);


#endif