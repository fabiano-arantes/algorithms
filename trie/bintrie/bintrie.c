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


static int trie_symbol_table_(const bin_trie_node* const n, bin_trie_st st[UCHAR_MAX + 1], unsigned char bits, unsigned char bits_count)
{
    if (n == NULL)
    {
        MSG("n is NULL");
        return 0;
    }

    static unsigned long depth = 0;
    static unsigned long max_depth = 0;

    ++depth;

    if (depth > max_depth)
    {
        max_depth = depth;
    }

    if ((n->childs[0] == NULL) && (n->childs[1] == NULL))
    {
        unsigned char char_bits[17];

        int_to_bits(bits, bits_count, char_bits);

        MSG_ARG("creating symbol table node: c = '%c', bits = %s, bit_count = %d", n->c, char_bits, bits_count);

        st[(size_t)n->c].c = n->c;
        st[(size_t)n->c].bits = bits;
        st[(size_t)n->c].bits_count = bits_count;

        --depth;

        return 1;
    }

    trie_symbol_table_(n->childs[0], st, (bits << 1), bits_count + 1);

    trie_symbol_table_(n->childs[1], st, (bits << 1) | 1, bits_count + 1);

    --depth;

    return 1;
}

int trie_symbol_table(const bin_trie_node* const trie, bin_trie_st st[UCHAR_MAX])
{
    if (trie == NULL)
    {
        MSG("trie is NULL");
        return 0;
    }

    return trie_symbol_table_(trie, st, 0, 0);
}

void print_symbol_table(bin_trie_st st[UCHAR_MAX])
{
    size_t i = UCHAR_MAX;
    unsigned char bits[17];

    while (i--)
    {
        if (st[i].bits_count == 0)
        {
            continue;
        }

        int_to_bits(st[i].bits, st[i].bits_count, bits);

        MSG_ARG("c = '%c', bits_count = %d, bits = %s", st[i].c, st[i].bits_count, bits);
    }
}

bin_trie_node* read_bin_trie(mem* const buffer)
{
    unsigned char c;
    bit_t b;

    if (MEM_GET_NEXT_BIT(buffer, &b) && (b == BIT_1))
    {
        MEM_GET_NEXT_VALUE(buffer, &c);

        MSG_ARG("Bin trie node found: c = %c", c);

        return trie_new_node(c, 0, NULL, NULL);
    }

    bin_trie_node *left = read_bin_trie(buffer);
    bin_trie_node *right = read_bin_trie(buffer);

    return trie_new_node('\0', 0, left, right);
}
