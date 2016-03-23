#include <utilcci.h>
#include <stdio.h>

void list_partition(node_int** head, int value)
{
	node_int *n = *head;
	node_int *first_head = NULL;
	node_int *second_head = NULL;
	node_int *first_end = NULL;
	node_int *second_end = NULL;
	node_int *next = NULL;

	if(*head == NULL)
	{
		return;
	}

	while(n != NULL)
	{
		next = n->next;
		n->next = NULL;

		if(n->d < value)
		{
			if(first_head == NULL)
			{
				DBG("Starting first part: n->d = %d", n->d);
				first_head = n;
				first_end = n;
			}
			else
			{
				DBG("Appending in first part: n->d = %d", n->d);
				first_end->next = n;
				first_end = n;
			}
		}
		else
		{
			if(second_head == NULL)
			{
				DBG("Starting second part: n->d = %d", n->d);
				second_head = n;
				second_end = n;
			}
			else
			{
				DBG("Appending in second part: n->d = %d", n->d);
				second_end->next = n;
				second_end = n;
			}
		}

		n = next;
	}

	if(first_head == NULL)
	{
		DBG("First part is empty. Returning second_head[%p]", second_head);
		*head = second_head;
		return;
	}

	DBG("Merging first and second part [%p]:[%p]", first_end, second_head);

	first_end->next = second_head;
	*head = first_head;
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
