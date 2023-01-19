#ifndef _CORRECTOR_LIB_
#define _CORRECTOR_LIB_


#include "CorrectorModel.h"
#include <stdio.h>


// ������������� ����������. �������� ������. ��������� ������� ������ �������� ���� (���������� - ���������� ��������)
// � ������ ��������� ��������� ������ ��� ������� ���������� EXIT_SUCCESSFULLY, ����� EXIT_MEMORY_FAILURE
int corrector_init(Corrector* corrector, unsigned max_word_length, WordsMetric metric_func);

// ������� ������ � ����������. �������� ��������, �������� ������ �������. ���� ������ ��� ��� ��������� ���������� ����������� - ���������� ������������� ��������� free(corrector)
void corrector_destroy(Corrector* corrector);

// ���������� ��������� ������ � �������� ���� � �������
// max_word_length=mwl
// <table_1> ...
// <table_2> ...
// ...
// <table_mwl> ...
// �� ��������� ������ ��������� ����
// � ������ ��������� ���������� ���������� EXIT_SUCCESSFULLY, ����� EXIT_FILE_FAILURE (���� �� ��������)
int corrector_save(Corrector* corrector, FILE* model_file);

// �������� ����� � ��������������� ������� � ����������. ���������� ������� � 1, ���� ��� ����� �����, ��� ��������� ������� ������������� ����� �� 1
void corrector_learn(Corrector* corrector, const char* word);

// ��������� ��������� ������ �� ��������� ����� � ������ ������
// �� ��������� �������� ������� ���� � �������
// �� ����� ����������� max_word_length � ������ ����������������. ����� ����������� ������� ����������� ������� �� �����
// ���� ������ ������� ��������� - ������� EXIT_SUCCESSFULLY, ���� ���� �� �������� - ������� EXIT_FILE_FAILURE, ���� ���� ����� ������������ ������ - ������� EXIT_USER_FAILURE
int corrector_load(Corrector* corrector, FILE* model_file);


#endif 

