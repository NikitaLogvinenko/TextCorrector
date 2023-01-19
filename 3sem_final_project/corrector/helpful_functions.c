#include "constants.h"
#include "helpful_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


bool not_null_ptr(const Pointer ptr, const char* error_msg)
{
    if (ptr != NULL) return true;
    if (error_msg != NULL) 
        printf("%s", error_msg);
    return false;
}

bool datas_are_equal(const Pointer data1, size_t data1_size, const Pointer data2, size_t data2_size)
{
    if (data1_size != data2_size) return false; // не совпадает размер данных
    bool datas_are_equal = (bool)(memcmp(data1, data2, data1_size) == 0);  // побайтовое сравнение
    return datas_are_equal;
}

bool ints_are_equal(int* number_1, int* number_2)
{
    return *number_1 == *number_2;
}

void multiply_int(int* data, const int* multiplier)
{
    *data *= *multiplier;
}

void print_help()
{
    printf("\t\tWARNING: ИМЕНА ФАЙЛОВ НЕ ДОЛЖНЫ СОДЕРЖАТЬ `%s`, ИНАЧЕ ОНИ МОГУТ БЫТЬ УДАЛЕНЫ!!!", TMP_SUFFIX);
    printf("\n3sem_final_project.exe [set_cfg_way] [appropriate_params]\n");
    printf("Все параметры вводятся через пробел. Допустимо заключать параметры в двойные кавычки\n\n");
    printf("Возможные варианты set_cfg_way:\n");
    printf("\t1. params (задать конфигурацию в параметрах)\n\t2. step_by_step (последовательно ввести конфигурацию в консоль)\n");
    printf("\t3. from_file(конфигурация записана в файле\n\t4. help (вывести справку по программе)\n");
    printf("Полное отсутствие параметров эквивалентно 2. step_by_step\n\n");
    printf("В случае 3. from_file в appropriate_params должен быть только путь к файлу с конфигурацией. Файл должен содержать записи параметров аналогично params (см. далее), параметры могут быть заключены в ДВОЙНЫЕ кавычки\n\n");
    printf("В случае 1. params в первую очередь должно идти название режима работы программы:\n");
    printf("\t1. train_new (обучение новой модели)\n\t2. train_existed (дообучение существующей модели)\n\t3. edit (редактирование текста с помощью обученной модели)\n\n");
    printf("В режиме train_new необходимы следующие параметры:\n");
    printf("\t1. Путь для сохранения новой модели\n\t2. Путь к обучающему тексту\n\t3. Максимальная длина запоминаемых слов (`max_word_length`)\n\n");
    printf("В режиме train_existed необходимы следующие параметры:\n");
    printf("\t1. Путь к существующей модели\n\t2. Путь к обучающему тексту\n\n");
    printf("В режиме edit необходимы следующие параметры:\n");
    printf("\t1. Путь к обученной модели\n\t2. Путь к редактируемому тексту\n\t3. Путь для сохранения отредактированного текста\n");
    printf("\t4. Максимально допустимое отклонение длины заменяющего слова от заменяемого (`size_tol`)\n\t5. Максимально допустимая разность слов (`threshold`)\n\n");
    printf("Более подробная информация доступна по ссылке https://github.com/NikitaLogvinenko/TextCorrector.git\n");
    printf("ПРИЯТНОГО ПОЛЬЗОВАНИЯ ПРИЛОЖЕНИЕМ!\n");
}

void exit_with_msg(const char* exit_msg, int exit_code)
{
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
    while (answer && *possible_int != '\0') // проверяем символы, пока не встретим '\0' или символ, отличный от цифры
    {
        answer *= ((int)*possible_int <= (int)'9') && ((int)*possible_int >= (int)'0');
        ++possible_int;
    }
    return answer;
}

int read_param_from_console(char* buffer, size_t buffer_size)
{
    int read_result = EXIT_SUCCESSFULLY;
    fgets(buffer, buffer_size, stdin);
    int null_term_index = strlen(buffer); // индекс терминирующего нуля
    if (null_term_index == buffer_size - 1 && buffer[null_term_index - 1] != '\n') // не хватило буфера (последний - '\0', а предпоследний не '\n')
    {
        read_result = EXIT_MEMORY_FAILURE;
        clear_buff();
    }
    else
    {
        buffer[null_term_index - 1] = '\0'; // заменяем \n на \0
        --null_term_index;  // обновляем индекс нуль-терминатора
        if (buffer[0] == '"' && buffer[null_term_index - 1] == '"') // кавычки в начале и в конце, удалим их
        {
            memmove(buffer, buffer + 1, null_term_index); // затёрли левую кавычку
            --null_term_index; // обновляем инлекс нуль-терминатора, т.к. передвинули
            buffer[null_term_index - 1] = '\0'; // удалили правую кавычку
        }
        if (strchr(buffer, '"') != NULL) // в параметрах ещё остались кавычки, значит пользователь некорректно ввёл параметр (кавычки нигде не предусмотрены)
            read_result = EXIT_USER_FAILURE;
    }
    return read_result;
}

void clear_buff()
{
    while (getchar() != '\n');
}

void print_train_new_cfg(Pointer* cfg)
{
    printf("\n>>>>>>>>>>>>>>>> ТЕКУЩАЯ КОНФИГУРАЦИЯ <<<<<<<<<<<<<<<<\n\n");
    printf("1. Режим: train_new\n");
    printf("2. Путь для сохранения модели: %s\n", (const char*)cfg[1]);
    printf("3. Путь к обучающему тексту: %s\n", (const char*)cfg[2]);
    printf("4. Максимальная длина запоминаемых слов: %d\n", *(int*)cfg[3]);
    printf("\n<<<<<<<<<<<<<<<< -------------------- >>>>>>>>>>>>>>>>\n\n");
}

void print_train_existed_cfg(Pointer* cfg)
{
    printf("\n>>>>>>>>>>>>>>>> ТЕКУЩАЯ КОНФИГУРАЦИЯ <<<<<<<<<<<<<<<<\n\n");
    printf("1. Режим: train_existed\n");
    printf("2. Путь к существующей модели: %s\n", (const char*)cfg[1]);
    printf("3. Путь к обучающему тексту: %s\n", (const char*)cfg[2]);
    printf("\n<<<<<<<<<<<<<<<< -------------------- >>>>>>>>>>>>>>>>\n\n");
}

void print_edit_cfg(Pointer* cfg)
{
    printf("\n>>>>>>>>>>>>>>>> ТЕКУЩАЯ КОНФИГУРАЦИЯ <<<<<<<<<<<<<<<<\n\n");
    printf("1. Режим: edit\n");
    printf("2. Путь к обученной модели: %s\n", (const char*)cfg[1]);
    printf("3. Путь к редактируемому тексту: %s\n", (const char*)cfg[2]);
    printf("4. Путь для сохранения отредактированного текста: %s\n", (const char*)cfg[3]);
    printf("5. Максимально допустимое отклонение длины заменяющего слова от заменяемого: %d\n", *(int*)cfg[4]);
    printf("6. Максимально допустимая разность слов: %d\n", *(int*)cfg[5]);
    printf("\n<<<<<<<<<<<<<<<< -------------------- >>>>>>>>>>>>>>>>\n\n");
}

bool yes_no_question()
{
    printf("[Y]/[N]: ");
    char ans = getchar();
    while (ans != 'Y' && ans != 'N')
    {
        while (getchar() != '\n');  // чистка буфера
        printf("Введите Y или N: ");
        ans = getchar();
    }
    while (getchar() != '\n');  // удалить оставшиеся в буфере символы
    if (ans == 'Y') return true;
    return false;
}