#include <utilcci.h>
#include <stdio.h>

node_int * sum(node_int *num1, node_int* num2)
{
	node_int *result_head = NULL;
	node_int *result = NULL;

	int mod = 0;

	while((num1 != NULL) || (num2 != NULL) || (mod > 0))
	{
		if(result == NULL)
		{
			DBG("%s", "Creating result head");
			result = new_node_int();
			result_head = result;
		}
		else
		{
			DBG("%s", "Creating next result node");
			result->next = new_node_int();
			result = result->next;
		}

		result->d = mod;

		if(num1 != NULL)
		{
			DBG("Sum num1: %d", num1->d);

			result->d += num1->d;
			num1 = num1->next;
		}

		if(num2 != NULL)
		{
			DBG("Sum num2: %d", num2->d);

			result->d += num2->d;
			num2 = num2->next;
		}

		DBG("Sum result: %d", result->d);

		mod = (result->d > 10) ? 1 : 0;
		result->d %= 10;

		DBG("Mod: %d", mod);

	}

	return result_head;
}

int main(int argc, char * argv[])
{
	node_int *result;

	node_int *num1_head = new_node_int();
	node_int *num1 = num1_head;
	num1->d = 7;
	num1->next = new_node_int();
	num1 = num1->next;
	num1->d = 6;
	num1->next = new_node_int();
	num1 = num1->next;
	num1->d = 9;

	print_linked_list_int(num1_head);

	node_int *num2_head = new_node_int();
	node_int *num2 = num2_head;
	num2->d = 5;
	num2->next = new_node_int();
	num2 = num2->next;
	num2->d = 7;
	num2->next = new_node_int();
	num2 = num2->next;
	num2->d = 3;
	num2->next = new_node_int();
	num2 = num2->next;
	num2->d = 9;

	print_linked_list_int(num2_head);

	result = sum(num1_head, num2_head);

	print_linked_list_int(result);

	return 0;
}
