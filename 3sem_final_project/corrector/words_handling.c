#include "constants.h"
#include "words_handling.h"
#include "helpful_functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


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

int word_to_upper(const char* initial_word, char* edited_word)
{
	int changes = 0;
	if (initial_word == NULL || edited_word == NULL)
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
			*edited_word = (char)uppered_symbol_code;
			++initial_word;
			++edited_word;
			if (uppered_symbol_code == 0)  // записываем до '\0' включительно
				flag = false;
		}
	}
	return changes;
}

int word_to_lower(const char* initial_word, char* edited_word)
{
	int changes = 0;
	if (initial_word == NULL || edited_word == NULL)
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
			*edited_word = (char)lowered_symbol_code;
			++initial_word;
			++edited_word;
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
		uppered_symbol_code = uppered_symbol_code - 'a' + 'A';
	else if (code < 0 && code == 'ё' || code >= 0 && code == (unsigned char)'ё')
		uppered_symbol_code = (char)'Ё';
	else if (is_lower_letter_rus(code))
		uppered_symbol_code = uppered_symbol_code - 'а' + 'А';
	return uppered_symbol_code;
}

int letter_to_lower(int code)
{
	int lowered_symbol_code = code;
	if (is_upper_letter_eng(code))
		lowered_symbol_code = lowered_symbol_code - 'A' + 'a';
	else if (code < 0 && code == 'Ё' || code >= 0 && code == (unsigned char)'Ё')
		lowered_symbol_code = (char)'ё';
	else if (is_upper_letter_rus(code))
		lowered_symbol_code = lowered_symbol_code - 'А' + 'а';
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
	code = signed_to_unsigned_char(code);
	return (code >= (unsigned char)'A' && code <= (unsigned char)'Z');
}

bool is_lower_letter_eng(int code)
{
	code = signed_to_unsigned_char(code);
	return (code >= (unsigned char)'a' && code <= (unsigned char)'z');
}

bool is_upper_letter_rus(int code)
{
	code = signed_to_unsigned_char(code);
	return (code >= (unsigned char)'А' && code <= (unsigned char)'Я' || code == (unsigned char)'Ё');
}

bool is_lower_letter_rus(int code)
{
	code = signed_to_unsigned_char(code);
	return (code >= (unsigned char)'а' && code <= (unsigned char)'я' || code == (unsigned char)'ё');
}

const char* prepare_train_word(const char* word)
{
	char* prepared_word = NULL;
	int not_letters_and_hyphens = wrong_symbols_in_word(word), letters_and_hyphens = letters_and_hyphens_in_word(word);
	if (word != NULL && not_letters_and_hyphens == 0 && letters_and_hyphens != 0 && has_double_hyphens(word) == false)
	{
		// В слове только буквы и дефисы, при этом не 0 штук и дефисы не идут подряд
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

int letters_in_word(const char* word)
{
	int counter = 0;
	if (word != NULL)
	{
		while (*word != '\0')
		{
			if (is_letter(*word))
				++counter;
			++word;
		}
	}
	return counter;
}

bool space_demanding(int symbol_code)
{
	bool demanding = true;
	if (symbol_code < 0 && symbol_code != EOF)  // в функцию должны передаваться unsigned char, но получили отрицательный код не EOF - поставим пробел на всякий случай
		demanding = true;
	else if (symbol_code == EOF || isspace(symbol_code) != 0)  // EOF и пробельные символы не требуют после себя ещё одного пробела
		demanding = false;
	else  // также не требуют пробел некоторые другие символы
	{
		unsigned char not_demanding[NOT_DEMANDING_SPACE] = { '"', '\'', '`', '~', '(', '[', '{', '<', '\\', '|', '/', '@', '#', '$', '&', '_' };
		for (unsigned i = 0; i < NOT_DEMANDING_SPACE && demanding; ++i)
		{
			if (symbol_code == not_demanding[i])
				demanding = false;
		}
	}
	return demanding;
}

unsigned signed_to_unsigned_char(int code)
{
	if (code < 0)
	{
		char signed_char = code;
		code = (unsigned char)signed_char;
	}
	return code;
}

int space_before(FILE* edited_file, int prev_code, int symbol_code)
{
	if (space_demanding(prev_code) && is_letter(symbol_code))  // ранее ввели символ, требующий после себя пробел, а новый символ - буквенный
	{
		fputc(' ', edited_file);
		prev_code = ' ';
	}
	return prev_code;
}

int erase_punct_remainder(char* string_with_punct)
{
	int null_term_index = 0;
	if (string_with_punct != NULL)
	{
		null_term_index = strlen(string_with_punct);
		for (; null_term_index != 0 && ispunct(signed_to_unsigned_char(string_with_punct[null_term_index - 1])) != 0; --null_term_index)
			string_with_punct[null_term_index - 1] = '\0';
	}
	return null_term_index;
}

int erase_not_letter_remainder(char* string_with_punct)
{
	int null_term_index = 0;
	if (string_with_punct != NULL)
	{
		null_term_index = strlen(string_with_punct);
		for (; null_term_index != 0 && is_letter(string_with_punct[null_term_index - 1]) == false; --null_term_index)
			string_with_punct[null_term_index - 1] = '\0';
	}
	return null_term_index;
}

bool has_double_hyphens(const char* word)
{
	bool answer = false;
	if (word != NULL && strlen(word) > 1)
	{
		char prev_symbol = word[0], current_symbol = word[1];
		while (answer == false && current_symbol != '\0')
		{
			if (prev_symbol == '-' && current_symbol == '-')
				answer = true;
			else if (current_symbol == '-')
			{
				word += 1;
				prev_symbol = word[0];
				current_symbol = word[1];
			}
			else if (word[2] != '\0')
			{
				word += 2;
				prev_symbol = word[0];
				current_symbol = word[1];
			}
			else
				current_symbol = '\0';
		}
	}
	return answer;
}

int capital_counter(const char* word)
{
	int counter = 0;
	if (word != NULL)
	{
		for (; *word != '\0'; ++word)
			counter += is_upper_letter(*word);
	}
	return counter;
}