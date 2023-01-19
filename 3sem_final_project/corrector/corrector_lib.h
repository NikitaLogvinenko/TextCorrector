#ifndef _CORRECTOR_LIB_
#define _CORRECTOR_LIB_


#include "CorrectorModel.h"


// ������������� ����������. �������� ������. ��������� ������� ������ �������� ���� (���������� - ���������� ��������)
// � ������ ��������� ��������� ������ ��� ������� ���������� EXIT_SUCCESSFULLY, ����� EXIT_MEMORY_FAILURE
int corrector_init(Corrector* corrector, unsigned max_word_length, WordsMetric metric_func);

// ������� ������ � ����������. �������� ��������, �������� ������ �������. ���� ������ ��� ��� ��������� ���������� ����������� - ���������� ������������� ��������� free(corrector)
void corrector_destroy(Corrector* corrector);


#endif 

