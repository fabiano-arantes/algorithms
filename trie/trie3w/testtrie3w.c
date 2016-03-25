#include <stdio.h>

#include "trie3w.h"

typedef struct
{
    char text[10];
    int value;
} test_data;

int main()
{
    test_data td[] = { {"teste", 1}, {"tesão", 2}, {"testa", 3}, {"telha", 4},
                        {"água", 5}, {"agulha", 10}, {"agora", 10}, {"antena", 10}};
    test_data *tdp = td;
    size_t size = sizeof(td)/sizeof(*td);
    trie_3w trie;
    trie.type_size = sizeof(int);
    trie.root = NULL;

    while(size-- > 0)
    {
        if (trie_put(&trie, tdp->text, &tdp->value) == NULL)
        {
            printf("Test error. trie_put failed: [tdp] = %p, tdp->text = %s, tdp->value = %d\n", tdp, tdp->text, tdp->value);
            return 1;
        }
        ++tdp;
    }

    if (trie_find(&trie, "telha") == NULL)
    {
        printf("Test error. String not found: %s\n", "telha");
    }

    if (trie_find(&trie, "telha1") != NULL)
    {
        printf("Test error. String found: %s. It shouldn't be found.\n", "telha1");
    }

    if (trie_find(&trie, "águia") != NULL)
    {
        printf("Test error. String found: %s. It shoudn't be found.\n", "águia");
    }

    trie_clean(&trie);

    return 0;
}
