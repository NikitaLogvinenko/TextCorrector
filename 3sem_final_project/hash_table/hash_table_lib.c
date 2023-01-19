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


// Проверить, что таблица инициализирована
static bool table_was_initialised(HashTable* ht);

// Получить индекс списка, в котором должен быть ключ word; если таблица не инициализирована - вернуть -1, если word=NULL - вернуть -2
// Хеш-функция возвращает нам различные значения, но нам нужно поместить слово в конечную таблицу. Следовательно, от хеша надо взять остаток от деления на размер таблицы
static int get_list_index(HashTable* ht, const char* word);

// Скопировать записи из старой таблицы в новую (размеры различаются, следовательно, расположение записей изменится)
// Новая таблица должна быть пустой
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

    if (not_null_ptr(ht, "Невозможно инициализировать таблицу по указателю NULL\n"))
    {
        if (size < 1)
            printf("Невозможно создать пустую таблицу\n");
        else
        {
            ht->table = (MList**)retry_malloc(sizeof(MList*) * size, MAX_MALLOC_ATTEMPTS);  // создаём динамический массив из size указателей на списки
            if (not_null_ptr(ht->table, "Ошибка выделения памяти под таблицу. Инициализация не состоялась\n"))
            {
                for (unsigned i = 0; i < size; ++i)
                    ht->table[i] = NULL;  // ставим все указатели в NULL
                ht->size = size;  // запоминаем размер таблицы
                ht->total_words = 0;  // пока что не добавляли никаких слов в таблицу
                // Устанавливаем функцию хеширования
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
            removed_list = ht->table[i];  // список всех данных с одинаковой хеш-суммой; далее будем его удалять
            mlist_destroy(removed_list);  // удаляем списки (поле word очищается тоже внутри mlist_destroy; поле counter использует статическую память)
        }
        free(ht->table);
        // очищение памяти самой HashTable пользователь выполняет вручную. Вдруг он выделял память под объект хеш-таблицы статически
    }
}

void ht_set(HashTable* ht, const char* word, unsigned counter)
{
    if (table_was_initialised(ht) == false)
        printf("Таблица для вставки слова не инициализирована\n");

    else if (word == NULL)
        printf("Некорректный ключ для вставки в таблицу!\n");

    else
    {
        if (ht_has(ht, word) == false)  // если это новое слово, то надо увеличить счётчик слов в таблице
        {
            ht->total_words += 1;
            if (ht->total_words >= ht->size * WRDS_EXCEED)  // слишком много слов в таблице, возможны длинные коллизии, увеличим количество строк в таблице
                ht_resize(ht, ht->size * HT_INCRS);
        }
        unsigned list_index = get_list_index(ht, word);  // индекс нужного списка
        if (ht->table[list_index] == NULL)  // в этой позиции в таблице список ещё не создан, создаём новый (а значит и слово это раньше не встречалось)
            ht->table[list_index] = mlist_create(word, counter);

        else  // список в таблице уже существует, добавляем в него word (если слово уже было в списке до этого, это всё решается внутри mlist_add
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
        printf("Попытка обхода неинициализированной таблицы\n");
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
        printf("Попытка изменения размера неинициализированной таблицы\n");

    else if (new_size < 1)
        printf("Попытка создания пустой таблицы\n");

    MList** new_table = (MList**)retry_malloc(sizeof(MList*) * new_size, MAX_MALLOC_ATTEMPTS);
    if (not_null_ptr(ht->table, "Ошибка выделения памяти под таблицу. Изменение размера таблицы не состоялось\n"))
    {
        displace_table(ht->hashfunc, new_table, new_size, ht->table, ht->size);  // перемещаем записи из старой таблицы в новую
        free(ht->table);  // удаляем старые указатели (под них выделялась динамическая память)
        ht->table = new_table;  // указываем на новую таблицу
        ht->size = new_size;  // меняем размер таблицы
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
        cur_mlist = old_table[old_index];  // запомнили перемещаемый список
        old_table[old_index] = NULL;  // старая таблица больше его не хранит
        // Теперь распределим элементы списка в новой таблице
        while (cur_mlist != NULL)
        {
            // Отсоединим голову
            without_head = cur_mlist->next;
            cur_mlist->next = NULL;
            // Вставим её в новую таблицу в начало соответствующей строки (списка)
            unsigned new_index = hf(cur_mlist->word) % new_size;
            cur_mlist->next = new_table[new_index];
            new_table[new_index] = cur_mlist;
            cur_mlist = without_head;
        }
    }
}