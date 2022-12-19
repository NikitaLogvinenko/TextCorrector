#define TEST_LLIST_SIZE 10


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
	assert(create_destroy_test() == 0);
	assert(append_test() == 0);
	assert(prepend_test() == 0);
	assert(concat_test() == 0);
	assert(copy_test() == 0);
	assert(find_test() == 0);
	assert(find_custom_test() == 0);
	assert(foreach_test() == 0);
	assert(get_test() == 0);
	assert(get_last_test() == 0);
	assert(length_test() == 0);
	assert(remove_test() == 0);
	assert(remove_all_equal_test() == 0);
	assert(remove_first_equal_test() == 0);
	assert(trip_to_index_test() == 0);

	printf("\nВсе тесты успешно пройдены\n");
	return 0;
}

int create_destroy_test()
{
	setlocale(LC_ALL, "ru");

	Linked_list* llist = create_llist_for_test(TEST_LLIST_SIZE);
	assert(not_null_ptr(llist, "Ошибка выделения памяти в тесте create_destroy_test\n"));

	Linked_list* next_number = llist;
	for (int i = 0; i < TEST_LLIST_SIZE; ++i)
	{
		assert(datas_are_equal(&i, sizeof(int), next_number->data, next_number->data_size));
		next_number = next_number->next;
	}

	llist_destroy(llist);
	printf("create_destroy_test успешно пройден\n");
	return 0;
}

int append_test()
{
	setlocale(LC_ALL, "ru");

	int number = 0;
	Linked_list* llist = llist_append(NULL, &number, sizeof(int));
	assert(not_null_ptr(llist, "Ошибка выделения памяти в тесте append_test\n"));

	for (int i = 1; i < TEST_LLIST_SIZE; ++i)
	{
		llist = llist_append(llist, &i, sizeof(int));
		assert(not_null_ptr(llist, "Ошибка выделения памяти в тесте append_test\n"));
	}

	Linked_list* next_number = llist;
	for (int i = 0; i < TEST_LLIST_SIZE; ++i)
	{
		assert(datas_are_equal(&i, sizeof(int), next_number->data, next_number->data_size));
		next_number = next_number->next;
	}

	llist_destroy(llist);
	printf("append_test успешно пройден\n");
	return 0;
}

int prepend_test()
{
	setlocale(LC_ALL, "ru");

	int number = TEST_LLIST_SIZE - 1;
	Linked_list* llist = llist_prepend(NULL, &number, sizeof(int));
	assert(not_null_ptr(llist, "Ошибка выделения памяти в тесте prepend_test\n"));

	for (int i = TEST_LLIST_SIZE - 2; i > -1; --i)
	{
		llist = llist_prepend(llist, &i, sizeof(int));
		assert(not_null_ptr(llist, "Ошибка выделения памяти в тесте prepend_test\n"));
	}

	Linked_list* next_number = llist;
	for (int i = 0; i < TEST_LLIST_SIZE; ++i)
	{
		assert(datas_are_equal(&i, sizeof(int), next_number->data, next_number->data_size));
		next_number = next_number->next;
	}

	llist_destroy(llist);
	printf("prepend_test успешно пройден\n");
	return 0;
}

int concat_test()
{
	setlocale(LC_ALL, "ru");

	Linked_list* llist_1 = create_llist_for_test(TEST_LLIST_SIZE);
	assert(not_null_ptr(llist_1, "Ошибка выделения памяти в тесте concat_test\n"));
	Linked_list* llist_2 = create_llist_for_test(TEST_LLIST_SIZE);
	assert(not_null_ptr(llist_2, "Ошибка выделения памяти в тесте concat_test\n"));
	llist_1 = llist_concat(llist_1, llist_2);

	Linked_list* next_number = llist_1;
	for (int i = 0; i < TEST_LLIST_SIZE; ++i)
	{
		assert(datas_are_equal(&i, sizeof(int), next_number->data, next_number->data_size));
		next_number = next_number->next;
	}
	for (int i = 0; i < TEST_LLIST_SIZE; ++i)
	{
		assert(datas_are_equal(&i, sizeof(int), next_number->data, next_number->data_size));
		next_number = next_number->next;
	}

	llist_destroy(llist_1);
	// llist_2 очистится вместе с llist_1, т.к. мы их сконкатенировали
	printf("concat_test успешно пройден\n");
	return 0;
}

