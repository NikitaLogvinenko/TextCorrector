#ifndef _CORRECTOR_MODEL_
#define _CORRECTOR_MODEL_

typedef void* Pointer;
typedef void (*Destructor)(Pointer);

/*
������������� ������.
������ ������������ ����� �����, ��������� ��������������(treshold, tol), ������ ������� �������(������ ������ - ����� ����������� �����), ������� ������� �������
*/
typedef struct Corrector
{
	int max_word_size;
	int treshold;
	Destructor dtor;
} Corrector;

#endif

