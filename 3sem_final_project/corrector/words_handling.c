#include "constants.h"
#include "words_handling.h"
#include <stdlib.h>
#include <string.h>


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