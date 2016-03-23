#include <utilcci.h>
#include <stdio.h>

int main(int argc, char * argv[])
{
	node_int *head;
	node_int *element;
	node_int *tmp;

	int kth;

	fill_linked_list_int(&head);

	print_linked_list_int(head);

	printf("Kth element to del: ");
	scanf("%d", &kth);

	if(kth == 1)
	{
		printf("First element not allowed\n");
		return 1;
	}
	else if(kth < 1)
	{
		printf("Kth out of range\n");
		return 1;
	}

	element = head;
	while((kth > 1) && (element != NULL) && (element->next != NULL))
	{
		--kth;
		element = element->next;
	}

	if(element == NULL)
	{
		printf("Kth out of range\n");
		return 1;
	}
	else if(element->next == NULL)
	{
		printf("Last element not allowed\n");
		return 1;
	}

	tmp = element->next;
	element->d = tmp->d;
	element->next = tmp->next;
	free(tmp);

	print_linked_list_int(head);

	return 0;
}
