#ifndef _TRAIN_MODULE_
#define _TRAIN_MODULE_


#include "CorrectorModel.h"


// ������� ����� ������ � ������������ � �������� � ��������� � � ����.
// ���������� EXIT_SUCCESSFULLY � ������ ��������� ����������, ����� ��� ������:
// * EXIT_MEMORY_FAILURE - �� ���������� ������
// * EXIT_FILE_FAILURE - �� �������� ����
// * EXIT_USER_FILE - ������ �� ���� ������������
int train_new_model(const Pointer* cfg);

// ��������� ������������ ������ � ������������ � �������� � ��������� ���������.
// ���������� EXIT_SUCCESSFULLY � ������ ��������� ����������, ����� ��� ������:
// * EXIT_MEMORY_FAILURE - �� ���������� ������
// * EXIT_FILE_FAILURE - �� �������� ����
// * EXIT_USER_FILE - ������ �� ���� ������������
int train_existed_model(const Pointer* cfg);


#endif