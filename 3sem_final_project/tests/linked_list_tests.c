#include "linked_list.h"
#include "ll_lib.h"
#include "linked_list_tests.h"
#include <locale.h>
#include <assert.h>

// � ������ ��������� ���������� ������ ���������� 0

int launch_all_tests()
{
	setlocale(LC_ALL, "ru");
	create_destroy_test();
	
	printf("��� ����� ������� ��������\n");
	return 0;
}