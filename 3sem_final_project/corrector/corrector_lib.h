#ifndef _CORRECTOR_LIB_
#define _CORRECTOR_LIB_


#include "CorrectorModel.h"
#include <stdio.h>


// ������������� ����������. �������� ������. ��������� ������� ������ �������� ���� (���������� - ���������� ��������)
// � ������ ��������� ��������� ������ ��� ������� ���������� EXIT_SUCCESSFULLY, ����� EXIT_MEMORY_FAILURE
int corrector_init(Corrector* corrector, unsigned max_word_length, WordsMetric metric_func);

// ������� ������ � ����������
// �������� ��������, �������� ������ �������. ���� ������ ��� ��� ��������� ���������� ����������� - ���������� ������������� ��������� free(corrector)
void corrector_destroy(Corrector* corrector);

// ���������� ��������� ������ � ���� model_file_name. ������ �������������� � �������:
// max_word_length=mwl
// <table_1> ...
// <table_2> ...
// ...
// <table_mwl> ...
// �� ��������� ������ ��������� ����
// � ������ ��������� ���������� ���������� EXIT_SUCCESSFULLY
// ���� model_file_name �� �������� ���� � ����� .txt - ������ EXIT_USER_FAILURE
// ���� �� �������� ���� - ������ EXIT_FILE_FAILURE
int corrector_save(Corrector* corrector, const char* model_file_name);

// �������� ����� � ��������������� ������� � ����������. ���������� ������� ����� � 1, ���� ��� ����� �����, ��� ��������� ������� ������������� ����� �� 1
// ���� ����� ������� �������� - ������ EXIT_SUCCESSFULLY
// ���� ��������� �� ������ ��� �� ����� ����� NULL - ������ EXIT_USER_FAILURE
// ���� ����� ������� ������� ��� ������� - ������ EXIT_TOO_LONG
// ���� �� ���������� ������ ��� ����� �����, �� ������ ������� �������, � ������� ������ EXIT_MEMORY_FAILURE
int corrector_learn(Corrector* corrector, const char* word);

// ��������� ��������� ������ �� ����� model_file_name � �������������������� ������ (���� ������ ��� ���������������� - ��� ����� ������������, ������ �������� ������ ������)
// �� ����� ����������� max_word_length � ������ ����������������, �������� ������ ���������� ������. ����� ������� ����������� ������� �� �����
// ���� ������ ������� ��������� - ������� EXIT_SUCCESSFULLY
// ���� �� ���������� ������ ��� ����������� ������� ��� ��� ������� ������� ������� - ������� EXIT_MEMORY_FAILURE. � ����� ������ ��� ����������� ������ ����� ��������
// ���� ���� �� �������� - ������� EXIT_FILE_FAILURE
// ���� ���� ����� ������������ ������ - ������� EXIT_USER_FAILURE
// ���� ������ ��� ��� ����� ��������� �� NULL, ��� ��� ����� �� �������� ���� � ����� .txt - ������� EXIT_USER_FAILURE
int corrector_load(Corrector* corrector, const char* model_file_name);


#endif 

