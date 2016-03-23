
typedef struct trie_node_
{
    char c;
    int value;
    struct trie_node_ *left;
    struct trie_node_ *mid;
    struct trie_node_ *right;
}trie_node;

trie_node* trie_put(trie_node* root, char* s, int value);

void trie_clean(trie_node* root);

int trie_find(trie_node* n, char *s);
