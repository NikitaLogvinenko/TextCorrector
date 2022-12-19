#include "helpful_functions.h"
#include <stdio.h>
#include <stdbool.h>
#include <locale.h>
#include <string.h>


// Проверяет, что указатель не нулевой. Если нулевой - вывести сообщение об ошибке error_msg (не открылся файл, не выделилась память и тд)
bool not_null_ptr(const Pointer ptr, const char* error_msg)
{
    setlocale(LC_ALL, "ru");
    if (ptr != NULL) return true;

    if (error_msg != NULL) printf("%s\n", error_msg);
    return false;
}

// Проверяет, что данные по указателям побайтово совпадают
bool is_equal(const Pointer data1, size_t data1_size, const Pointer data2, size_t data2_size)
{
    if (data1_size != data2_size) return false; // не совпадает размер данных

    const char* char_ptr1 = (char*)data1;
    const char* char_ptr2 = (char*)data2;
    bool data_is_equal = (bool)(strncmp(char_ptr1, char_ptr2, data1_size) == 0);  // побайтовое сравнение

    return data_is_equal;
}