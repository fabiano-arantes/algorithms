#include <stdio.h>

#include "trie.h"

int main()
{
    trie_node *trie = NULL;

    trie = trie_put(trie, "teste", 1);
    trie_put(trie, "tesão", 2);
    trie_put(trie, "testa", 3);
    trie_put(trie, "telha", 4);
    trie_put(trie, "água", 5);
    trie_put(trie, "agulha", 10);
    trie_put(trie, "agora", 10);
    trie_put(trie, "antena", 10);

    if (trie_find(trie, "telha") == -1)
    {
        printf("String not found: %s\n", "telha");
    }

    if (trie_find(trie, "telha1") == -1)
    {
        printf("String not found: %s\n", "telha1");
    }

    if (trie_find(trie, "águia") == -1)
    {
        printf("String not found: %s\n", "águia");
    }

    trie_clean(trie);

    return 0;
}
