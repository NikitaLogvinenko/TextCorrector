#define NULL_KEY_ERROR -2
#define NOT_INITIALISED_TABLE -1
#define WRITING_FAILED -1
#define SUCCESSFULLY_WRITTEN 0
#define WRDS_EXCEED 10
#define HT_INCRS 2


#include "constants.h"
#include "marked_list.h"
#include "marked_list_lib.h"
#include "hash_table.h"
#include "hash_table_lib.h"
#include "helpful_functions.h"
#include <stdbool.h>
#include <stdlib.h>


// ���������, ��� ������� ����������������
static bool table_was_initialised(HashTable* ht);

// �������� ������ ������, � ������� ������ ���� ���� word; ���� ������� �� ���������������� - ������� -1, ���� word=NULL - ������� -2
// ���-������� ���������� ��� ��������� ��������, �� ��� ����� ��������� ����� � �������� �������. �������������, �� ���� ���� ����� ������� �� ������� �� ������ �������
static int get_list_index(HashTable* ht, const char* word);

// ����������� ������ �� ������ ������� � ����� (������� �����������, �������������, ������������ ������� ���������)
// ����� ������� ������ ���� ������
static void displace_table(HashFunction hf, MList** new_table, unsigned new_size, MList** old_table, unsigned old_size);


