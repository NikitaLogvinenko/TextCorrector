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
    if (data1_size != data2_size) return false; // �� ��������� ������ ������
    bool datas_are_equal = (bool)(memcmp(data1, data2, data1_size) == 0);  // ���������� ���������
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
    printf("\t\tWARNING: ����� ������ �� ������ ��������� `%s`, ����� ��� ����� ���� �������!!!", TMP_SUFFIX);
    printf("\n3sem_final_project.exe [set_cfg_way] [appropriate_params]\n");
    printf("��� ��������� �������� ����� ������. ��������� ��������� ��������� � ������� �������\n\n");
    printf("��������� �������� set_cfg_way:\n");
    printf("\t1. params (������ ������������ � ����������)\n\t2. step_by_step (��������������� ������ ������������ � �������)\n");
    printf("\t3. from_file(������������ �������� � �����\n\t4. help (������� ������� �� ���������)\n");
    printf("������ ���������� ���������� ������������ 2. step_by_step\n\n");
    printf("� ������ 3. from_file � appropriate_params ������ ���� ������ ���� � ����� � �������������. ���� ������ ��������� ������ ���������� ���������� params (��. �����), ��������� ����� ���� ��������� � ������� �������\n\n");
    printf("� ������ 1. params � ������ ������� ������ ���� �������� ������ ������ ���������:\n");
    printf("\t1. train_new (�������� ����� ������)\n\t2. train_existed (���������� ������������ ������)\n\t3. edit (�������������� ������ � ������� ��������� ������)\n\n");
    printf("� ������ train_new ���������� ��������� ���������:\n");
    printf("\t1. ���� ��� ���������� ����� ������\n\t2. ���� � ���������� ������\n\t3. ������������ ����� ������������ ���� (`max_word_length`)\n\n");
    printf("� ������ train_existed ���������� ��������� ���������:\n");
    printf("\t1. ���� � ������������ ������\n\t2. ���� � ���������� ������\n\n");
    printf("� ������ edit ���������� ��������� ���������:\n");
    printf("\t1. ���� � ��������� ������\n\t2. ���� � �������������� ������\n\t3. ���� ��� ���������� ������������������ ������\n");
    printf("\t4. ����������� ���������� ���������� ����� ����������� ����� �� ����������� (`size_tol`)\n\t5. ����������� ���������� �������� ���� (`threshold`)\n\n");
    printf("����� ��������� ���������� �������� �� ������ https://github.com/NikitaLogvinenko/TextCorrector.git\n");
    printf("��������� ����������� �����������!\n");
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
    if (array != NULL) // ���� ������ ����������, ������������� ��������� � ������� � NULL
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
    while (answer && *possible_int != '\0') // ��������� �������, ���� �� �������� '\0' ��� ������, �������� �� �����
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
    int null_term_index = strlen(buffer); // ������ �������������� ����
    if (null_term_index == buffer_size - 1 && buffer[null_term_index - 1] != '\n') // �� ������� ������ (��������� - '\0', � ������������� �� '\n')
    {
        read_result = EXIT_MEMORY_FAILURE;
        clear_buff();
    }
    else
    {
        buffer[null_term_index - 1] = '\0'; // �������� \n �� \0
        --null_term_index;  // ��������� ������ ����-�����������
        if (buffer[0] == '"' && buffer[null_term_index - 1] == '"') // ������� � ������ � � �����, ������ ��
        {
            memmove(buffer, buffer + 1, null_term_index); // ������ ����� �������
            --null_term_index; // ��������� ������ ����-�����������, �.�. �����������
            buffer[null_term_index - 1] = '\0'; // ������� ������ �������
        }
        if (strchr(buffer, '"') != NULL) // � ���������� ��� �������� �������, ������ ������������ ����������� ��� �������� (������� ����� �� �������������)
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
    printf("\n>>>>>>>>>>>>>>>> ������� ������������ <<<<<<<<<<<<<<<<\n\n");
    printf("1. �����: train_new\n");
    printf("2. ���� ��� ���������� ������: %s\n", (const char*)cfg[1]);
    printf("3. ���� � ���������� ������: %s\n", (const char*)cfg[2]);
    printf("4. ������������ ����� ������������ ����: %d\n", *(int*)cfg[3]);
    printf("\n<<<<<<<<<<<<<<<< -------------------- >>>>>>>>>>>>>>>>\n\n");
}

void print_train_existed_cfg(Pointer* cfg)
{
    printf("\n>>>>>>>>>>>>>>>> ������� ������������ <<<<<<<<<<<<<<<<\n\n");
    printf("1. �����: train_existed\n");
    printf("2. ���� � ������������ ������: %s\n", (const char*)cfg[1]);
    printf("3. ���� � ���������� ������: %s\n", (const char*)cfg[2]);
    printf("\n<<<<<<<<<<<<<<<< -------------------- >>>>>>>>>>>>>>>>\n\n");
}

void print_edit_cfg(Pointer* cfg)
{
    printf("\n>>>>>>>>>>>>>>>> ������� ������������ <<<<<<<<<<<<<<<<\n\n");
    printf("1. �����: edit\n");
    printf("2. ���� � ��������� ������: %s\n", (const char*)cfg[1]);
    printf("3. ���� � �������������� ������: %s\n", (const char*)cfg[2]);
    printf("4. ���� ��� ���������� ������������������ ������: %s\n", (const char*)cfg[3]);
    printf("5. ����������� ���������� ���������� ����� ����������� ����� �� �����������: %d\n", *(int*)cfg[4]);
    printf("6. ����������� ���������� �������� ����: %d\n", *(int*)cfg[5]);
    printf("\n<<<<<<<<<<<<<<<< -------------------- >>>>>>>>>>>>>>>>\n\n");
}

bool yes_no_question()
{
    printf("[Y]/[N]: ");
    char ans = getchar();
    while (ans != 'Y' && ans != 'N')
    {
        while (getchar() != '\n');  // ������ ������
        printf("������� Y ��� N: ");
        ans = getchar();
    }
    while (getchar() != '\n');  // ������� ���������� � ������ �������
    if (ans == 'Y') return true;
    return false;
}