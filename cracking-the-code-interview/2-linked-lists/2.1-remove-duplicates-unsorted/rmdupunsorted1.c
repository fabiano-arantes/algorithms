#include <utilcci.h>
#include <stdio.h>

static node_int *sorted_list = NULL;

int check_duplicate(int value)
{
	node_int *tmp;
	node_int *last = NULL;
	node_int *n = NULL;

	if(sorted_list == NULL)
	{
		DBG("Creating sorted list: %d", value);
		sorted_list = new_node_int();
		sorted_list->d = value;
		return 0;
	}

	tmp = sorted_list;
	last = sorted_list;

	while(tmp)
	{
		if(value > tmp->d)
		{
			DBG("value > tmp->d : %d > %d", value, tmp->d);
			last = tmp;
			tmp = tmp->next;
		}
		else if(tmp->d == value)
		{
			return 1;
		}
		else
		{
			DBG("New unique value: %d", value);
			n = new_node_int();
			last->next = n;
			n->d = value;
			n->next = tmp;

			return 0;
		}
	}

	DBG("New unique value: %d", value);
	n = new_node_int();
	n->d = value;
	last->next = n;

	return 0;
}

int main(int argc, char * argv[])
{
	node_int *head = NULL;
	node_int *n = NULL;
	node_int *last = NULL;

	fill_linked_list_int(&head);

	print_linked_list_int(head);

	n = head;

	while(n != NULL)
	{
		DBG("Checking [%p] = %d", n, n->d);
		if(check_duplicate(n->d))
		{
			DBG("Duplicate found: %d", n->d);
			last->next = n->next;
			free(n);

			if(last->next == NULL)
			{
				break;
			}

			n = last;
		}
		last = n;
		n = n->next;
	}

	print_linked_list_int(head);

	free(head);

	return 0;
}
