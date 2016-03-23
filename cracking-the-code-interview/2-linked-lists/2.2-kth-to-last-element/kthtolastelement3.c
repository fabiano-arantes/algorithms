#include <utilcci.h>
#include <stdio.h>

node_int *find_kth_reverse_iterative(node_int* head, const int kth)
{
	node_int *n1, *n2;
	int i;

	if(head == NULL)
	{
		return NULL;
	}

	n1 = head;
	n2 = head;

	for(i = 0; i < kth - 1; ++i)
	{
		if(n2 == NULL)
		{
			break;
		}
		n2 = n2->next;
	}

	if(n2 == NULL)
	{
		DBG("Kth element out of range: kth = %d", kth);
		return NULL;
	}

	while(n2->next != NULL)
	{
		n1 = n1->next;
		n2 = n2->next;
	}

	return n1;
}

int main(int argc, char * argv[])
{
	node_int* head;
	node_int* kth_node;
	int kth;

	fill_linked_list_int(&head);

	print_linked_list_int(head);

	printf("kth = ");
	scanf("%d", &kth);

	kth_node = find_kth_reverse_iterative(head, kth);

	if(kth_node != NULL)
	{
		printf("Kth reverse element: [%p] = %d\n", kth_node, kth_node->d);
	}
	else
	{
		printf("Kth reverse element not found\n");
	}
	return 0;
}
