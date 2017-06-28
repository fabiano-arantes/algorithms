/*
Test: https://www.hackerrank.com/challenges/contacts

Author: Fabiano Arantes
Contact: https://www.linkedin.com/in/fabianoarantes/
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <vector>

static const size_t SIZE = 22;
static const unsigned char ALPHABET_SIZE = 'z' - 'a' + 1;

typedef struct TrieNode
{
	struct TrieNode* child[ALPHABET_SIZE];
    unsigned int count;

}TrieNode;

TrieNode* trie;

TrieNode* newTrieNode(void)
{
	TrieNode* tn = new TrieNode();

    tn->count = 1;
    
	for(unsigned int i = 0; i < ALPHABET_SIZE; ++i)
	{
		tn->child[i] = NULL;
	}

	return tn;
}

void addNewNode(char *input)
{	
	static char c;
	size_t size = strlen(input);
	TrieNode *tn = trie;
	unsigned int count = 0;

	for(size_t i = 0; i < size; ++i)
	{
		c = input[i] - 'a';

		++(tn->count);

		if(tn->child[c] == NULL)
			tn->child[c] = newTrieNode();

		tn = tn->child[c];
	}


}

unsigned int countContacts(char* input)
{
	static char c;
	size_t size = strlen(input);
	unsigned int count = 0;
	TrieNode *tn = trie;

	for(size_t i = 0; i < size; ++i)
	{
		c = input[i] - 'a';

		if(tn->child[c] == NULL)
			return 0;

		tn = tn->child[c];
	}

	return tn->count;
}

int main() {

	int n;
	char cmd[5];
	char input[SIZE];

	trie = newTrieNode();

	scanf("%d", &n);

	while(n-- > 0)
	{
		scanf("%s %s", cmd, input);

		if(strcmp(cmd, "add") == 0)
			addNewNode(input);
		else if(strcmp(cmd, "find") == 0)
			printf("%d\n", countContacts(input));
	}

    return 0;
}
