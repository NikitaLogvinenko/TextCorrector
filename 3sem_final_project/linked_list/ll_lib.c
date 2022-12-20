#include "ll_lib.h"
#include "helpful_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Удалить голову, вернуть новую голову
static Linked_list* remove_head(Linked_list* old_head);

// Удалить следующий узел в списке
static void remove_next(Linked_list* prev_node);

Linked_list* llist_create(const Pointer data, size_t data_size)
{
    // Выделение памяти под список
    Linked_list* new_ll = (Linked_list*)malloc(sizeof(Linked_list));
    if (!not_null_ptr(new_ll, "Ошибка выделения памяти по связный список\n")) 
        return NULL;
    // Копирование данных
    if (data != NULL && data_size != 0)
    {
        new_ll->data = malloc(data_size);
        if (!not_null_ptr(new_ll->data, "Ошибка выделения памяти под данные\n"))
        {
            free(new_ll);
            return NULL;
        }
        memcpy(new_ll->data, data, data_size);
        new_ll->data_size = data_size;
    }
    else
    {
        new_ll->data = NULL;
        new_ll->data_size = 0;
    }
    // Следующего элемента пока что нет
    new_ll->next = NULL;

    return new_ll;
}

void llist_destroy(Linked_list* ll)
{
    if (ll == NULL) 
        return;
    // Удаляем хвост списка
    llist_destroy(ll->next);
    // Чистим память, выделенную под голову
    free(ll);
}

Linked_list* llist_copy(const Linked_list* ll)
{
    if (ll == NULL) 
        return NULL;
    // Копируем голову
    Linked_list* copy_ll = llist_create(ll->data, ll->data_size);
    if (!not_null_ptr(copy_ll, "Ошибка выделения памяти при копировании\n")) return NULL;
    // Копируем все следующие элементы
    Linked_list* cur_node = copy_ll;
    Linked_list* next_node = NULL;
    while (ll->next != NULL)
    {
        ll = ll->next;
        // Копируем следующий элемент
        next_node = llist_create(ll->data, ll->data_size);
        if (!not_null_ptr(copy_ll, "Ошибка выделения памяти при копировании\n"))
        {
            llist_destroy(copy_ll);
            return NULL;
        }
        cur_node->next = next_node;
        cur_node = cur_node->next;
    }

    return copy_ll;
}

int llist_length(const Linked_list* ll)
{
    int counter = 0;
    while (ll != NULL)
    {
        counter += 1;
        ll = ll->next;
    }
    return counter;
}

Linked_list* llist_prepend(Linked_list* ll, const Pointer data, size_t data_size)
{
    // Создаём новую голову
    Linked_list* new_head = llist_create(data, data_size);
    if (!not_null_ptr(new_head, "Ошибка выделения памяти при вставке. Возвращён старый список\n")) 
        return ll;
    // Указываем на старую голову (даже если это NULL, то ничего всё хорошо)
    new_head->next = ll;

    return new_head;
}

Linked_list* llist_append(Linked_list* ll, const Pointer data, size_t data_size)
{
    // Создаём новый элемент
    Linked_list* new_tail = llist_create(data, data_size);
    if (!not_null_ptr(new_tail, "Ошибка выделения памяти при вставке. Возвращён старый список\n"))
        return ll;
    // Если создали новый список, то хвост - это и есть голова
    if (ll == NULL)
        return new_tail;
    // Вставляем элемент в конец существующего списка
    Linked_list* cur_node = ll;
    while (cur_node->next != NULL) 
        cur_node = cur_node->next;
    cur_node->next = new_tail;
    return ll;
}

Pointer llist_get(const Linked_list* ll, const unsigned index)
{
    // Перемещаемся по списку либо необходимое число раз, либо до конца
    for (int cur_index = 0; (cur_index < index) && (ll != NULL); ++cur_index) ll = ll->next;
    // Ничего не нашли
    if (ll == NULL)
        return NULL;
    // Нашли - копируем данные
    Pointer returned_data = malloc(ll->data_size);
    if (!not_null_ptr(returned_data, "Ошибка выделения памяти под данные\n"))
        return NULL;
    memcpy(returned_data, ll->data, ll->data_size);
    return returned_data;
}

Pointer llist_get_last(const Linked_list* ll)
{
    // В пустом списке ничего нет
    if (ll == NULL) return NULL;
    // Перемещаемся по списку до конца
    while (ll->next != NULL)
        ll = ll->next;
    // Копируем данные
    Pointer returned_data = malloc(ll->data_size);
    if (!not_null_ptr(returned_data, "Ошибка выделения памяти под данные\n"))
        return NULL;
    memcpy(returned_data, ll->data, ll->data_size);
    return returned_data;
}

Linked_list* llist_trip_to_index(const Linked_list* ll, const unsigned index)
{
    // Пустой список
    if (ll == NULL) return NULL;
    // Перемещаемся по списку нужное число раз
    for (int i = 0; (i < index) && (ll != NULL); ++i)
        ll = ll->next;
    return ll;
}

