#include "helpful_functions.h"
#include "help_func_tests.h"
#include <stdio.h>
#include <locale.h>
#include <stdbool.h>
#include <assert.h>


int help_func_launch_all_tests()
{
	setlocale(LC_ALL, "ru");
	
	assert(help_func_not_null_ptr_test() == 0);
	assert(help_func_datas_are_equal_test() == 0);
	assert(help_func_ints_are_equal_test() == 0);
	assert(help_func_multiply_int_test() == 0);

	printf("\n--- ¬се тесты со вспомогательными функци€ми успешно пройдены! ---\n\n");
	return 0;
}

int help_func_not_null_ptr_test()
{
	setlocale(LC_ALL, "ru");

	int just_number = 10;
	int* just_ptr = &just_number;
	assert(not_null_ptr(just_ptr, "It must not be printed\n") == true);
	assert(not_null_ptr(NULL, "Expected message from help_func_not_null_ptr_test") == false);

	printf("help_func_not_null_ptr_test успешно пройден\n");
	return 0;
}

int help_func_datas_are_equal_test()
{
	setlocale(LC_ALL, "ru");

	int equal_number_1 = 10;
	int equal_number_2 = 10;
	int inequal_number = 20;
	assert(datas_are_equal(&equal_number_1, sizeof(int), &equal_number_2, sizeof(int)) == true);
	assert(datas_are_equal(&equal_number_1, sizeof(int), &equal_number_2, sizeof(int) - 1) == false);
	assert(datas_are_equal(&equal_number_1, sizeof(int), &inequal_number, sizeof(int)) == false);

	printf("help_func_datas_are_equal_test успешно пройден\n");
	return 0;
}

int help_func_ints_are_equal_test()
{
	setlocale(LC_ALL, "ru");

	int equal_number_1 = 10;
	int equal_number_2 = 10;
	int inequal_number = 20;
	assert(ints_are_equal(&equal_number_1, &equal_number_2) == true);
	assert(ints_are_equal(&equal_number_1, &inequal_number) == false);

	printf("help_func_ints_are_equal_test успешно пройден\n");
	return 0;
}

int help_func_multiply_int_test()
{
	setlocale(LC_ALL, "ru");

	int number_for_multiply = 10;
	int multiplier = 2;
	multiply_int(&number_for_multiply, &multiplier);
	assert(number_for_multiply == 20);
	assert(multiplier == 2);

	printf("help_func_multiply_int_test успешно пройден\n");
	return 0;
}