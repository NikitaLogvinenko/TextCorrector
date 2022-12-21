#ifndef _TRAIN_MODULE_
#define _TRAIN_MODULE_


#include "CorrectorModel.h"


// ������� ����� ������ � ������������ � �������� � ��������� � � ����.
// ���������� 0 � ������ ��������� ����������, ����� ��� ������
int train_new_model(const Pointer* cfg);

// ��������� ������������ ������ � ������������ � �������� � ��������� ���������.
// ���������� 0 � ������ ��������� ����������, ����� ��� ������
int train_existed_model(const Pointer* cfg);

#endif