int llist_find(const Linked_list* ll, const Pointer data, size_t data_size)
{
    // В пустом списке ничего нет
    if (ll == NULL) return -1;
    // Поэлементно сравниваем данные в списке с искомым значением
    for (int counter = 0; ll != NULL; ++counter)
    {
        if (datas_are_equal(ll->data, ll->data_size, data, data_size))
            return counter;
        ll = ll->next;
    }
    // Ничего не нашли
    return -1;
}

int llist_find_custom(const Linked_list* ll,
    bool (*predicate)(const Pointer data, const Pointer extra_data),
    const Pointer extra_data,
    Pointer const custom_data)
{
    // В пустом списке ничего нет
    if (ll == NULL) return -1;
    // Поэлементно выполняем проверку условия
    for (int counter = 0; ll != NULL; ++counter)
    {
        if (predicate(ll->data, extra_data))
        {
            if (custom_data != NULL)
                memcpy(custom_data, ll->data, ll->data_size);
            return counter;
        }
        ll = ll->next;
    }
    // Ничего не нашли
    return -1;
}

Linked_list* llist_remove(Linked_list* ll, unsigned index)
{
    // Из пустого списка нечего удалять
    if (!not_null_ptr(ll, "Пустой список, удалять нечего\n"))
        return NULL;
    // Если удаляем голову, то после удаления у списка новая голова
    if (index == 0)
    {
        Linked_list* new_head = remove_head(ll);
        return new_head;
    }
    // Перемещаемся по списку либо остановившись перед удаляемым элементом, либо до конца
    Linked_list* cur_node = ll;
    for (int i = 0; (i < index - 1) && cur_node->next != NULL; ++i)
        cur_node = cur_node->next;
    // Индекса не существует - ничего не удаляем
    if (cur_node->next == NULL)
        return ll;
    // Удаляем следующий элемент
    remove_next(cur_node);

    return ll;
}

Linked_list* llist_remove_first_equal(Linked_list* ll, const Pointer data, size_t data_size)
{
    // Из пустого списка нечего удалять
    if (ll == NULL)
        return NULL;
    // Проверяем данные в голове списка
    if (datas_are_equal(ll->data, ll->data_size, data, data_size))
    {
        Linked_list* new_head = remove_head(ll);
        return new_head;
    }
    // Последовательно проверяем данные в следующем узле
    Linked_list* cur_node = ll;
    while (cur_node->next != NULL)
    {
        // Удаляем данные один раз при совпадении
        if (datas_are_equal(cur_node->next->data, cur_node->next->data_size, data, data_size))
        {
            remove_next(cur_node);
            break;
        }
        // Переходим на следующий элемент, если его данные не совпали
        cur_node = cur_node->next;
    }

    return ll;
}

Linked_list* llist_remove_all_equal(Linked_list* ll, const Pointer data, size_t data_size)
{
    // Из пустого списка нечего удалять
    if (ll == NULL)
        return NULL;
    // Проверяем данные в голове списка
    Linked_list* new_head = ll;
    while(datas_are_equal(new_head->data, new_head->data_size, data, data_size))
    {
        new_head = remove_head(new_head);
        if (new_head == NULL)
            return NULL; // Если удалили всё
    }
    // Последовательно проверяем данные в следующем узле
    Linked_list* cur_node = new_head;
    while (cur_node->next != NULL)
    {
        // Удаляем данные при совпадении и остаёмся в этом же узле для дальнейшей проверки
        if (datas_are_equal(cur_node->next->data, cur_node->next->data_size, data, data_size))
        {
            remove_next(cur_node);
            continue;
        }
        // Переходим на следующий элемент, если его данные не совпали
        cur_node = cur_node->next;
    }

    return new_head;
}

Linked_list* llist_concat(Linked_list* ll1, Linked_list* ll2)
{
    // Если один из списков пустой, то результат конкатенации - второй список, даже если второй тоже NULL
    if (ll1 == NULL) return ll2;
    if (ll2 == NULL) return ll1;
    // Если списки не пустые, то идём в хвост первого и присоединяем голову второго
    Linked_list* cur_node = ll1;
    while (cur_node->next != NULL) cur_node = cur_node->next;
    cur_node->next = ll2;
    return ll1;
}

void llist_foreach(Linked_list* ll,
    void (*func)(Pointer data, const Pointer extra_data),
    const Pointer extra_data)
{
    // Функция не передана, ничего не меняем
    if (func == NULL)
        return;
    // Поэлементно применяем функцию
    while (ll != NULL)
    {
        func(ll->data, extra_data);
        ll = ll->next;
    }
}

static Linked_list* remove_head(Linked_list* old_head)
{
    // Из пустого списка нечего удалять
    if (old_head == NULL)
        return NULL;

    // Отвязываем удаляемый элемент от остального списка и удаляем
    Linked_list* new_head = old_head->next;
    old_head->next = NULL;
    llist_destroy(old_head);

    return new_head;
}

static void remove_next(Linked_list* prev_node)
{
    // Удалять нечего
    if ((prev_node == NULL) || (prev_node->next == NULL))
        return;
    // Отвязываем удаляемый элемент от остального списка и удаляем
    Linked_list* removed_node = prev_node->next;
    prev_node->next = removed_node->next;
    removed_node->next = NULL;
    llist_destroy(removed_node);
}