#ifndef TRIE_3W_H
#define TRIE_3W_H

#include <stddef.h>

typedef struct trie_3w_node_
{
    char c;
    void* value;
    struct trie_3w_node_ *left;
    struct trie_3w_node_ *mid;
    struct trie_3w_node_ *right;
}trie_3w_node;

typedef struct
{
    size_t type_size;
    trie_3w_node *root;
}trie_3w;

trie_3w_node* trie_put(trie_3w* const root, const char* const s, const void* const value);

void trie_clean(trie_3w* const root);

void* trie_find(const trie_3w* const n, const char* const s);

#endif //TRIE_H
