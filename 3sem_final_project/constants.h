#ifndef _CONSTANTS_
#define _CONSTANTS_

#define _CRT_SECURE_NO_WARNINGS

// ������ ������ ��� ������ ������� �������
#define TEST_LLIST_SIZE 10

// ��������� ������� ����������
#define EXIT_SUCCESSFULLY 0
// ����������������� ������ �� ���� ������������. ������� ������� � ������� �� ���������
#define EXIT_SOFT_FAILURE 1
// ����������� ������ (�� ���������� ������, �� �������� ���� � ��). ���������� ����� �� ���������
#define EXIT_FATAL_FAILURE 2

// ����������� �������� ��������
#define WRONG_PARAM -1
// ����� ������������ ����� ���������
#define CFG_FROM_PARAMS 0
// ����� ������������ ��������������� ����� �������
#define CFG_STEP_BY_STEP 1
// ����� ������������ � �����
#define CFG_FROM_FILE 2
// ������� �������
#define PRINT_HELP 3

// ����������� �����
#define WRONG_MODE -1
// ����� �������� ����� ������
#define TRAIN_NEW_MODE 0
// ����� ���������� ������������ ������
#define TRAIN_EXISTED_MODE 1
// ����� �������������� ������
#define EDIT_MODE 2

// ����� ���������� � ������� � ������ TRAIN_NEW
#define TRAIN_NEW_CFG_SIZE 4
// ����� ���������� � ������� � ������ TRAIN_EXISTED
#define TRAIN_EXISTED_CFG_SIZE 3
// ����� ���������� � ������� � ������ EDIT
#define EDIT_CFG_SIZE 6


#endif