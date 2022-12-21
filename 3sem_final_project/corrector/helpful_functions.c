#include "helpful_functions.h"
#include "constants.h"
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


bool not_null_ptr(const Pointer ptr, const char* error_msg)
{
    setlocale(LC_ALL, "ru");
    if (ptr != NULL) return true;

    if (error_msg != NULL) printf("%s\n", error_msg);
    return false;
}

bool datas_are_equal(const Pointer data1, size_t data1_size, const Pointer data2, size_t data2_size)
{
    if (data1_size != data2_size) return false; // не совпадает размер данных

    const char* char_ptr1 = (char*)data1;
    const char* char_ptr2 = (char*)data2;
    bool data_is_equal = (bool)(strncmp(char_ptr1, char_ptr2, data1_size) == 0);  // побайтовое сравнение

    return data_is_equal;
}

bool ints_are_equal(int* number_1, int* number_2)
{
    return *number_1 == *number_2;
}

void multiply_int(int* data, const int* multiplier)
{
    *data *= *multiplier;
}

void print_help()//---------------------------------------------------------------------------------------------
{
    printf("HELP!\n");
}

void exit_soft(const char* exit_msg)
{
    printf("%s\n", exit_msg);
    print_help();
    exit(EXIT_SOFT_FAILURE);
}