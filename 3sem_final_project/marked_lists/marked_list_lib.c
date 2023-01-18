#define _CRT_SECURE_NO_WARNINGS


#include "constants.h"
#include "marked_list.h"
#include "marked_list_lib.h"
#include "helpful_functions.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>



MList* mlist_create(const char* word)  // ��������������, ��� ������ ��� Mlist ���������� ������ �����������
{
    MList* new_ml = NULL;
    if (not_null_ptr(word, "���������� ������� ����� ������� �����\n"))
    {
        new_ml = (MList*)retry_malloc(sizeof(MList), MAX_MALLOC_ATTEMPTS);
        if (not_null_ptr(new_ml, "������ ��������� ������ ��� ������ MList ��� �������� ����� %s\n", word))
        {
            new_ml->word = word;
            new_ml->counter = 1;
            new_ml->next = NULL;
        }
    }
    return new_ml;
}

void mlist_destroy(MList* ml)
{
    if (ml != NULL)
    {
        mlist_destroy(ml->next);
        free(ml->word);  // ������ ��� ����� ���������� �����������
        free(ml);  // ��������������, ��� ������ ��� mlist ���������� ������ �����������
    }
}

unsigned mlist_length(const MList* ml)
{
    unsigned length = 0;
    while (ml != NULL)
    {
        length += 1;
        ml = ml->next;
    }
    return length;
}

MList* mlist_add(MList* ml, const char* word)
{
    MList* new_head = ml;
    MList* existing_node = mlist_find(ml, word);
    if (existing_node != NULL) // ���� ����� ��� ���� - �������� � �������� 1
    {
        existing_node->counter += 1;
        // �.�. ����� �������� � ������������ ������, �� � ��� ��� ������� ������ � ���������� ������
        // ����� ������� � ����������� ����� ��������, �.�. �� ������ ����� �� ������������
        // ����� ����������� ������ ��� ��������. �� ������ �����, ���������� � ������� � ������ ����� �� ���������� ����� ��������. ���� ��� �� ������ � ������� - ��� �� �����
        free(word);
    }
    else  // ���� ��� - ��������� ����� ����� � ������
    {
        new_head = mlist_create(word);
        if (not_null_ptr(new_head, "������ ��������� ������ ��� ���������� �����\n")) // ���� ������ ��� ����� ����� ���������� - ������ ��� ������� ������
            new_head->next = ml;
        else
        {
            new_head = ml;  // ���� ������ �� ����������, �� ������ ������� �������
            free(word);  // ������ ������������ ������, ������������ ��� �����, �.�. ��� ������ �� �����
        }
    }
    return new_head;
}

int* mlist_get(const MList* ml, const char* word)
{
    int* word_count = NULL;
    if (word != NULL)
    {
        while (ml != NULL && word_count == NULL)
        {
            if (strcmp(word, ml->word) == 0)
                word_count = &(ml->counter);
            ml = ml->next;
        }
    }
    return word_count;
}

bool mlist_has(const MList* ml, const char* word)
{
    bool answer = false;
    if (word != NULL)
    {
        while (ml != NULL && answer == false)
        {
            if (strcmp(word, ml->word) == 0) 
                answer = true;
            ml = ml->next;
        }
    }
    return answer;
}

MList* mlist_find(const MList* ml, const char* word)
{
    MList* appropriate_mlist = NULL;
    if (word != NULL)
    {
        while (ml != NULL && appropriate_mlist == NULL)
        {
            if (strcmp(word, ml->word) == 0)
                appropriate_mlist = ml;
            ml = ml->next;
        }
    }
    return appropriate_mlist;
}

MList* mlist_remove(MList* ml, const char* word)
{
    MList* new_head = ml;

    if (ml != NULL && word != NULL)  // ���� ��� � ��� ������
    { 
        if (strcmp(word, ml->word) == 0)  // ���� ����� � ������, �� ���������� ��������� ������� (����� ������), ������ ���������� ������ �� ������� � �������
        {
            new_head = ml->next;
            ml->next = NULL;
            mlist_destroy(ml);
        }
        else  // ���� ������� �� ������, �� ������ ������� �������, � ��������� ���� ������� �� ���������� � ����������� ����� � �������
        {
            MList* cur_node = ml;
            while (cur_node->next != NULL)
            {
                if (strcmp(word, cur_node->next->word) == 0)
                {
                    MList* removed_node = cur_node->next;
                    cur_node->next = removed_node->next;
                    removed_node->next = NULL;
                    mlist_destroy(removed_node);
                    break;
                }
                cur_node = cur_node->next;
            }
        }
    }

    return new_head;
}

void mlist_foreach_counter(MList* ml, void (*func)(int* number))
{
    if (func != NULL)
    {
        while (ml != NULL)
        {
            func(&(ml->counter));
            ml = ml->next;
        }
    }
}

void mlist_foreach_item(MList* ml, void (*func)(const char* word, int* number))
{
    if (func != NULL)
    {
        while (ml != NULL)
        {
            func(ml->word, &(ml->counter));
            ml = ml->next;
        }
    }
}

void print_mlist(MList* ml)
{
    printf("-------------------------------------------\n");
    while (ml != NULL)
    {
        printf("%s: %d\n", ml->word, ml->counter);
        ml = ml->next;
    }
    printf("-------------------------------------------\n");
}