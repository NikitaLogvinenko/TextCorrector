#include "constants.h"
#include "words_handling.h"
#include "helpful_functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>


unsigned hamming_distance(const char* word1, const char* word2)
{
	unsigned distance;
	if (word1 == NULL && word2 == NULL)
		distance = 0;
	else if (word1 == NULL)
		distance = strlen(word2);
	else if (word2 == NULL)
		distance = strlen(word1);
	else
	{
		distance = max(strlen(word1), strlen(word2));
		while (*word1 != '\0' && *word2 != '\0')
		{
			distance -= (*word1 == *word2) ? 1 : 0;
			++word1;
			++word2;
		}
	}
	return distance;
}

int word_to_upper(const char* initial_word, char* editted_word)
{
	int changes = 0;
	if (initial_word == NULL || editted_word == NULL)
		changes = -1;
	else
	{
		bool flag = true;
		while (flag != false)  // записываем до '\0' включительно
		{
			int initial_symbol_code = (int)*initial_word;
			int uppered_symbol_code = letter_to_upper(initial_symbol_code);
			if (uppered_symbol_code != initial_symbol_code)
				++changes;
			*editted_word = (char)uppered_symbol_code;
			++initial_word;
			++editted_word;
			if (uppered_symbol_code == 0)  // записываем до '\0' включительно
				flag = false;
		}
	}
	return changes;
}

int word_to_lower(const char* initial_word, char* editted_word)
{
	int changes = 0;
	if (initial_word == NULL || editted_word == NULL)
		changes = -1;
	else
	{
		bool flag = true;
		while (flag != false)  // записываем до '\0' включительно
		{
			int initial_symbol_code = (int)*initial_word;
			int lowered_symbol_code = letter_to_lower(initial_symbol_code);
			if (lowered_symbol_code != initial_symbol_code)
				++changes;
			*editted_word = (char)lowered_symbol_code;
			++initial_word;
			++editted_word;
			if (lowered_symbol_code == 0)  // записываем до '\0' включительно
				flag = false;
		}
	}
	return changes;
}

int letter_to_upper(int code)
{
	int uppered_symbol_code = code;
	if (is_lower_letter_eng(code))
		uppered_symbol_code = uppered_symbol_code - FIRST_ENG_LOWER + FIRST_ENG_CAPITAL;
	else if (code == YO_LOWER)
		uppered_symbol_code = YO_CAPITAL;
	else if (is_lower_letter_rus(code))
		uppered_symbol_code = uppered_symbol_code - FIRST_RUS_LOWER + FIRST_RUS_CAPITAL;
	return uppered_symbol_code;
}

int letter_to_lower(int code)
{
	int lowered_symbol_code = code;
	if (is_upper_letter_eng(code))
		lowered_symbol_code = lowered_symbol_code - FIRST_ENG_CAPITAL + FIRST_ENG_LOWER;
	else if (code == YO_CAPITAL)
		lowered_symbol_code = YO_LOWER;
	else if (is_upper_letter_rus(code))
		lowered_symbol_code = lowered_symbol_code - FIRST_RUS_CAPITAL + FIRST_RUS_LOWER;
	return lowered_symbol_code;
}

bool is_letter(int code)
{
	return (is_eng_letter(code) || is_rus_letter(code));
}

bool is_eng_letter(int code)
{
	return (is_upper_letter_eng(code) || is_lower_letter_eng(code));
}

bool is_rus_letter(int code)
{
	return (is_upper_letter_rus(code) || is_lower_letter_rus(code));
}

bool is_upper_letter(int code)
{
	return (is_upper_letter_eng(code) || is_upper_letter_rus(code));
}

bool is_lower_letter(int code)
{
	return (is_lower_letter_eng(code) || is_lower_letter_rus(code));
}

bool is_upper_letter_eng(int code)
{
	return (code >= FIRST_ENG_CAPITAL && code <= LAST_ENG_CAPITAL);
}

bool is_lower_letter_eng(int code)
{
	return (code >= FIRST_ENG_LOWER && code <= LAST_ENG_LOWER);
}

bool is_upper_letter_rus(int code)
{
	return (code >= FIRST_RUS_CAPITAL && code <= LAST_RUS_CAPITAL || code == YO_CAPITAL);
}

bool is_lower_letter_rus(int code)
{
	return (code >= FIRST_RUS_LOWER && code <= LAST_RUS_LOWER || code == YO_LOWER);
}

bool read_train_word(FILE* text_file, char* buffer, unsigned max_word_length)
{
	assert(max_word_length != 0);
	// проверка, что файл открыт, выполнена ранее, в train_one_file, откуда и вызывается функция read_train_word
	bool file_not_ended = true;
	int new_symbol = fgetc(text_file);  // fgetc возвращает от 0 до 255, т.е. для unsigned char. В случае конца файла возвращает -1. А обычные char записываются от -128 до 127
	while (new_symbol != EOF && (isspace(new_symbol) != 0 || ispunct(new_symbol) != 0))  // а вот функции из модуля <ctype.h> работают с unsigned char
		new_symbol = fgetc(text_file);  // пропускаем все пробельные символы и знаки препинания, пока не встретим букву или конец файла

	if (new_symbol == EOF)  // дошли до конца файла и не встретили слово
	{
		buffer[0] = '\0';
		file_not_ended = false;
	}
	else  // нашли слово
	{
		int counter = 0;
		while (counter < max_word_length && new_symbol != EOF && isspace(new_symbol) == 0 && ispunct(new_symbol) == 0 || new_symbol == (int)'-')
		{
			// записываем новые символы в буфер, пока не превысим лимит или пока не встретим пробельный или пунктуационный символ или конец файла
			buffer[0] = new_symbol;
			++counter;
			++buffer;
			new_symbol = fgetc(text_file);
		}
		
		if (new_symbol == EOF)  // дошли до конца файла
		{
			file_not_ended = false;
			buffer[0] = '\0';
		}
		
		else if (isspace(new_symbol) != 0 || ispunct(new_symbol) != 0)  // целиком прочитали слово
			buffer[0] = '\0';
		
		else  // превышен лимит по буквам, зануляем всё слово и пропускаем оставшиеся символы до пробельного
		{
			*(buffer - max_word_length) = '\0';
			while (isspace(new_symbol) == 0)
			{
				new_symbol = fgetc(text_file);
				if (new_symbol == EOF)
				{
					file_not_ended = false;
					new_symbol = (unsigned char)' ';
				}
					
			}
		}
	}

	return file_not_ended;
}

const char* prepare_train_word(const char* word)
{
	char* prepared_word = NULL;
	int not_letters_and_hyphens = wrong_symbols_in_word(word), letters_and_hyphens = letters_and_hyphens_in_word(word);
	if (word != NULL && not_letters_and_hyphens == 0 && letters_and_hyphens != 0)
	{
		prepared_word = (char*)retry_malloc(sizeof(char) * (letters_and_hyphens + 1), MAX_MALLOC_ATTEMPTS);
		if (prepared_word != NULL)
			word_to_lower(word, prepared_word);
	}
	return prepared_word;
}

int wrong_symbols_in_word(const char* word)
{
	int counter = 0;
	if (word != NULL)
	{
		while (*word != '\0')
		{
			if (is_letter(*word) == false && *word != '-')
				++counter;
			++word;
		}
	}
	return counter;
}

int letters_and_hyphens_in_word(const char* word)
{
	int counter = 0;
	if (word != NULL)
	{
		while (*word != '\0')
		{
			if (is_letter(*word) || *word == '-')
				++counter;
			++word;
		}
	}
	return counter;
}