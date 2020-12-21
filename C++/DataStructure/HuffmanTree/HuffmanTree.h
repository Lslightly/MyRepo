#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 256
typedef struct HTNode
{
    unsigned int weight;
    unsigned int parent, lchild, rchild;
}HTNode, * HuffmanTree;

typedef char ** HuffmanCode;

void Select(HuffmanTree & HT, int i, int & s1, int & s2);

void PrintHuffmanTree(HuffmanTree T, int n, char chtable[]);

void HuffmanDecoding(HuffmanTree & HT, HuffmanCode &HC, int &n);

//  HC: Huffman Code
void HuffmanCoding(HuffmanTree & HT, HuffmanCode & HC, unsigned int * w, int n)
{
    int m = 0, i = 0;
    int s1, s2;
    HuffmanTree p = nullptr;

    if (n <= 1) return ;
    m = 2*n-1;
    HT = (HuffmanTree)malloc(sizeof(HTNode)*(m+1));
    
    for (p = HT+1, i = 1; i <= n; ++i, ++p, ++w) *p = {*w, 0, 0, 0};

    for (; i <= m; ++i, ++p) *p = {0, 0, 0, 0};

    for (i = n+1; i <= m; i++)
    {
        Select(HT, i-1, s1, s2);
        HT[s1].parent = i;  HT[s2].parent = i;
        HT[i].lchild = s1; HT[i].rchild = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
    }

    HC = (HuffmanCode)malloc(sizeof(*HC)*(n+1));
    char * cd = (char *)malloc(n * sizeof(char));

    cd[n-1] = '\0';
    for (i = 1; i<=n; i++)
    {
        int start = n-1, f = HT[i].parent, c = i;

        for (; f != 0; c = f, f = HT[c].parent)
        {
            if (HT[f].lchild == c) cd[--start] = '0';
            else cd[--start] = '1';
        }

        HC[i] = (char *)malloc((n-start) * sizeof(char));
        strcpy(HC[i], cd+start);
    }
    free(cd);
}

void Select(HuffmanTree & HT, int i, int & s1, int &s2)
{
    s1 = 0, s2 = 0;
    int min1 = __INT32_MAX__, min2 = __INT32_MAX__;
    for (int j = 1; j <= i; j++)
    {
        if (HT[j].parent == 0 && HT[j].weight < min1)
        {
            s1 = j;
            min1 = HT[j].weight;
        }
    }
    for (int j = 1; j <= i; j++)
    {
        if (HT[j].parent == 0 && HT[j].weight >= min1 && HT[j].weight < min2 && j != s1)
        {
            s2 = j;
            min2 = HT[j].weight;
        }
    }
    if (s1 > s2)
    {
        int t = s1;
        s1 = s2;
        s2 = t;
    }
}

void PrintHuffmanTree(HuffmanTree T, int n, char chtable[MAX_CHARS])
{
    int i = 1;
    printf("%d\n", n);
    int m = 2*n-1;
    for (i = 1; i <= n; i++)
    {
        printf("%c %u %u %u %u\n", chtable[i], T[i].weight, T[i].parent,T[i].lchild, T[i].rchild);
    }
    for (; i<=m; i++)
    {
        printf("%u %u %u %u\n", T[i].weight, T[i].parent,T[i].lchild, T[i].rchild);
    }

}
