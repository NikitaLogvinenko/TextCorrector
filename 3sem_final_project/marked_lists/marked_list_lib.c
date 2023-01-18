#define _CRT_SECURE_NO_WARNINGS


#include "constants.h"
#include "marked_list.h"
#include "marked_list_lib.h"
#include "helpful_functions.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>



MList* mlist_create(const char* word, unsigned counter)  // предполагается, что память под MList выделяется только динамически
{
    MList* new_ml = NULL;
    if (not_null_ptr(word, "Невозможно выучить слово нулевой длины\n"))
    {
        new_ml = (MList*)retry_malloc(sizeof(MList), MAX_MALLOC_ATTEMPTS);
        if (not_null_ptr(new_ml, "Ошибка выделения памяти под объект MList для хранения слова %s\n", word))
        {
            new_ml->word = word;
            new_ml->counter = counter;
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
        free(ml->word);  // память под слово выделялась динамически
        free(ml);  // предполагается, что память под mlist выделяется только динамически
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

MList* mlist_add(MList* ml, const char* word, unsigned counter)
{
    MList* new_head = ml;
    MList* existing_node = mlist_find(ml, word);
    if (existing_node != NULL) // если слово уже есть - добавить к счётчику counter
    {
        existing_node->counter += counter;
        // Т.к. слова хранятся в динамической памяти, но у нас два участка памяти с одинаковым словом
        // Тогда участок с повторением нужно очистить, т.к. он больше нигде не используется
        // Слова добавляются только при обучении. Мы читаем слово, записываем в таблицу и больше нигде не используем слово отдельно. Если оно не попало в таблицу - оно не нужно
        free(word);
    }
    else  // если нет - добавляем новое слово в список
    {
        new_head = mlist_create(word, counter);
        if (not_null_ptr(new_head, "Ошибка выделения памяти при добавлении слова\n")) // если память под новое слово выделилась - делаем его головой списка
            new_head->next = ml;
        else
        {
            new_head = ml;  // если память не выделилась, то голова остаётся прежней
            free(word);  // чистим динамическую память, выделявшуюся под слово, т.к. она больше не нужна (не смогли записать слово)
        }
    }
    return new_head;
}

unsigned* mlist_get(const MList* ml, const char* word)
{
    unsigned* word_count = NULL;
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

    if (ml != NULL && word != NULL)  // есть что и где искать
    { 
        if (strcmp(word, ml->word) == 0)  // если слово в голове, то запоминаем следующий элемент (новую голову), просто отвязываем голову от остатка и удаляем
        {
            new_head = ml->next;
            ml->next = NULL;
            mlist_destroy(ml);
        }
        else  // если удаляем не голову, то голова остаётся прежней, а удаляемый узел удаляем от предыдущей и последующей части и удаляем
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

void mlist_foreach_counter(MList* ml, void (*func)(unsigned* number))
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

void mlist_foreach_item(MList* ml, void (*func)(const char* word, unsigned* number))
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
        printf("%s: %u\n", ml->word, ml->counter);
        ml = ml->next;
    }
    printf("-------------------------------------------\n");
}