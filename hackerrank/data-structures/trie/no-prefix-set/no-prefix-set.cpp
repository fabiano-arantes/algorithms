/*
Test: https://www.hackerrank.com/challenges/no-prefix-set

Author: Fabiano Arantes
Contact: https://www.linkedin.com/in/fabianoarantes/
*/

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

static const size_t ALPHABET_SIZE = 'j' - 'a' + 1;

typedef struct trieNode_
{
    struct trieNode_* alph[ALPHABET_SIZE];
    bool fullWord;
}TrieNode;

TrieNode* NewTrieNode()
{
    TrieNode* tn = new TrieNode();
    
    tn->fullWord = false;
    
    for(size_t ii = 0; ii < ALPHABET_SIZE; ++ii)
    {
        tn->alph[ii] = NULL;
    }
    
    return tn;
}

bool InsertNodeAndCheck(TrieNode *root, string word, size_t max, size_t c)
{
    if(root->alph[word[c] - 'a'] == NULL)
    {
        root->alph[word[c] - 'a'] = NewTrieNode();

        if(c == max)
        {
            root->alph[word[c] - 'a']->fullWord = true; 
        
            return true;            
        }
    }
    
    if((root->alph[word[c] - 'a']->fullWord) || (c == max))
    {
        return false;
    }
    
    return InsertNodeAndCheck(root->alph[word[c] - 'a'], word, max, c + 1);
}

int main() {
    size_t n;
    string word;
    TrieNode *root = NewTrieNode();
    
    cin >> n;
    
    while(n--)
    {
        cin >> word;
        
        if(InsertNodeAndCheck(root, word, word.size() - 1, 0) == false)
        {
            cout << "BAD SET" << endl << word << endl;

            return 1;
        }
    }

    cout << "GOOD SET" << endl;
   
    return 0;
}

