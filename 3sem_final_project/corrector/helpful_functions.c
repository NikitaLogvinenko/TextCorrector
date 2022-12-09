#include "helpful_functions.h"
#include <stdio.h>
#include <stdbool.h>
#include <locale.h>

bool not_null_ptr(const Pointer ptr, const char* error_msg)
{
    setlocale(LC_ALL, "ru");
    if (ptr != NULL) return true;
    if (error_msg != NULL) printf("%s\n", error_msg);
    return false;
}

bool is_equal(const Pointer data1, size_t data1_size, const Pointer data2, size_t data2_size)
{
    if (data1_size != data2_size) return false; // совпадает размер
    for (int i = 0; i < data1_size; ++i)
    {
        if (*((char*)(data1)+i) != *((char*)(data2)+i)) return false; // совпадают i-е байты
    }
    return true;
}