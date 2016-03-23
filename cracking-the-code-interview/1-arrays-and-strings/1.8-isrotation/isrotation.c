#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
	#define DBG(x, ...) printf(x"\n", __VA_ARGS__)
#else
	#define DBG(x, ...)
#endif

int main(int argc, char * argv[])
{
	char *str;
	size_t size;

	if(argc != 3)
	{
		printf("Usage format: %s <string1> <string2>\n", argv[0]);
		return 1;
	}

	size = strlen(argv[1]);

	DBG("Size of string 1: %zu", size);
	DBG("Size of string 2: %zu", strlen(argv[2]));

	if(size != strlen(argv[2]))
	{
		printf("String1 (%s) is NOT a rotation of String2 (%s): different sizes\n", argv[1], argv[2]);
		return 1;
	}

	DBG("Content of string 1: %s", argv[1]);
	DBG("Content of string 2: %s", argv[2]);

	str = malloc(2 * (sizeof(char) * size) + 1);

	strcpy(str, argv[1]);
	DBG("Content of str: %s", str);

	strcat(str, argv[1]);
	DBG("Content of str after concatenation: %s", str);

	if(strstr(str, argv[2]) == NULL)
	{
		printf("String1 (%s) is NOT a rotation of String2 (%s)\n", argv[1], argv[2]);
	}
	else
	{
		printf("String1 (%s) is a rotation of String2 (%s)\n", argv[1], argv[2]);
	}

	return 0;
}