int copy_test()
{
	setlocale(LC_ALL, "ru");

	Linked_list* llist_1 = create_llist_for_test(TEST_LLIST_SIZE);
	assert(not_null_ptr(llist_1, "Ошибка выделения памяти в тесте copy_test\n"));
	Linked_list* llist_2 = llist_copy(llist_1);
	assert(not_null_ptr(llist_2, "Ошибка выделения памяти в тесте copy_test\n"));

	Linked_list* next_number_1 = llist_1;
	Linked_list* next_number_2 = llist_2;
	for (int i = 0; i < TEST_LLIST_SIZE; ++i)
	{
		assert(datas_are_equal(&i, sizeof(int), next_number_1->data, next_number_1->data_size));
		assert(datas_are_equal(&i, sizeof(int), next_number_2->data, next_number_2->data_size));
		assert(datas_are_equal(next_number_1->data, next_number_1->data_size, next_number_2->data, next_number_2->data_size));
		next_number_1 = next_number_1->next;
		next_number_2 = next_number_2->next;
	}

	llist_destroy(llist_1);
	llist_destroy(llist_2);
	printf("copy_test успешно пройден\n");
	return 0;
}

int find_test()
{
	setlocale(LC_ALL, "ru");

	Linked_list* llist = create_llist_for_test(10);
	assert(not_null_ptr(llist, "Ошибка выделения памяти в тесте find_test\n"));

	for (int i = 0; i < TEST_LLIST_SIZE * 2; ++i)
	{
		bool in_the_llist = (i < TEST_LLIST_SIZE) && (llist_find(llist, &i, sizeof(int)) == i);
		bool out_of_the_llist = (i >= TEST_LLIST_SIZE) && (llist_find(llist, &i, sizeof(int)) == -1);
		assert(in_the_llist || out_of_the_llist);
	}

	llist_destroy(llist);
	printf("find_test успешно пройден\n");
	return 0;
}

int find_custom_test()
{
	setlocale(LC_ALL, "ru");

	Linked_list* llist = create_llist_for_test(TEST_LLIST_SIZE);
	int custom_data;
	assert(not_null_ptr(llist, "Ошибка выделения памяти в тесте find_custom_test\n"));

	for (int i = 0; i < TEST_LLIST_SIZE * 2; ++i)
	{
		int finded_index = llist_find_custom(llist, ints_are_equal, &i, &custom_data);
		bool in_the_llist = (i < TEST_LLIST_SIZE) && (finded_index == i) && (custom_data == i);
		bool out_of_the_llist = (i >= TEST_LLIST_SIZE) && (finded_index == -1);
		assert(in_the_llist || out_of_the_llist);
	}

	llist_destroy(llist);
	printf("find_custom_test успешно пройден\n");
	return 0;
}

int foreach_test()
{
	setlocale(LC_ALL, "ru");

	Linked_list* llist = create_llist_for_test(TEST_LLIST_SIZE);
	assert(not_null_ptr(llist, "Ошибка выделения памяти в тесте foreach_test\n"));
	int multiplier = TEST_LLIST_SIZE;
	llist_foreach(llist, multiply_int, &multiplier);
	Linked_list* next_number = llist;
	for (int i = 0; i < TEST_LLIST_SIZE; ++i)
	{
		int expected_value = i * TEST_LLIST_SIZE;
		assert(datas_are_equal(&expected_value, sizeof(int), next_number->data, next_number->data_size));
		next_number = next_number->next;
	}

	llist_destroy(llist);
	printf("foreach_test успешно пройден\n");
	return 0;
}

int get_test()
{
	setlocale(LC_ALL, "ru");

	Linked_list* llist = create_llist_for_test(TEST_LLIST_SIZE);
	assert(not_null_ptr(llist, "Ошибка выделения памяти в тесте get_test\n"));

	for (int i = 0; i < TEST_LLIST_SIZE * 2; ++i)
	{
		int* copy_of_the_number = llist_get(llist, i);
		bool in_the_llist = (i < TEST_LLIST_SIZE) && (*copy_of_the_number == i);
		bool out_of_the_llist = (i >= TEST_LLIST_SIZE) && (copy_of_the_number == NULL);
		assert(in_the_llist || out_of_the_llist);
	}

	llist_destroy(llist);
	printf("get_test успешно пройден\n");
	return 0;
}

int get_last_test()
{
	setlocale(LC_ALL, "ru");

	Linked_list* llist = create_llist_for_test(TEST_LLIST_SIZE);
	assert(not_null_ptr(llist, "Ошибка выделения памяти в тесте get_last_test\n"));

	int* copy_of_the_last_number = llist_get_last(llist);
	assert(*copy_of_the_last_number == TEST_LLIST_SIZE - 1);
	copy_of_the_last_number = llist_get_last(NULL);
	assert(copy_of_the_last_number == NULL);

	llist_destroy(llist);
	printf("get_last_test успешно пройден\n");
	return 0;
	return 0;
}

int length_test()
{
	setlocale(LC_ALL, "ru");

	Linked_list* llist = create_llist_for_test(10);
	assert(not_null_ptr(llist, "Ошибка выделения памяти в тесте length_test\n"));
	assert(llist_length(NULL) == 0);
	assert(llist_length(llist) == 10);
	llist_destroy(llist);
	printf("length_test успешно пройден\n");
	return 0;
}

