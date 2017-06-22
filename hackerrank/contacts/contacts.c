#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define SIZE 22

typedef struct trieNode_
{
	char name[SIZE];
	char wholeWord;
	struct trieNode_* l;
	struct trieNode_* e;
	struct trieNode_* b;
}TrieNode;

TrieNode* newTrieNode(char *name)
{
	TrieNode* n = malloc(sizeof(TrieNode));

	strncpy(n->name, name, SIZE);
	n->wholeWord = 0;
	n->l = NULL;
	n->e = NULL;
	n->b = NULL;

	return n;
}

void addNewNode(TrieNode** root, char* name, unsigned int p)
{
	size_t size = strlen((*root)->name);

	if(p >= size)
	{
		return;
	}

	if(*root == NULL)
	{
		*root = newTrieNode(name);
	}
	else
	{
		TrieNode* n = newTrieNode(name);
		unsigned int ii;

		for(ii = p; ii < size; ++ii)
		{
			if(name[ii] > (*root)->name[ii])
			{
				addNewNode(&((*root)->b), name + ii, ii);
				break;
			}
			else if(name[ii] < (*root)->name[ii])
			{
				addNewNode(&((*root)->l), name + ii, ii);
				break;
			}
		}

		if(ii < size)
		{
			addNewNode(&((*root)->e), name + ii + 1, ii + 1);
			(*root)->name[ii + 1] = '\0';
		}
	}

}

int countContacts(TrieNode* root, char* partial, unsigned int p)
{
	size_t size = strlen(partial);

	if(p >= size)
		return 0;

	for(unsigned int ii = p; ii < size; ++ii)
	{
		if(partial[ii] > root->name[ii])
			return countContacts(root->b, partial + ii, ii);
		else if(partial[ii] < root->name[ii])
			return countContacts(root->l, partial + ii, ii);
	}


}

int main() {

	int n;
	char cmd[4];
	char input[SIZE];
	TrieNode* root = NULL;

	scanf("%d", &n);

	while(n-- > 0)
	{
		scanf("%s %s", cmd, input);

		if(strcmp(cmd, "add") == 0)
			addNewNode(&root, input, 0);
		else if(strcmp(cmd, "find") == 0)
			printf("%d", countContacts(root, input, 0));
	}

    /* Enter your code here. Read input from STDIN. Print output to STDOUT */    
    return 0;
}
