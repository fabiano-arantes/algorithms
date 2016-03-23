#include <utilcci.h>
#include <stdio.h>

void list_partition(node_int** orig_node, int value)
{
	node_int *node = *orig_node;
	node_int *head = node;
	node_int *tail = node;
	node_int *next = NULL;

	if(head == NULL)
	{
		return;
	}

	while(node != NULL)
	{
		next = node->next;

		if(node->d < value)
		{
			DBG("Adding before head (%d): node->d = %d", head->d, node->d);
			node->next = head;
			head = node;
		}
		else
		{
			DBG("Appending after tail (%d) : node->d = %d", tail->d, node->d);
			tail->next = node;
			tail = node;
		}

		node = next;
	}
	tail->next = NULL;

	*orig_node = head;
}

int main(int argc, char * argv[])
{
	node_int *head;
	int value;

	fill_linked_list_int(&head);

	print_linked_list_int(head);

	printf("Value of element: ");
	scanf("%d", &value);

	list_partition(&head, value);

	print_linked_list_int(head);

	return 0;
}
