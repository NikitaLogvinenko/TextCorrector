#ifndef _EDIT_MODULE_
#define _EDIT_MODULE_


#include "CorrectorModel.h"


// ����������� ����� � ������������ � �������� � ��������� ����������������� ����� � ����� �����
// ���������� EXIT_SUCCESSFULLY � ������ ��������� ����������, ����� ��� ������:
// * EXIT_MEMORY_FAILURE - �� ���������� ������
// * EXIT_FILE_FAILURE - �� �������� ����
// * EXIT_USER_FILE - ������ �� ���� ������������
int edit_text(const Pointer* cfg);


#endif