unsigned jenkins_one_at_a_time_hash(const char* key)
{
    unsigned hash = 0;

    for (; *key; ++key) {
        hash += *key;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

int ht_init(HashTable* ht, unsigned size, HashFunction hf)
{
    int func_res = EXIT_SUCCESSFULLY;

    if (not_null_ptr(ht, "���������� ���������������� ������� �� ��������� NULL\n"))
    {
        if (size < 1)
            printf("���������� ������� ������ �������\n");
        else
        {
            ht->table = (MList**)retry_malloc(sizeof(MList*) * size, MAX_MALLOC_ATTEMPTS);  // ������ ������������ ������ �� size ���������� �� ������
            if (not_null_ptr(ht->table, "������ ��������� ������ ��� �������. ������������� �� ����������\n"))
            {
                for (unsigned i = 0; i < size; ++i)
                    ht->table[i] = NULL;  // ������ ��� ��������� � NULL
                ht->size = size;  // ���������� ������ �������
                ht->total_words = 0;  // ���� ��� �� ��������� ������� ���� � �������
                // ������������� ������� �����������
                if (hf == NULL)
                    hf = jenkins_one_at_a_time_hash;
                ht->hashfunc = hf;
            }
            else
                func_res = EXIT_MEMORY_FAILURE;
        }
    }
    return func_res;
}

void ht_destroy(HashTable* ht)
{
    if (ht != NULL)
    {
        MList* removed_list = NULL;
        for (unsigned i = 0; i < ht->size; ++i)
        {
            removed_list = ht->table[i];  // ������ ���� ������ � ���������� ���-������; ����� ����� ��� �������
            mlist_destroy(removed_list);  // ������� ������ (���� word ��������� ���� ������ mlist_destroy; ���� counter ���������� ����������� ������)
        }
        free(ht->table);
        // �������� ������ ����� HashTable ������������ ��������� �������. ����� �� ������� ������ ��� ������ ���-������� ����������
    }
}

void ht_set(HashTable* ht, const char* word, unsigned counter)
{
    if (table_was_initialised(ht) == false)
        printf("������� ��� ������� ����� �� ����������������\n");

    else if (word == NULL)
        printf("������������ ���� ��� ������� � �������!\n");

    else
    {
        if (ht_has(ht, word) == false)  // ���� ��� ����� �����, �� ���� ��������� ������� ���� � �������
        {
            ht->total_words += 1;
            if (ht->total_words >= ht->size * WRDS_EXCEED)  // ������� ����� ���� � �������, �������� ������� ��������, �������� ���������� ����� � �������
                ht_resize(ht, ht->size * HT_INCRS);
        }
        unsigned list_index = get_list_index(ht, word);  // ������ ������� ������
        if (ht->table[list_index] == NULL)  // � ���� ������� � ������� ������ ��� �� ������, ������ ����� (� ������ � ����� ��� ������ �� �����������)
            ht->table[list_index] = mlist_create(word, counter);

        else  // ������ � ������� ��� ����������, ��������� � ���� word (���� ����� ��� ���� � ������ �� �����, ��� �� �������� ������ mlist_add
            ht->table[list_index] = mlist_add(ht->table[list_index], word, counter);
    }
}

unsigned* ht_get(const HashTable* ht, const char* word)
{
    int list_index = get_list_index(ht, word);
    MList* ml_with_word = (list_index >= 0) ? ht->table[list_index] : NULL;
    return mlist_get(ml_with_word, word);
}

bool ht_has(const HashTable* ht, const char* word)
{
    int list_index = get_list_index(ht, word);
    MList* ml_for_word = (list_index >= 0) ? ht->table[list_index] : NULL;
    return mlist_has(ml_for_word, word);
}

void ht_delete(HashTable* ht, const char* word)
{
    int list_index = get_list_index(ht, word);
    if (list_index >= 0)
    {
        MList* removed_elem = mlist_find(ht->table[list_index], word);
        if (removed_elem != NULL)
            ht->table[list_index] = mlist_remove(ht->table[list_index], word);
    }
}

void ht_traverse(HashTable* ht, void (*func)(const char* word, unsigned* counter))
{
    if (table_was_initialised(ht) == false)
        printf("������� ������ �������������������� �������\n");
    else
    {
        MList* cur_mlist = NULL;
        for (unsigned i = 0; i < ht->size; ++i)
        {
            cur_mlist = ht->table[i];
            mlist_foreach_item(cur_mlist, func);
        }
    }
}

void ht_resize(HashTable* ht, unsigned new_size)
{
    if (table_was_initialised(ht) == false)
        printf("������� ��������� ������� �������������������� �������\n");

    else if (new_size < 1)
        printf("������� �������� ������ �������\n");

    MList** new_table = (MList**)retry_malloc(sizeof(MList*) * new_size, MAX_MALLOC_ATTEMPTS);
    if (not_null_ptr(ht->table, "������ ��������� ������ ��� �������. ��������� ������� ������� �� ����������\n"))
    {
        displace_table(ht->hashfunc, new_table, new_size, ht->table, ht->size);  // ���������� ������ �� ������ ������� � �����
        free(ht->table);  // ������� ������ ��������� (��� ��� ���������� ������������ ������)
        ht->table = new_table;  // ��������� �� ����� �������
        ht->size = new_size;  // ������ ������ �������
    }
}

void fprint_ht(FILE* opened_file, HashTable* ht, const char* table_name)
{
    if (opened_file != NULL && table_was_initialised(ht))
    {
        fprintf(opened_file, "<%s> size=%d total_words=%d\n", table_name, ht->size, ht->total_words);
        for (unsigned row_index = 0; row_index < ht->size; ++row_index)
            fprint_row(opened_file, ht->table[row_index], row_index);
    }
}

void fprint_row(FILE* opened_file, MList* ml, unsigned row_index)
{
    if (opened_file != NULL)
    {
        unsigned row_length = mlist_length(ml);
        fprintf(opened_file, "[row_%d] length=%d\n", row_index, row_length);
        for (unsigned word_index = 0; word_index < row_length; ++word_index)
        {
            fprintf(opened_file, "%s %d\n", ml->word, ml->counter);
            ml = ml->next;
        }
    }
}


static bool table_was_initialised(HashTable* ht)
{
    if ((ht == NULL) || (ht->hashfunc) == NULL || (ht->table) == NULL || (ht->size < 1)) return false;
    return true;
}

static int get_list_index(HashTable* ht, const char* word)
{
    int index = 0;
    if (table_was_initialised(ht) == false)
        index = NOT_INITIALISED_TABLE;
    else if (word == NULL)
        index = NULL_KEY_ERROR;
    else
        index = ht->hashfunc(word) % ht->size;
    return index;
}

static void displace_table(HashFunction hf, MList** new_table, unsigned new_size, MList** old_table, unsigned old_size)
{
    for (unsigned i = 0; i < new_size; ++i)
        new_table[i] = NULL;

    MList* cur_mlist = NULL, *without_head = NULL;
    for (unsigned old_index = 0; old_index < old_size; ++old_index)
    {
        cur_mlist = old_table[old_index];  // ��������� ������������ ������
        old_table[old_index] = NULL;  // ������ ������� ������ ��� �� ������
        // ������ ����������� �������� ������ � ����� �������
        while (cur_mlist != NULL)
        {
            // ���������� ������
            without_head = cur_mlist->next;
            cur_mlist->next = NULL;
            // ������� � � ����� ������� � ������ ��������������� ������ (������)
            unsigned new_index = hf(cur_mlist->word) % new_size;
            cur_mlist->next = new_table[new_index];
            new_table[new_index] = cur_mlist;
            cur_mlist = without_head;
        }
    }
}