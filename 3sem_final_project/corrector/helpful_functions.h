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

#endif