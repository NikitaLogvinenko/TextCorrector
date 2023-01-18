//#include "hash_table.h"
//#include "hash_table_lib.h"
//#include "hash_table_test.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <assert.h>
//#include <locale.h>
//
//
//static void print_int_ht(HashTable* ht);
//
//static void print_int_mlist(MList* ml);
//
//static void print_removed_data(int* data);
//
//static void add_one(const char* key, int* data);
//
//
//void hash_table_tests()
//{
//	printf("*******************************************************************************************\n");
//	printf("HashTable tests were launched\n");
//	HashTable ht;
//	ht_init(&ht, 5, NULL, print_removed_data);
//	// добавим данные в таблицу
//	int Roma182 = 182;
//	ht_set(&ht, "Рома", &Roma182);
//	int Victor160 = 160;
//	ht_set(&ht, "Виктор", &Victor160);
//	int Alex170 = 170;
//	ht_set(&ht, "Александр", &Alex170);
//	int Nastya172 = 172;
//	ht_set(&ht, "Настя", &Nastya172);
//	int Masha164 = 164;
//	ht_set(&ht, "Маша", &Masha164);
//	int Slava189 = 189;
//	ht_set(&ht, "Слава", &Slava189);
//	int Saveliy179 = 179;
//	ht_set(&ht, "Савелий", &Saveliy179);
//	int Andrey202= 202;
//	ht_set(&ht, "Андрей", &Andrey202);
//	int Vova140= 140;
//	ht_set(&ht, "Вова", &Vova140);
//	int Olya173 = 173;
//	ht_set(&ht, "Оля", &Olya173);
//	print_int_ht(&ht);
//	// добавим данные по старым ключам
//	int Slava181 = 181;
//	ht_set(&ht, "Слава", &Slava181);
//	int Saveliy167 = 167;
//	ht_set(&ht, "Савелий", &Saveliy167);
//	int Andrey200 = 200;
//	ht_set(&ht, "Андрей", &Andrey200);
//	print_int_ht(&ht);
//	// проверим функцию проверки наличия ключа
//	assert(ht_has(&ht, "Слава"));
//	assert(ht_has(&ht, "Савелий"));
//	assert(!ht_has(&ht, "савелий"));
//	assert(!ht_has(&ht, "Аркадий"));
//	// проверим функцию возвращения данных по ключу
//	assert(*((int*)ht_get(&ht, "Рома")) == 182);
//	assert(*((int*)ht_get(&ht, "Виктор")) == 160);
//	assert(*((int*)ht_get(&ht, "Слава")) >= 181);
//	// добавим ещё записей
//	int Victor1 = 160;
//	ht_set(&ht, "Виктор1", &Victor1);
//	int Victor2 = 160;
//	ht_set(&ht, "Виктор2", &Victor2);
//	int Victor3 = 160;
//	ht_set(&ht, "Виктор3", &Victor3);
//	int Victor4 = 160;
//	ht_set(&ht, "Виктор4", &Victor4);
//	int Victor5 = 160;
//	ht_set(&ht, "Виктор5", &Victor5);
//	int Victor6 = 160;
//	ht_set(&ht, "Виктор6", &Victor6);
//	int Victor7 = 160;
//	ht_set(&ht, "Виктор7", &Victor7);
//	int Victor8 = 160;
//	ht_set(&ht, "Виктор8", &Victor8);
//	// удалим Виктора2 и Виктора6, а также несуществующих Виктора10 и Аркадия
//	assert(ht_has(&ht, "Виктор2"));
//	assert(ht_has(&ht, "Виктор6"));
//	ht_delete(&ht, "Виктор2");
//	ht_delete(&ht, "Виктор6");
//	assert(!ht_has(&ht, "Виктор2"));
//	assert(!ht_has(&ht, "Виктор6"));
//	ht_delete(&ht, "Виктор10");
//	ht_delete(&ht, "Аркадий");
//	// увеличим таблицу
//	printf("Старая маленькая таблица:\n");
//	print_int_ht(&ht);
//	ht_resize(&ht, 20);
//	printf("Новая таблица побольше:\n");
//	print_int_ht(&ht);
//	// увеличим все значения на 1
//	ht_traverse(&ht, add_one);
//	assert(*((int*)ht_get(&ht, "Рома")) == 183);
//	assert(*((int*)ht_get(&ht, "Виктор")) == 161);
//	print_int_ht(&ht);
//
//	printf("------------Конец теста. Удаление хеш-таблицы------------\n");
//	ht_destroy(&ht);
//	printf("All HashTable tests were passed successfully!\n");
//	printf("*******************************************************************************************\n");
//}
//
//static void print_int_ht(HashTable* ht)
//{
//	setlocale(LC_ALL, "ru");
//	if (ht == NULL) return;
//	for (int i = 0; i < ht->size; ++i)
//	{
//		printf("[%d] ", i);
//		print_int_mlist(ht->table[i]);
//	}
//}
//
//static void print_int_mlist(MList* ml)
//{
//	setlocale(LC_ALL, "ru");
//	while (ml != NULL)
//	{
//		printf("(%s: %d)\t", ml->key, *((int*)ml->data));
//		ml = ml->next;
//	}
//	printf("\n");
//}
//
//static void print_removed_data(int* data)
//{
//	printf("Удалено значение из таблицы: %d\n", *data);
//}
//
//static void add_one(const char* key, int* data)
//{
//	++* data;
//	printf("Data with the key \"%s\" was increased by 1\n", key);
//}