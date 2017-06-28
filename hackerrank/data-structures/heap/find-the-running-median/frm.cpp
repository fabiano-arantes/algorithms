/*
Test: https://www.hackerrank.com/challenges/find-the-running-median

Author: Fabiano Arantes
Contact: https://www.linkedin.com/in/fabianoarantes/
*/

#include <iostream>
#include <iomanip>

typedef struct LinkedList_
{
    unsigned int v;
    struct LinkedList_* prev;
    struct LinkedList_* next;
}LinkedList;

LinkedList* newLinkedList(unsigned int v)
{
    LinkedList* ll = new LinkedList();
    
    ll->v = v;
    ll->prev = NULL;
    ll->next = NULL;

    return ll;
}

void InsertNodeAfter(LinkedList* node, LinkedList* newNode)
{
    //node -> newNode -> after
    //node <- newNode <- after
    
    newNode->next = node->next;
    newNode->prev = node;
    
    if(node->next)
    {
        node->next->prev = newNode;        
    }
   
    node->next = newNode;
}

using namespace std;
int main() {
    unsigned int nn, aa, size;
    double median;

    LinkedList* first = NULL;
    LinkedList* mid = NULL;
    LinkedList* last = NULL;
    LinkedList* pllNew = NULL;
    LinkedList* pll = NULL;

    cin >> nn;

    while(nn--)
    {
        cin >> aa;

        ++size;

        if(mid == NULL)
        {
            last = newLinkedList(aa);
            mid = last; 
            first = mid;
            
            median = aa;
        }
        else
        {
            pllNew = newLinkedList(aa);
            
            if(aa < first->v)
            {
                pllNew->next = first;
                first = pllNew;
            }
            else
            {
                pll = last;

                while(pll && (pll->v > pllNew->v))
                {
                    pll = pll->prev;
                }

                if(pll)
                    InsertNodeAfter(pll, pllNew);                                
            }

            if(size % 2)
            {
                if(aa > mid->v)
                {
                    mid = mid->next;
                }

                median = mid->v;
            }
            else
            {
                if(aa < mid->v)
                {
                    mid = mid->prev;
                }

                median = (double)(mid->v + mid->next->v) / 2.0 ;
            }
        }


	pll = first;

	while(pll)
	{
		cout << pll->v << " - ";
		pll = pll->next;
        }

        cout << std::setprecision(1) << std::fixed << median << endl;
    }

    return 0;
}

