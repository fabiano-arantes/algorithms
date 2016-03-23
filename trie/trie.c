#include "trie.h"

#include <stdlib.h>
#include <string.h>

static trie_node* trie_put_(trie_node* n, char* s, size_t d, int value)
{
    if (n == NULL)
    {
        n = malloc(sizeof(trie_node));
        n->c = s[d];
        n->value = -1;
        n->left = NULL;
        n->mid = NULL;
        n->right = NULL;
    }

    if (n->c < s[d])
    {
        n->left = trie_put_(n->left, s, d, value);
    }
    else if (n->c > s[d])
    {
        n->right = trie_put_(n->right, s, d, value);
    }
    else if (strlen(s) > (d + 1))
    {
        n->mid = trie_put_(n->mid, s, d + 1, value);
    }
    else
    {
        n->value = value;
    }

    return n;
}

trie_node* trie_put(trie_node* root, char* s, int value)
{
    return trie_put_(root, s, 0, value);
}

void trie_clean(trie_node* n)
{
    if (n == NULL)
    {
        return;
    }

    trie_clean(n->left);
    trie_clean(n->mid);
    trie_clean(n->right);

    free(n);
}

int trie_find_(trie_node *n, char *s, size_t d)
{
    if (n == NULL)
    {
        return -1;
    }

    if (n->c < s[d])
    {
        return trie_find_(n->left, s, d);
    }
    else if (n->c > s[d])
    {
        return trie_find_(n->right, s, d);
    }
    else if (strlen(s) > (d + 1))
    {
        return trie_find_(n->mid, s, d + 1);
    }

    return n->value;
}

int trie_find(trie_node *n, char *s)
{
    if (n == NULL)
    {
        return -1;
    }

    return trie_find_(n, s, 0);
}
