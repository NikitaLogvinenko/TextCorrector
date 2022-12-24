# TextCorrector
## НГУ. ФФ. Физическая информатика
### Структурное программирование. 3 семестр. Проект
### TextCorrector
#### Что делает:
- Программа для редактирования текста.
- Работа через консоль.
- В параметрах при запуске передаётся путь к редактируемому файлу.
- Отредактированный файл сохраняется под требуемым именем.
#### Как делает:
- В основе лежит модель, запоминающая существующие слова по набору текстов на этапе обучения.
- При редактировании текста заменяет незнакомые слова в тексте на максимально похожие выученные. (см. далее)

Слова хранятся в связных списках. Каждый связный список хранит слова одной и той же длины в алфавитном порядке. На этапе обучения слова, содержащие символы, кроме русских и английских букв, не запоминаются.

Первоначально при редактировании слова алгоритм ищет совпадения в списке слов такой же длины и если находит - то ничего не меняет.

Если же такого слова нет, то нужно найти максимально близкое слово. Тогда проходимся по спискам изученных слов такой же длины +-`size_tol` (максимально допустимое отклонение в длине слова, нужно задать в параметрах) и для каждого считает отклонение от слова из текста. Из всех слов выбирается слово с минимальным отклонением.

Отклонение считается как количество различающихся букв в соответствующих позициях. При этом если минимальное отклонение меньше, чем параметр `threshold`, то слово также остаётся без изменений, т.к. ему не удалось подобрать адекватную замену.

#### Параметры, определение конфигурации работы программы.
ВСЕ ПАРАМЕТРЫ ДОЛЖНЫ БЫТЬ РАЗДЕЛЕНЫ ПРОБЕЛАМИ!!!

ДВОЙНЫЕ кавычки позволительны (даже обязательны, если параметр - это некоторый путь, содержащий пробелы). Если параметр написан без кавычек - он читается до первого пробела. Если параметр в двойных кавычках - он читается от открывающей до закрывающей кавычки.

Первый параметр при запуске - режим ввода конфигурации
1. `params`
2. `step_by_step` (если параметров нет, то запускается также этот режим)
3. `from_file`
4. `help` - вывести справку
- В режиме `params` вся конфигурация должна быть задана в параметрах вызова
- В режиме `step_by_step` больше никаких параметров нет, по крайней мере мы их не обрабатываем. Пользователя последовательно просят ввести все параметры в консоль. Пользователю даются некоторые пояснения и несколько попыток корректно ввести требуемые параметры конфигурации
- В режиме `from_file` единственный следующий параметр - путь к файлу с конфигурацией (чтобы каждый раз не вводить параметры)

Далее надо определиться с режимом работы:
- `train_new` - обучение новой модели
- `train_existed` - дообучение существующей модели
- `edit` - редактирование текста

В режиме `train_new` нужны:
- путь для сохранения модели в формате __*.txt__ (вместе с новым именем)
- путь к обучающему тексту в формате __*.txt__
- максимальная длина запоминаемых слов (`max_word_size`). От 1 до 35
В режиме `train_existed` нужны:
- путь к существующей модели в формате __*.txt__
- путь к обучающему тексту в формате __*.txt__
В режиме `edit` нужны:
- путь к обученной модели в формате __*.txt__
- путь к редактируемому файлу в формате __*.txt__
- путь для сохранения отредактированного файла в формате __*.txt__ (вместе с новым именем)
- максимально допустимое отклонение длины заменяющего слова от заменяемого (`size_tol`). От 0 до 3
- максимально допустимая разность слов (`threshold`). Если разница для минимально отличающегося слова превышает этот порог, то слово остаётся в тексте без изменений (не удалось подобрать адекватную замену). От 0 до 3

#### Как выглядит окончательный конфиг
В режиме `train_new`:
1. Метка режима, 0, тип int
2. Путь для сохранения новой модели, строка (char*)
3. Путь к обучающему тексту, строка (char*)
4. Максимальная длина запоминаемых слов (`max_word_length`)
- anything else????????????????????????????????????????????????

В режиме `train_existed` (т.к. модель существующая, то параметр `max_word_length` был задан ранее и записан в файл с моделью; он будет установлен при чтении модели из файла, в cfg его писать не нужно):
1. Метка режима, 1, тип int
2. Путь к существующей модели, строка (char*)
3. Путь к обучающему тексту, строка (char*)
- anything else????????????????????????????????????????????????

В режиме `edit`:
1. Метка режима, 2, тип int
2. Путь к обученной модели
3. Путь к редактируемому тексту
4. Путь для сохранения отредактированного текста (путь вместе с именем)
5. Максимально допустимое отклонение длины заменяющего слова от заменяемого (`size_tol`)
6. Максимально допустимая разность слов (`threshold`)
- anything else????????????????????????????????????????????????


