#include <utilcci.h>
#include <stdio.h>

node_int *find_kth_recursive(node_int* node, int kth, int *count)
{
	node_int *n;

	if(node == NULL)
	{
		*count = 0;
		return NULL;
	}

	n = find_kth_recursive(node->next, kth, count);

	if(n == NULL)
	{
		++*count;

		if(kth == *count)
		{
			DBG("Node found: node->d = %d", node->d);
			return node;
		}
	}

	return n;
}

node_int *find_kth_reverse(node_int* head, const int kth)
{
	node_int *n;
	int count = 0;

	return find_kth_recursive(head, kth, &count);
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

	kth_node = find_kth_reverse(head, kth);

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
