#include "constants.h"

// ������ ������ ��� ������ ������� �������
const int TEST_LLIST_SIZE = 10;

// ��������� ������� ����������
const int EXIT_SUCCESSFULLY = 0;
// �������� ���������� ������ ��� ��������� ������������. ������� �������� ������ � ��������� �����, ����������� ������������ ������� �� ����������� ����������
const int EXIT_SOFT_FAILURE = 1;
// ������ �� ����� �������� ��� ��������������
const int EXIT_FATAL_FAILURE = 2;

// ����������� �������� ��������
const int WRONG_PARAM = -1;
// ����� ������������ ����� ���������
const int CFG_FROM_PARAMS = 0;
// ����� ������������ ��������������� ����� �������
const int CFG_STEP_BY_STEP = 1;
// ����� ������������ � �����
const int CFG_FROM_FILE = 2;
// ������� �������
const int PRINT_HELP = 3;

// ����� �������� ����� ������
const int TRAIN_NEW_MODE = 0;
// ����� ���������� ������������ ������
const int TRAIN_EXISTED_MODE = 1;
// ����� �������������� ������
const int EDIT_MODE = 2;

// ����� ���������� � ������� � ������ TRAIN_NEW
const int TRAIN_NEW_CFG_SIZE = 4;
// ����� ���������� � ������� � ������ TRAIN_EXISTED
const int TRAIN_EXISTED_CFG_SIZE = 3;
// ����� ���������� � ������� � ������ EDIT
const int EDIT_CFG_SIZE = 6;