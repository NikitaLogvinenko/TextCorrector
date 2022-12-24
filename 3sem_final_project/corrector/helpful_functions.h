#ifndef _HELPFUL_FUNCTIONS_
#define _HELPFUL_FUNCTIONS_


#include <stdio.h>
#include <stdbool.h>


typedef void* Pointer;


// Проверяет, что указатель не нулевой. Если нулевой - вывести сообщение об ошибке error_msg (не открылся файл, не выделилась память и тд)
bool not_null_ptr(const Pointer ptr, const char* error_msg);

// Проверяет, что данные по указателям побайтово совпадают (с учётом размеров данных)
bool datas_are_equal(const Pointer data1, size_t data1_size, const Pointer data2, size_t data2_size);

// Проверка, что два числа равны
bool ints_are_equal(int* number_1, int* number_2);

// умножить data на multiplier
void multiply_int(int* data, const int* multiplier);

// Вывести справку
void print_help();

// Написать сообщение exit_msg и выйти
// Если ошибка по вине пользователя и она предсказывалась при разработке, то exit_code=EXIT_SOFT_FAILURE. Тогда ещё выводится справка.
void exit_with_msg(const char* exit_msg, int exit_code);

// Создать динамический массив из ptrs_amount указателей void*. Инициализировать указатели NULL. В случае ошибки выделения памяти возвращается NULL
Pointer* empty_pointers_array(unsigned ptrs_amount);

// Выделение памяти размером memory_size, либо max_attempts попыток, либо пока память не выделится (что произойдёт раньше). В случае неудачи вернёт NULL.
Pointer retry_malloc(size_t memory_size, unsigned max_attempts);

// Проверяет, что в строке possible_int только цифры и ничего больше (проверка идёт, пока не встретится '\0')
// Если передан NULL или нет нуль-терминатора, или наоборот только нуль-терминатор (пустая строка), то возвращается false
bool is_integer(char* possible_int);

// Считать параметр с консоли с помощью fgets и записать в буфер размером buffer_size.
// Если длина записанной строки равна (buffer_size-1) (не забываем про нуль-терминатор), то последний символ равен \0 => проверяем предпоследний символ.
// Если предпоследний символ \n, значит смогли считать всю строку, если же предпоследний символ не \n, то считали не всё (возможно, как раз \n и не считали).
// Если не хватило памяти, то делаем очистку буфера и возвращаем EXIT_MEMORY_FAILURE.
// Если памяти хватило, то, во-первых, удаляем \n в конце (заменяем на \0). Во-вторых, проверяем, если строка начинается и заканчивается кавычками, то удаляем их.
// Если кавычек нет или они парные - возвращаем EXIT_SUCCESSFULLY. Если кавычки только одни - возвращаем EXIT_USER_FAILURE
// Если после всех операций в buffer остануться кавычки - вернуть EXIT_USER_FAILURE
int read_param_from_console(char* buffer, size_t buffer_size);

// Очистить буфер (считать все символы из stdin, пока не встретит \n)
void clear_buff();

// Вывести конфигурацию работы программы. Предполагается, что она верная
void print_train_new_cfg(Pointer* cfg);
// Вывести конфигурацию работы программы. Предполагается, что она верная
void print_train_existed_cfg(Pointer* cfg);
// Вывести конфигурацию работы программы. Предполагается, что она верная
void print_edit_cfg(Pointer* cfg);

#endif