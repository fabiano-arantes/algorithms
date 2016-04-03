#include <stdio.h>

#include "bintrie.h"

#include <heapsort.h>

typedef struct
{
    unsigned char c;
    size_t freq;
} test_data_input;

int main()
{
    test_data_input tdi[] = { {'a', 45}, {'b', 13}, {'c', 12}, {'d', 16}, {'e',
            9}, {'f', 5}};
    bin_trie_input ti[] = { {0b0, 1}, {0b101, 3}, {0b100, 3}, {0b111, 3}, {
            0b1101, 4}, {0b1100, 4}};

    size_t size_tdi = sizeof(tdi) / sizeof(*tdi);
    size_t size_ti = sizeof(ti) / sizeof(*ti);
    size_t i;
    bin_trie_node *node1, *node2;
    bin_trie_node *trie_root;
    bin_trie_st st[UCHAR_MAX + 1];
    sort_data heap;

    memset(&heap, 0, sizeof(sort_data));
    heap.cmp_func = asc_cmp_trie_freq;

    for (i = 0; i < size_tdi; ++i)
    {
        node1 = trie_new_node(tdi[i].c, tdi[i].freq, NULL, NULL);

        heap_put(&heap, node1);
    }

    while ((node1 = heap_remove_top(&heap)) != NULL)
    {
        if ((node2 = heap_remove_top(&heap)) == NULL)
        {
            break;
        }

        MSG_ARG("node1->freq = %zu, node2->freq = %zu", node1->freq, node2->freq);

        node1 = trie_merge(node1, node2);

        heap_put(&heap, node1);
    }

    trie_root = node1;

    trie_symbol_table(trie_root, st);

    //printf("c = '%c', bits = ", st->c);

    //for (i = st->bits_count; i--;)
    //{
    //    printf("%d", (st->bits >> i) & 1);
    //}

    //printf("\n");

    for (i = 0; i < size_ti; ++i)
    {
        if ((st[tdi[i].c].bits_count == 0) || (st[tdi[i].c].bits != ti[i].bits))
        {
            printf("Test error. Bits are different from expected value for %c\n", tdi[i].c);
        }
    }

    for (i = 0; i < size_ti; ++i)
    {
        if (trie_find(trie_root, &ti[i]) == 0)
        {
            printf("Test error. Char not found: %c\n", tdi[i].c);
        }
    }

    trie_clean(trie_root);

    return 0;
}
