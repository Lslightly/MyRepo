#include <stdio.h>
#include "../Stack_Queue/Stack/cStack.h"

#define MAXSIZE 20
#define LT(a, b) (a < b)
#define EQ(a, b) (a == b)


typedef int KeyType;
typedef char * InfoType;
typedef struct
{
    KeyType key;        //  关键字
    InfoType otherinfo; //  其他信息
}RedType;

typedef struct
{
    RedType *r;   //  r[0]闲置
    int length;
}SqList;

typedef SqList HeapType;

int Partition(SqList & L, int low, int high)
{
    L.r[0] = L.r[low];
    int pivotkey = L.r[low].key;
    while (low < high)
    {
        while (low < high && L.r[high].key >= pivotkey) --high;
        L.r[low] = L.r[high];
        while (low < high && L.r[low].key <= pivotkey) ++low;
        L.r[high] = L.r[low];
    }
    L.r[low] = L.r[0];
    return low;
}

void QSort(SqList &L, int low, int high)
{
    if (low < high)
    {
        int pivotloc = Partition(L, low, high);
        QSort(L, low, pivotloc-1);
        QSort(L, pivotloc+1, high);
    }
}

void QuickSort(SqList &L)
{
    QSort(L, 1, L.length);
}

//  按照题目的要求，应该是在进行Partition之后先对左序列排序，暂存右序列，左序列全部排完之后再排右序列
//  所以，这里就在Partition之后先把右序列的上下界入栈，然后再将左序列的上下界入栈，下一次循环的时候又会将左序列的上下界出栈
//  进行排序，重复操作。就相当于对左序列的深度优先排序。如果上下界之差小于2，即待排数为3，则冒泡排序
void QSortNoRecursion(SqList &L, int low, int high)
{
    SqStack S;
    InitStack(S);
    Push(S, high);
    Push(S, low);
    while (!StackEmpty(S))
    {
        int part_low, part_high;
        Pop(S, part_low);
        Pop(S, part_high);
        if (part_high - part_low <= 2)
        {
            for (int i = part_low; i < part_high; i++)
            {
                for (int j = part_high; j > i; j--)
                {
                    if (LT(L.r[j].key ,L.r[j-1].key))
                    {
                        KeyType t = L.r[j].key;
                        L.r[j].key = L.r[j-1].key;
                        L.r[j-1].key = t;
                    }
                }
            }
        }
        else
        {
            int pivotloc = Partition(L, part_low, part_high);
            Push(S, part_high);
            Push(S, pivotloc+1);
            Push(S, pivotloc-1);
            Push(S, part_low);
        }
    }
}

void QuickSortNoRecursion(SqList &L)
{
    QSortNoRecursion(L, 1, L.length);
}

int CreateSqList(SqList &L)
{
    scanf("%d", &L.length);
    L.r = (RedType *)malloc(sizeof(*L.r) * (L.length+1));
    for (int i = 1; i <= L.length; i++)
    {
        scanf("%d", &L.r[i].key);
        L.r[i].otherinfo = nullptr;
    }
    return OK;
}

int PrintSqList(SqList L)
{
    for (int i = 1; i <= L.length; i++)
    {
        printf("%d ", L.r[i].key);
    }
    putchar('\n');
    return OK;
}

int ThreeColorSort(SqList &L)
{
    int num_red = 0, num_blue = 0;
    int i = 1;
    for (i = 1; i <= L.length; i++)
    {
        if (L.r[i].key == 0)
            ++num_red;
        else if (L.r[i].key == 2)
            ++num_blue;
    }
    for (i = 1; i <= num_red; i++)
        L.r[i].key = 0;
    for (; i <= L.length-num_blue; i++)
        L.r[i].key = 1;
    for (; i <= L.length; i++)
        L.r[i].key = 2;
    
    return OK;
}

void HeapAdjust(HeapType &H, int s, int m)
{
    RedType rc = H.r[s];
    int j = 2*s;
    for (; j <= m; j*=2)
    {
        if (j < m && LT(H.r[j].key, H.r[j+1].key)) ++j;
        if (!LT(rc.key, H.r[j].key)) break;
        H.r[s] = H.r[j];
        s = j;
    }
    H.r[s] = rc;
}

void HeapSort(HeapType &H)
{
    int i = H.length/2;
    for (; i > 0; --i)
        HeapAdjust(H, i, H.length);
    for (i = H.length; i > 1; --i)
    {
        RedType t = H.r[i];
        H.r[i] = H.r[1];
        H.r[1] = t;

        HeapAdjust(H, 1, i-1);
    }
}

void InsertHeap(HeapType &H, RedType k)
{
    H.length++;
    H.r = (RedType *)realloc(H.r, (H.length+1) * sizeof(*H.r));
    H.r[H.length] = k;

    int j = H.length;
    while (j != 1)
    {
        if (LT(H.r[j/2].key, H.r[j].key))
        {
            RedType t = H.r[j/2];
            H.r[j/2] = H.r[j];
            H.r[j] = t;
            j /= 2;
        }
        else break;
    }
}
