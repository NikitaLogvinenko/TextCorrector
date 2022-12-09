#include "linked_list.h"
#include "ll_lib.h"
#include "linked_list_tests.h"
#include <locale.h>
#include <assert.h>
#include <stdlib.h>

// ¬ случае успешного выполнени€ тестов возвращаем 0

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

	printf("¬се тесты успешно пройдены\n");
	return 0;
}

int create_destroy_test()
{
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

static Linked_list* create_llist_for_test()
{
	Linked_list* list_for_test = (Linked_list*)malloc(sizeof(Linked_list));

	return list_for_test;
}