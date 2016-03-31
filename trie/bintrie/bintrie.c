#include "bintrie.h"

#include <debug.h>

#include <stdlib.h>
#include <string.h>

bin_trie_node* trie_merge(bin_trie_node* const left, bin_trie_node* const right)
{
    if (left == NULL)
    {
        MSG("left is NULL");
        return NULL;
    }

    if (right == NULL)
    {
        MSG("right is NULL");
        return NULL;
    }

    return trie_new_node('\0', left->freq + right->freq, left, right);
}

void trie_clean(bin_trie_node* root)
{
    if (root == NULL)
    {
        MSG("root is NULL");
        return;
    }

    trie_clean(root->childs[0]);
    trie_clean(root->childs[1]);

    free(root);
}

char trie_find(const bin_trie_node* const trie, bin_trie_input* const i)
{
    if (trie == NULL)
    {
        MSG("trie is NULL");
        return 0;
    }

    if (i == NULL)
    {
        MSG("input is NULL");
        return 0;
    }

    int bits = (i->bits >> --i->shift) & 1;

    if (i->shift == 0)
    {
        return trie->childs[bits]->c;
    }
    else
    {
        return trie_find(trie->childs[bits], i);
    }
}


static bin_trie_st* trie_symbol_table_(const bin_trie_node* const n, unsigned char bits, unsigned char bits_count)
{
    if (n == NULL)
    {
        MSG("n is NULL");
        return NULL;
    }

    bin_trie_st *node, *next;

    if ((n->childs[0] == NULL) && (n->childs[1] == NULL))
    {
        MSG_ARG("creating symbol table node: c = %c, bits = %d, bit_count = %d", n->c, bits, bits_count);
        node = malloc(sizeof(bin_trie_st));
        node->c = n->c;
        node->bits = bits;
        node->bits_count = bits_count;
        node->next = NULL;

        return node;
    }

    node = trie_symbol_table_(n->childs[0], (bits << 1), bits_count + 1);

    next = node;

    while (next->next != NULL)
    {
        next = next->next;
    }

    next->next = trie_symbol_table_(n->childs[1], (bits << 1) | 1, bits_count + 1);

    return node;
}

bin_trie_st* trie_symbol_table(const bin_trie_node* const trie)
{
    if (trie == NULL)
    {
        MSG("trie is NULL");
        return NULL;
    }

    return trie_symbol_table_(trie, 0, 0);
}
