#include "trie3w.h"

#include <debug.h>

#include <stdlib.h>
#include <string.h>

static trie_3w_node* trie_put_(trie_3w_node* n, const char* const s, size_t d, const void* const value, size_t type_size)
{
    if (n == NULL)
    {
        n = malloc(sizeof(trie_3w_node));
        n->c = s[d];
        n->value = NULL;
        n->left = NULL;
        n->mid = NULL;
        n->right = NULL;
    }

    if (n->c < s[d])
    {
        n->left = trie_put_(n->left, s, d, value, type_size);
    }
    else if (n->c > s[d])
    {
        n->right = trie_put_(n->right, s, d, value, type_size);
    }
    else if (strlen(s) > (d + 1))
    {
        n->mid = trie_put_(n->mid, s, d + 1, value, type_size);
    }
    else
    {
        n->value = malloc(type_size);
        memcpy(n->value, value, type_size);
    }

    return n;
}

trie_3w_node* trie_put(trie_3w* const trie, const char* const s, const void* const value)
{
    if (trie == NULL)
    {
        MSG("trie is NULL");
        return NULL;
    }

    if (s == NULL)
    {
        MSG("string is NULL");
        return NULL;
    }

    if (strlen(s) == 0)
    {
        MSG("string is empty");
        return NULL;
    }

    if (value == NULL)
    {
        MSG("value is NULL");
        return NULL;
    }

    return (trie->root = trie_put_(trie->root, s, 0, value, trie->type_size));
}

static void trie_clean_(trie_3w_node* n)
{
    if (n == NULL)
    {
        return;
    }

    trie_clean_(n->left);
    trie_clean_(n->mid);
    trie_clean_(n->right);

    free(n);
}

void trie_clean(trie_3w* const trie)
{
    if (trie == NULL)
    {
        MSG("trie is NULL");
        return;
    }

    trie_clean_(trie->root);
}

void* trie_find_(const trie_3w_node* const n, const char* const s, size_t d)
{
    if (n == NULL)
    {
        return NULL;
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

void* trie_find(const trie_3w* const trie, const char* const s)
{
    if (trie == NULL)
    {
        return NULL;
    }

    return trie_find_(trie->root, s, 0);
}
