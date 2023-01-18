#ifndef _MARKED_LIST_
#define _MARKED_LIST_


// word - указатель на слово. ƒинамическа€ пам€ть. „иститс€ в mlist_destroy
// counter - сколько раз слово встретилось при обучении. —татическа€ пам€ть
typedef struct MarkedList {
    const char* word;
    int counter;
    struct MarkedList* next;
} MList;


#endif