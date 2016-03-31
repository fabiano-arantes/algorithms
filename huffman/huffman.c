#include <bintrie.h>
#include <heapsort.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    char buffer[UCHAR_MAX + 1] = {0};
    char *buf;
    size_t count[UCHAR_MAX + 1];
    size_t c;
    size_t buffer_size = sizeof(buffer)/sizeof(*buffer);
    //bin_trie_node btrie;
    sort_data heap;

    heap.cmp_func = asc_cmp_trie_freq;

    if (argc < 2)
    {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    for (c = 0; c < sizeof(count)/sizeof(*count); ++c)
    {
        count[c] = 0;
    }

    fp = fopen(argv[1], "rb");
    if (fp == NULL)
    {
        printf("Fail to open file '%s'", argv[1]);
    }

    size_t t = 0;
    while ((c = fread(buffer, 1, buffer_size, fp)) > 0)
    {
        ++t;
        printf("%zu-", t);

        fflush(stdout);

        buf = buffer;
        do
        {
            ++count[(size_t)*buf];
            ++buf;
        }while(--c);
    }

    fclose(fp);

    bin_trie_node *node1, *node2;

    for (c = 0; c < buffer_size; ++c)
    {
        if (count[c] > 0)
        {
            MSG_ARG("[%zu] %c = %zu\n", c, (char)c, count[c]);

            node1 = trie_new_node(c, count[(size_t)c], NULL, NULL);

            heap_put(&heap, node1);
        }
    }

    while ((node1 = heap_remove_top(&heap)) != NULL)
    {
        if ((node2 = heap_remove_top(&heap)) == NULL)
        {
            break;
        }

        node1 = trie_merge(node1, node2);

        heap_put(&heap, node1);
    }

    return 0;
}
