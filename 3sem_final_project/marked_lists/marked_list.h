#ifndef _MARKED_LIST_
#define _MARKED_LIST_


// word - ��������� �� �����. ������������ ������. �������� � mlist_destroy
// counter - ������� ��� ����� ����������� ��� ��������. ����������� ������
typedef struct MarkedList {
    const char* word;
    int counter;
    struct MarkedList* next;
} MList;


#endif