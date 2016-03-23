#include <utilcci.h>
#include <stdio.h>

node_int *find_kth_reverse(node_int* head, const int kth)
{
	node_int *n;
	int count = 0;
	int pos;

	if(head == NULL)
	{
		return NULL;
	}

	n = head;

	while(n != NULL)
	{
		++count;
		n = n->next;
	}

	DBG("Number of elements in linked list: %d", count);

	if(kth > count)
	{
		DBG("Index out of range: kth > count : %d > %d", kth, count);
		return NULL;
	}

	pos = count - kth;
	n = head;

	DBG("Position of kth element in linked list: %d", pos + 1);

	while(pos > 0)
	{
		n = n->next;
		--pos;
	}

	return n;
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
