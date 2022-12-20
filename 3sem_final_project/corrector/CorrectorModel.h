#ifndef _CORRECTOR_MODEL_
#define _CORRECTOR_MODEL_

typedef void* Pointer;
typedef void (*Destructor)(Pointer);

/*
������������� ������.
������ ������������ ����� �����, ������ ������� ������� (������ ������ - ��������� ����� ����������� �����), ������� ������� �������
*/
typedef struct Corrector
{
	int max_word_size;
	Destructor dtor;
} Corrector;

#endif

