#ifndef _CONSTANTS_
#define _CONSTANTS_

#define _CRT_SECURE_NO_WARNINGS


// ��������� �� ������ ������ � �������
#define APOLOGY_MEMORY_MSG "������ ������ � �������(((\n���������� ��� �� ������������� ����������!\n���������� ��� ���\n"
// ���������� ��������� ������� ��������� ������������ ������
#define MAX_MALLOC_ATTEMPTS 3
// ���������� ������� ������������ ������ ���-�� � �������
#define MAX_USER_ATTEMPTS 5
// ��������� ������� ����������
#define EXIT_SUCCESSFULLY 0
// ����������������� ������ �� ���� ������������
#define EXIT_USER_FAILURE 1
// ������ ��������� ������
#define EXIT_MEMORY_FAILURE 2
// ������ ������ � �������
#define EXIT_FILE_FAILURE 3

// ����������� ���� ��� �������� ����� .txt (� ������� �������� ���� �����������, ��� ���� ������������ �� .txt"
#define WRONG_PATH "wrong path"
// ����������� �������� �������� (��� ��������� ������ ������ ���� ����������������)
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
// ������� ����� ������� ������
#define MODES_AMOUNT 3

// ����� ���������� � ������� � ������ TRAIN_NEW
#define TRAIN_NEW_CFG_SIZE 4
// ����� ���������� � ������� � ������ TRAIN_EXISTED
#define TRAIN_EXISTED_CFG_SIZE 3
// ����� ���������� � ������� � ������ EDIT
#define EDIT_CFG_SIZE 6

// ������� ������ �������� ��� ����� ��� ������ (5600 - ������� �������� ��������� �� ��������� WORD)
#define BUFFER_SIZE 5601

// ������� ��������� ������, ������� ����� ������� ����� �������������
#define TMP_SUFFIX "_temporary_file__"
// ��� ���������� ����� ��� �������� � ������ ���������� ������������
#define TMP_PARAMS_FILE "__params_temporary_file__.txt"

/*
� ����� �������� ������� ����� ������� ������ �������� ����� ��������� ����������������������������������, � ��� � 33 �����. ��� ��� ������ 1933-�� ����. 
� � 2003 ���� ��� �������� ����� �� 35 �������� � ������������������������������������.
���� � ����� ������� �����, �������� ����� �� � ����� (�.�. ��� �������� ��������������). �� ����� ���� ����� ���� � ���������� �� ��� ������.
���� ���� ������� ����� ���������� � ������, �� ������ ����� �� ������� ��������� ��� ����� �������� ������, ���� ������ �� ���������� ����� ������� threshold
*/
#define MAX_AVAILABLE_WORD_LENGTH 35
// ������� �������� ����� ���� ��������� ���� (size_tol)
#define MAX_AVAILABLE_MISSES 3
// ������� �������� ���� ����� ���� � ��������� (threshold)
#define MAX_AVAILABLE_ERRORS 5

// ������ �����, ���������� ���������� ��� ������ ������� � ����������
#define INITIAL_TABLE_SIZE 100


#endif