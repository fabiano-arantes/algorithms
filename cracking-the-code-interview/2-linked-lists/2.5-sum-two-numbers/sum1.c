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

void fill_linked_list_array(node_int** head, char *num)
{
	node_int* node = *head;

	while((*num != '\n') && (*num != '\0'))
	{
		DBG("*num = %d", *num - '0');
		if(*head == NULL)
		{
			*head = new_node_int();
			node = *head;
		}
		else
		{
			node->next = new_node_int();
			node = node->next;
		}

		node->d = *num - '0';
		++num;
	}
}

int main(int argc, char * argv[])
{
	char num[256];

	node_int *result;
	node_int *num1_head = NULL;
	node_int *num2_head = NULL;
	node_int *expected = NULL;

	fgets(num, 256, stdin);

	if((*num == '\n') || (*num == '\0'))
	{
		printf("%s", "Empty num array");
	}

	fill_linked_list_array(&num1_head, num);
	print_linked_list_int(num1_head);

	fgets(num, 256, stdin);

	if((*num == '\n') || (*num == '\0'))
	{
		printf("%s", "Empty num array");
	}

	fill_linked_list_array(&num2_head, num);
	print_linked_list_int(num2_head);

	fgets(num, 256, stdin);

	if((*num == '\n') || (*num == '\0'))
	{
		printf("%s", "Empty num array");
	}

	fill_linked_list_array(&expected, num);
	print_linked_list_int(expected);

	result = sum(num1_head, num2_head);
	print_linked_list_int(result);

	while(expected != NULL)
	{
		if(expected->d != result->d)
		{
			printf("Sum error\n");
			break;
		}
		expected = expected->next;
		result = result->next;
	}

	erase_linked_list_int(num1_head);
	erase_linked_list_int(num2_head);
	erase_linked_list_int(result);
	erase_linked_list_int(expected);


	return 0;
}
