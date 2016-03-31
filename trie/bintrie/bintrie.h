#ifndef BIN_TRIE_H
#define BIN_TRIE_H

#include <debug.h>

#include <stdlib.h>

typedef struct bin_trie_node_
{
    char c;
    size_t freq;
    struct bin_trie_node_ *childs[2];
} bin_trie_node;

typedef struct
{
    unsigned char bits;
    int shift;
} bin_trie_input;

typedef struct bin_trie_st_
{
    char c;
    unsigned char bits;
    unsigned char bits_count;
    struct bin_trie_st_* next;
} bin_trie_st;

static inline int asc_cmp_trie_freq(const void* const v1, const void* const v2)
{
    bin_trie_node *node1 = (bin_trie_node *) v1;
    bin_trie_node *node2 = (bin_trie_node *) v2;

    if (node1->freq < node2->freq)
    {
        return 1;
    }

    return 0;
}

static inline bin_trie_node* trie_new_node(char c, size_t freq, bin_trie_node* const left, bin_trie_node* const right)
{
    bin_trie_node* node;

    node = malloc(sizeof(bin_trie_node));
    if (node == NULL)
    {
        MSG("Error on allocate new node");
    }
    else
    {
        node->c = c;
        node->freq = freq;
        node->childs[0] = left;
        node->childs[1] = right;
    }

    return node;
}

bin_trie_node* trie_merge(bin_trie_node* const left, bin_trie_node* const right);

void trie_clean(bin_trie_node* const root);

char trie_find(const bin_trie_node* const n, bin_trie_input* const i);

bin_trie_st* trie_symbol_table(const bin_trie_node* const n);


#endif //BIN_TRIE_H