int remove_test()
{
	setlocale(LC_ALL, "ru");

	Linked_list* llist = create_llist_for_test(TEST_LLIST_SIZE);
	assert(not_null_ptr(llist, "Ошибка выделения памяти в тесте remove_test\n"));
	
	// Удалим все узлы, кроме головы
	for (int i = 0; i < TEST_LLIST_SIZE - 1; ++i)
	{
		// Удаляем узел по индексу 1 из списка
		llist = llist_remove(llist, 1);
		// Проверяем размер нового списка
		assert(llist_length(llist) == TEST_LLIST_SIZE - i - 1);
	}
	// Удаляем нулевой узел
	llist = llist_remove(llist, 0);
	assert(llist_length(llist) == 0);
	assert(llist == NULL);
	printf("remove_test успешно пройден\n");
	return 0;
}

int remove_all_equal_test()
{
	setlocale(LC_ALL, "ru");

	Linked_list* llist_1 = create_llist_for_test(TEST_LLIST_SIZE);
	assert(not_null_ptr(llist_1, "Ошибка выделения памяти в тесте remove_all_equal_test\n"));
	Linked_list* llist_2 = create_llist_for_test(TEST_LLIST_SIZE);
	assert(not_null_ptr(llist_2, "Ошибка выделения памяти в тесте remove_all_equal_test\n"));
	Linked_list* llist_3 = create_llist_for_test(TEST_LLIST_SIZE);
	assert(not_null_ptr(llist_3, "Ошибка выделения памяти в тесте remove_all_equal_test\n"));
	Linked_list* united_llist = llist_concat(llist_1, llist_concat(llist_2, llist_3));

	for (int i = 0; i < TEST_LLIST_SIZE; ++i)
	{
		united_llist = llist_remove_all_equal(united_llist, &i, sizeof(int));
		assert(llist_length(united_llist) == (TEST_LLIST_SIZE - i - 1) * 3);
	}

	assert(united_llist == NULL);
	printf("remove_all_equal_test успешно пройден\n");
	return 0;
}

int remove_first_equal_test()
{
	setlocale(LC_ALL, "ru");

	Linked_list* llist_1 = create_llist_for_test(TEST_LLIST_SIZE);
	assert(not_null_ptr(llist_1, "Ошибка выделения памяти в тесте remove_first_equal_test\n"));
	Linked_list* llist_2 = create_llist_for_test(TEST_LLIST_SIZE);
	assert(not_null_ptr(llist_2, "Ошибка выделения памяти в тесте remove_first_equal_test\n"));
	Linked_list* llist_3 = create_llist_for_test(TEST_LLIST_SIZE);
	assert(not_null_ptr(llist_3, "Ошибка выделения памяти в тесте remove_first_equal_test\n"));
	Linked_list* united_llist = llist_concat(llist_1, llist_concat(llist_2, llist_3));

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < TEST_LLIST_SIZE; ++j)
		{
			united_llist = llist_remove_first_equal(united_llist, &j, sizeof(int));
			assert(llist_length(united_llist) == TEST_LLIST_SIZE * 3 - (j + 1) - TEST_LLIST_SIZE * i);
		}
	}

	assert(united_llist == NULL);
	printf("remove_first_equal_test успешно пройден\n");
	return 0;
}

int trip_to_index_test()
{
	setlocale(LC_ALL, "ru");

	Linked_list* llist = create_llist_for_test(TEST_LLIST_SIZE);
	assert(not_null_ptr(llist, "Ошибка выделения памяти в тесте trip_to_index_test\n"));

	Linked_list* llist_node = llist_trip_to_index(NULL, 0);
	assert(llist_node == NULL);
	for (int i = 0; i < TEST_LLIST_SIZE * 2; ++i)
	{
		llist_node = llist_trip_to_index(llist, i);
		bool in_the_llist = (i < TEST_LLIST_SIZE) && (datas_are_equal(&i, sizeof(int), llist_node->data, llist_node->data_size));
		bool out_of_the_llist = (i >= TEST_LLIST_SIZE) && (llist_node == NULL);
		assert(in_the_llist || out_of_the_llist);
	}

	llist_destroy(llist);
	printf("trip_to_index_test успешно пройден\n");
	return 0;
}

static Linked_list* create_llist_for_test(unsigned int range)
{
	int elem = 0;
	Linked_list* llist_for_test = llist_create(&elem, sizeof(int));
	if (!not_null_ptr(llist_for_test, "Ошибка выделения памяти при создании тестового списка\n")) return NULL;
	Linked_list* cur_number = llist_for_test;
	Linked_list* next_number = NULL;
	for (int i = 1; i < range; ++i)
	{
		next_number = llist_create(&i, sizeof(int));
		if (!not_null_ptr(next_number, "Ошибка выделения памяти при создании тестового списка\n")) return NULL;
		cur_number->next = next_number;
		cur_number = next_number;
	}

	return llist_for_test;
}

