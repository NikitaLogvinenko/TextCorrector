#include "constants.h"
#include "helpful_functions.h"
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>


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
    setlocale(LC_ALL, "ru");
    printf("HELP!\n");
}

void exit_with_msg(const char* exit_msg, int exit_code)
{
    setlocale(LC_ALL, "ru");
    if (exit_msg != NULL)
        printf("%s", exit_msg);
    if (exit_code == EXIT_USER_FAILURE)
        print_help();
    exit(exit_code);
}

Pointer* empty_pointers_array(unsigned ptrs_amount)
{
    Pointer* array = (Pointer*)retry_malloc(sizeof(Pointer) * ptrs_amount, MAX_MALLOC_ATTEMPTS);
    if (array != NULL) // если память выделилась, устанавливаем указатели в массиве в NULL
    {
        for (unsigned i = 0; i < ptrs_amount; ++i)
            array[i] = NULL;
    }
    return array;
}

Pointer retry_malloc(size_t memory_size, unsigned max_attempts)
{
    Pointer dynamic_memory = NULL;
    for (unsigned attempts = 0; attempts < max_attempts && dynamic_memory == NULL; ++attempts)
        dynamic_memory = (Pointer)malloc(memory_size);
    return dynamic_memory;
}

bool is_integer(char* possible_int)
{
    bool answer = true;
    if (possible_int == NULL || *possible_int == '\0')
        answer = false;
    while (*possible_int != '\0' && answer) // проверяем символы, пока не встретим '\0' или символ, отличный от цифры
    {
        answer *= ((int)*possible_int <= (int)'9') && ((int)*possible_int >= (int)'0');
        ++possible_int;
    }
    return answer;
}

int read_param_from_console(char* buffer, size_t buffer_size)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int read_result = EXIT_SUCCESSFULLY;
    fgets(buffer, buffer_size, stdin);
    int null_term_index = strlen(buffer); // индекс терминирующего нуля
    if (null_term_index == buffer_size - 1 && buffer[null_term_index - 1] != '\n') // не хватило буфера
    {
        read_result = EXIT_MEMORY_FAILURE;
        clear_buff;
    }
    else
    {
        buffer[null_term_index - 1] = '\0'; // заменяем \n на \0
        --null_term_index;
        if (buffer[0] == '"' && buffer[null_term_index - 1] == '"') // кавычки в начале и в конце, удалим их
        {
            memmove(buffer, buffer + 1, null_term_index); // затёрли левую кавычку
            --null_term_index; // т.к. передвинули
            buffer[null_term_index - 1] = '\0'; // удалили правую кавычку
        }
        if (strchr(buffer, '"') != NULL) // в параметрах ещё остались кавычки, значит пользователь некорректно ввёл параметр
            read_result = EXIT_USER_FAILURE;
    }
    return read_result;
}

void clear_buff()
{
    while (getchar() != '\n');
}