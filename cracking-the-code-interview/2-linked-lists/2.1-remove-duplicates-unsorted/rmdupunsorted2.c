#include <utilcci.h>
#include <stdio.h>

int main(int argc, char * argv[])
{
	node_int *head = NULL;
	node_int *first = NULL;
	node_int *second = NULL;
	node_int *past = NULL;

	fill_linked_list_int(&head);

	print_linked_list_int(head);

	first = head;

	while(first != NULL)
	{
		second = first->next;
		past = second;

		DBG("first[%p] = %d", first, first->d);
		while(second)
		{
			DBG("second[%p] = %d", second, second->d);
			if(second->d == first->d)
			{
				DBG("Duplicate found: %d", second->d);
				past->next = second->next;
				free(second);

				if(past->next == NULL)
				{
					break;
				}

				second = past;
			}
			past = second;
			second = second->next;
		}

		first = first->next;
	}

	print_linked_list_int(head);

	free(head);

	return 0;
}
