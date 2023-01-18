//#ifndef _HASH_TABLE_LIB_
//#define _HASH_TABLE_LIB_
//
//
//#include "hash_table.h"
//#include <stdbool.h>
//
//
///* ���������������� �������. 
//������� ��� �������! �� ������ �������������� �
// *
// * size - ������ �������� �������;
// * hf   - ���-�������;
// * dtor - ����������. ���� ������� ����� ������������ data ��������� ���������
// *        (ht_destroy, ht_delete, � ����� ��� ���������� ��������� � ht_set).
// *
// * ���� hf=0, ������� ����������� ������� (��������).
// * ���� dtor=0, ���������� ����������� (�� ����� ����������).
// */
//void ht_init(HashTable* ht, size_t size, HashFunction hf, Destructor dtor);
//
///* 
//���������� ������� 
//�������� ������ ������ ������� ���� HashTable ������ ����������� �������, �.�. ������������, ��������, ������� ������ ��� ������ ����������
//������ data ��������� � ������� ���������� ��� �������� ������� dtor
//*/
//void ht_destroy(HashTable* ht);
//
///* �������� � ������� ������������ key -> data. ���� key ��� �����������,
// * �����. ���� data ����� ������� (����� dtor) � ������������ */
//void ht_set(HashTable* ht, const char* key, const Pointer data);
//
///* �������� �������� �� �����. ���� ����� ��� � �������, ������� NULL. */
//Pointer ht_get(const HashTable* ht, const char* key);
//
///* �������� ������������� ����� key � ������� */
//bool ht_has(const HashTable* ht, const char* key);
//
///* ������� ������� � ������ key �� ������� (���� �� ����) */
//void ht_delete(HashTable* ht, const char* key);
//
///* ����� ������� � ���������� ���� ���������. ������� f ����� ������� ���
// * ���� ��� (key, data) �� �������. ���� ������ ������!*/
//void ht_traverse(HashTable* ht, void (*f)(const char* key, Pointer data));
//
///* �������� ������ �������� �������. */
//void ht_resize(HashTable* ht, size_t new_size);
//
//
//#endif