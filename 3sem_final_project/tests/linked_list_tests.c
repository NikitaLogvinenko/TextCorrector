#include "linked_list.h"
#include "ll_lib.h"
#include "linked_list_tests.h"
#include "helpful_functions.h"
#include <locale.h>
#include <assert.h>
#include <stdlib.h>

// Создаёт связный список [0, 1, 2, ..., range - 1] для тестов
static Linked_list* create_llist_for_test(unsigned int range);

// В случае успешного выполнения тестов возвращаем 0

int launch_all_tests()
{
	setlocale(LC_ALL, "ru");
	create_destroy_test();
	append_test();
	prepend_test();
	concat_test();
	copy_test();
	find_test();
	find_custom_test();
	foreach_test();
	get_test();
	get_last_test();
	length_test();
	remove_test();
	remove_all_equal_test();
	remove_first_equal_test();

	printf("Все тесты успешно пройдены\n");
	return 0;
}

int create_destroy_test()
{
	Linked_list* llist = create_llist_for_test(10);
	assert(not_null_ptr(llist, "Ошибка выделения памяти в тесте create_destroy_test\n"));
	for (int i = 0; i < 10; ++i)
	{
		assert(is_equal(&i, sizeof(int), llist->data, llist->data_size));
		llist = llist->next;
	}
	llist_destroy(llist);
	return 0;
}

int append_test()
{
	return 0;
}

int prepend_test()
{
	return 0;
}

int concat_test()
{
	return 0;
}

int copy_test()
{
	return 0;
}

int find_test()
{
	return 0;
}

int find_custom_test()
{
	return 0;
}

int foreach_test()
{
	return 0;
}

int get_test()
{
	return 0;
}

int get_last_test()
{
	return 0;
}

int length_test()
{
	return 0;
}

int remove_test()
{
	return 0;
}

int remove_all_equal_test()
{
	return 0;
}

int remove_first_equal_test()
{
	return 0;
}

static Linked_list* create_llist_for_test(unsigned int range)
{
	int elem = 0;
	Linked_list* llist_for_test = llist_create(&elem, sizeof(int));
	if (!not_null_ptr(llist_for_test, "Ошибка выделения памяти при создании тестового списка\n")) return NULL;

	for (int i = 1; i < range; ++i)
	{
		Linked_list* next_number = llist_create(&i, sizeof(int));
		if (!not_null_ptr(next_number, "Ошибка выделения памяти при создании тестового списка\n")) return NULL;
	}

	return llist_for_test;
}