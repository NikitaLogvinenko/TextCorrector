#ifndef _PARAMS_WORKING_
#define _PARAMS_WORKING_

typedef void* Pointer;


// Определяем конфигурацию работы программы по введённым параметрам при запуске
// Возвращается динамический массив указателей на конфигурацию
Pointer* set_cfg(int argc, char** argv);

// Чистим данные конфига, т.к. память под него выделялась динамически
void delete_cfg(int mode, Pointer* cfg);

#endif