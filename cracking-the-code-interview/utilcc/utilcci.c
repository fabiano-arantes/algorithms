#include "utilcci.h"
#include <stdio.h>

node_int *new_node_int(void)
{
	node_int *n = NULL;

	n = malloc(sizeof(node_int));

	if(n == NULL)
	{
		return NULL;
	}

	n->d = 0;
	n->next = NULL;

	return n;
}

void fill_linked_list_int(node_int **head)
{
	node_int *tmp = NULL;
	int count, v;

	printf("Number of elements: ");
	scanf("%d", &count);

	while(count > 0)
	{
		--count;

		scanf("%d", &v);

		if(tmp == NULL)
		{
			tmp = new_node_int();
		}
		else
		{
			tmp->next = new_node_int();
			tmp = tmp->next;
		}
		tmp->d = v;

		if(!*head)
		{
			*head = tmp;
		}
	}

}

void print_linked_list_int(node_int *head)
{
	node_int *tmp = head;

	while(tmp != NULL)
	{
		printf("%d", tmp->d);

		if(tmp->next != NULL)
		{
			printf("->");
		}

		tmp = tmp->next;
	}
	printf("\n");
}

void erase_linked_list_int(node_int* node)
{
	node_int* tmp;

	while(node != NULL)
	{
		tmp = node->next;
		free(node);
		node = tmp;
	}
}

