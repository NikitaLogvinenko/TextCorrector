//#define NOT_INITIALISED_TABLE -1
//#define NULL_KEY_ERROR -2
//
//#include "marked_list.h"
//#include "marked_list_lib.h"
//#include "hash_table_lib.h"
//#include "hash_table.h"
//#include <stdbool.h>
//#include <stdlib.h>
//#include <locale.h>
//
//
//// ���������, ��� ��������� �� �������, ����� ������� ��������� error_msg
//static bool not_null_ptr(const Pointer ptr, const char* error_msg);
//
//// ���-������� ���������
//static unsigned jenkins_one_at_a_time_hash(const char* key);
//
//// ���������, ��� ������� ����������������
//static bool table_was_initialised(HashTable* ht);
//
//// ������ ������ � ����������� ���������� ��������
//static bool yes_no_question();
//
//// �������� ������ ������, � ������� ������ ���� ���� key; ���� ������� �� ���������������� - ������� -1
//static int get_list_index(HashTable* ht, const char* key);
//
//// ����������� ������ �� ������ ������� � ����� (������� �����������, �������������, ������������ ������� ���������)
//// ����� ������� ������ ���� ������
//static void displace_table(HashFunction hf, MList** new_table, size_t new_size, MList** old_table, size_t old_size);
//
//
//void ht_init(HashTable* ht, size_t size, HashFunction hf, Destructor dtor)
//{
//    if (!not_null_ptr(ht, "���������� ���������������� ������� �� ��������� NULL\n")) return;
//
//    if (size < 1)
//    {
//        printf("���������� ������� ������ �������\n");
//        return;
//    }
//
//    ht->table = (MList**)malloc(sizeof(MList*) * size);
//    if (!not_null_ptr(ht->table, "������ ��������� ������ ��� �������. ������������� �� ����������\n")) return;
//
//    for (int i = 0; i < size; ++i)
//    {
//        ht->table[i] = NULL;
//    }
//        
//    ht->size = size;
//    
//    if (!not_null_ptr(hf, "���-������� �� ������. ������������ ������� ���������\n")) hf = jenkins_one_at_a_time_hash;
//    ht->hashfunc = hf;
//    not_null_ptr(dtor, "������� �������� �������� ������ �� ������. �������� ������ ������\n");
//    ht->dtor = dtor;
//}
//
//void ht_destroy(HashTable* ht)
//{
//    if (ht == NULL) return;
//
//    MList* removed_list = NULL;
//    for (int i = 0; i < ht->size; ++i)
//    {
//        removed_list = ht->table[i];  // ������ ���� ������ � ���������� ���-������
//        mlist_foreach_data(removed_list, ht->dtor);  // ������� ��� ���� data � dtor (��� ����� ���� ����������� � ����� ���� � ������������ ���������� ������)
//        mlist_destroy(removed_list);  // ������� ������ (���������� ��� �� ������������ ������ ����� data, ������� ������ ����� dtor)
//    }
//
//    free(ht->table);
//    // �������� ������ ����� HashTable ������������ ��������� �������. ����� �� ������� ������ ��� ������ ���-������� ����������
//}
//
//void ht_set(HashTable* ht, const char* key, const Pointer data)
//{
//    if (!table_was_initialised(ht))
//    {
//        printf("������� �� ����������������\n");
//        return;
//    }
//
//    if (!not_null_ptr(key, "������������ ����!\n"))
//
//    if (!not_null_ptr(data, "������������ ������!\n")) return;
//
//    int list_index = get_list_index(ht, key);  // ������ ������� ������
//    if (list_index == NOT_INITIALISED_TABLE) return;  // ������� �� ����������������
//    if (!not_null_ptr(ht->table[list_index], NULL))  // ������ ��� �� ������, ������ �����
//    {
//        ht->table[list_index] = mlist_create(key, data);
//        return;
//    }
//
//    MList* existing_node = mlist_find(ht->table[list_index], key);
//    if (existing_node == NULL)  // � ������ ��� ��� ������ �����, �������� ������ ������ �� �������
//    {
//        ht->table[list_index] = mlist_add(ht->table[list_index], key, data);
//        return;
//    }
//
//    // � ������ ��� ���� ����� ����, ���� ������, ��������� �� ������
//    printf("������ �� ����� \"%s\" ��� ����������. ��������? ", key);
//    bool replace_old_data = yes_no_question();
//    if (replace_old_data)
//    {
//        ht->dtor(existing_node->data);
//        existing_node->data = data;
//        printf("��������� ���������\n");
//        return;
//    }
//    printf("��������� ��������\n");
//}
//
//Pointer ht_get(const HashTable* ht, const char* key)
//{
//    int list_index = get_list_index(ht, key);
//    MList* ml_with_key = (list_index >= 0) ? ht->table[list_index] : NULL;
//    return mlist_get(ml_with_key, key);
//}
//
//bool ht_has(const HashTable* ht, const char* key)
//{
//    int list_index = get_list_index(ht, key);
//    MList* ml_for_key = (list_index >= 0) ? ht->table[list_index] : NULL;
//    return mlist_has(ml_for_key, key);
//}
//
//void ht_delete(HashTable* ht, const char* key)
//{
//    int list_index = get_list_index(ht, key);
//    if (list_index < 0) return;  // ������� �� ���� ����������������
//    MList* removed_elem = mlist_find(ht->table[list_index], key);
//    if (removed_elem == NULL) return;  // ������ ����� ��� � ������
//    ht->dtor(removed_elem->data);
//    ht->table[list_index] = mlist_remove(ht->table[list_index], key);
//}
//
//void ht_traverse(HashTable* ht, void (*f)(char* key, Pointer data))
//{
//    if (!table_was_initialised(ht))
//    {
//        printf("��������������� �������\n");
//        return;
//    }
//
//    MList* cur_mlist = NULL;
//    for (int i = 0; i < ht->size; ++i)
//    {
//        cur_mlist = ht->table[i];
//        mlist_foreach_item(cur_mlist, f);
//    }
//}
//
//void ht_resize(HashTable* ht, size_t new_size)
//{
//    if (!table_was_initialised(ht))
//    {
//        printf("��������������� �������\n");
//        return;
//    }
//    if (new_size < 1)
//    {
//        printf("���������� ������� ������ �������\n");
//        return;
//    }
//
//    MList** new_table = (MList**)malloc(sizeof(MList*) * new_size);
//    if (!not_null_ptr(ht->table, "������ ��������� ������ ��� �������. ��������� ������� ������� �� ����������\n")) return;
//    displace_table(ht->hashfunc, new_table, new_size, ht->table, ht->size);
//    free(ht->table);
//    ht->table = new_table;
//    ht->size = new_size;
//}
//
//static bool not_null_ptr(const Pointer ptr, const char* error_msg)
//{
//    setlocale(LC_ALL, "ru");
//    if (ptr != NULL) return true;
//    if (error_msg != NULL) printf("%s\n", error_msg);
//    return false;
//}
//
//static unsigned jenkins_one_at_a_time_hash(const char* key) 
//{
//    unsigned hash = 0;
//
//    for (; *key; ++key) {
//        hash += *key;
//        hash += (hash << 10);
//        hash ^= (hash >> 6);
//    }
//    hash += (hash << 3);
//    hash ^= (hash >> 11);
//    hash += (hash << 15);
//    return hash;
//}
//
//static bool table_was_initialised(HashTable* ht)
//{
//    if ((ht == NULL) || (ht->hashfunc) == NULL || (ht->table) == NULL || (ht->size < 1)) return false;
//    return true;
//}
//
//static bool yes_no_question()
//{
//    printf("[Y]/[N]: ");
//    char ans = getchar();
//    while (ans != 'Y' && ans != 'N')
//    {
//        while (getchar() != '\n');  // ������ ������
//        printf("������� Y ��� N: ");
//        ans = getchar();
//    }
//    while (getchar() != '\n');  // ������� ���������� � ������ �������
//    if (ans == 'Y') return true;
//    return false;
//}
//
//static int get_list_index(HashTable* ht, const char* key)
//{
//    if (!table_was_initialised(ht))
//    {
//        printf("������� �� ����������������\n");
//        return NOT_INITIALISED_TABLE;
//    }
//
//    if (key == NULL) return NULL_KEY_ERROR;
//
//    return ht->hashfunc(key) % ht->size;;
//}
//
//static void displace_table(HashFunction hf, MList** new_table, size_t new_size, MList** old_table, size_t old_size)
//{
//    for (int i = 0; i < new_size; ++i)
//    {
//        new_table[i] = NULL;
//    }
//    MList* cur_mlist = NULL, *without_head = NULL;
//    for (int old_index = 0; old_index < old_size; ++old_index)
//    {
//        cur_mlist = old_table[old_index];  // ��������� ������������ ������
//        old_table[old_index] = NULL;  // ������ ������� ������ ��� �� ������
//        // ������ ����������� �������� ������ � ����� �������
//        while (cur_mlist != NULL)
//        {
//            without_head = cur_mlist->next;
//            cur_mlist->next = NULL;
//            // �������� �������� ������ � ��������� � � ����� ������� � ������ ��������������� ������ (������)
//            int new_index = hf(cur_mlist->key) % new_size;
//            cur_mlist->next = new_table[new_index];
//            new_table[new_index] = cur_mlist;
//            cur_mlist = without_head;
//        }
//    }
//}