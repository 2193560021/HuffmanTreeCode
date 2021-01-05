#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#define N 1000
// #define M 2 * N - 1
#define Maxvalue 1000000
int w[1000] = {0};
int n = 0, num;
char filepath[100];
typedef char *HuffmanCode[N + 1];
typedef struct
{
    int weight;
    int parent, Lchild, Rchild;
    char value;
    char *code;
} HTNode, HuffmanTree[2 * N];

void select(HuffmanTree ht, int end, int *s1, int *s2) //
{
    int i;
    int m1, m2;
    m1 = m2 = Maxvalue;
    for (i = 1; i <= end; i++)
    {
        if (ht[i].parent == 0)
        {
            if (ht[i].weight < m1)
            {
                m2 = m1;
                m1 = ht[i].weight;
                *s2 = *s1;
                *s1 = i;
            }
            else if (ht[i].weight < m2)
            {
                m2 = ht[i].weight;
                *s2 = i;
            }
        }
    }
}

int CrtHuffmanTree(HuffmanTree ht) //ctrhuffman
{
    n = 0;
    num = 0;
    int m, s1, s2;
    int i;
    FILE *fp;

    char ch;
    while (1)
    {
        printf("\t\tPlz input the file path:");
        scanf("%s", filepath);
        fp = fopen(filepath, "r");
        if (fp != NULL)
            break;
        printf("\t\t!!!Cann't find the file!!!\n");
    }

    while ((ch = fgetc(fp)) != EOF)
    {
        num++;
        w[ch]++;
    }
    fclose(fp);
    for (i = 1; i <= 1000; i++)
        if (w[i] != 0)
        {
            ++n;
            ht[n].weight = w[i];
            ht[n].value = i;
        }
    --n;
    m = 2 * n - 1;
    for (i = 1; i <= n; i++) //初始化前N个元素
    {
        ht[i].parent = ht[i].Lchild = ht[i].Rchild = 0;
    }
    for (i = n + 1; i <= m; i++) //初始化后N+1个元素
    {
        ht[i].weight = ht[i].parent = ht[i].Lchild = ht[i].Rchild = 0;
    }
    for (i = n + 1; i <= m; i++)
    {
        select(ht, i - 1, &s1, &s2); //选权值最小的两个结点

        ht[i].weight = ht[s1].weight + ht[s2].weight; //建新结点，赋权值
        ht[i].Lchild = s1;
        ht[i].Rchild = s2; //赋新结点左右孩子指针
        ht[s1].parent = i;
        ht[s2].parent = i; //改s1,s2的双亲指针
    }
}
void CrtHuffmanCode1(HuffmanTree ht, HuffmanCode hc)
{
    printf("\n\t\tThere are %d characters,and %d different characters\n", num, n);
    printf("\t\n");
    int i;
    char *cd;
    int start;
    cd = (char *)malloc(n * sizeof(char));
    cd[n - 1] = '\0';
    printf("\t\t\tvaule times huffmancode\n");
    for (i = 1; i <= n; i++)
    {
        start = n - 1;
        int c = i;
        int p = ht[i].parent;
        while (p != 0)
        {
            --start;
            if (ht[p].Lchild == c)
                cd[start] = '0';
            else
                cd[start] = '1';
            c = p;
            p = ht[p].parent;
        }
        hc[i] = (char *)malloc((n - start) * sizeof(char));
        strcpy(hc[i], &cd[start]);

        printf("\t\t\t  %c     %d     %s\n", ht[i].value, ht[i].weight, hc[i]); //
    }
    free(cd);
}
int Encoding(HuffmanTree ht) //
{
    printf("\n\t\tThe HuffmanCode:\n");
    FILE *fp, *fp1;
    int j = 0;
    int i;
    char *cd;
    int start;
    cd = (char *)malloc(n * sizeof(char));
    cd[n - 1] = '\0';
    printf("\t\t");
    char ch;
    char filename[100];
    fp = fopen(filepath, "r");
    fp1 = fopen("HuffmanByLYY.code", "w+");

    while ((ch = fgetc(fp)) != EOF)
    {
        for (i = 1; i <= n; i++)
        {
            if (ht[i].value == ch)
            {
                start = n - 1;
                int c = i;
                int p = ht[i].parent;
                while (p != 0)
                {
                    --start;
                    if (ht[p].Lchild == c)
                        cd[start] = '0';
                    else
                        cd[start] = '1';
                    c = p;
                    p = ht[p].parent;
                }
                char *ss;
                ss = (char *)malloc((n - start) * sizeof(char));
                strcpy(ss, &cd[start]);
                fputs(ss, fp1);

                printf("%s", ss);
            }
        }
        j++;
    }
    fclose(fp);
    fclose(fp1);
    free(cd);
    printf("\n");
}
int Decoding(HuffmanTree ht) //
{
    int i = 0;
    int j = 2 * n - 1;
    FILE *fp, *fp2;
    char ch;
    char filename[100];
    fp2 = fopen("DecodeByLYY.decode", "w+");
    while (1)
    {
        printf("\t\tPlz input the file path to be decoded:");
        scanf("%s", filename);
        fp = fopen(filename, "r");
        if (fp != NULL)
            break;
        printf("\t\t!!!Cann't find the file!!!\n");
    }

    char str[1000];
    printf("\n\t\tThe decoding result is:\n\t\t");
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ht[j].Lchild && ht[j].Rchild)
        {
            if (ch == '0')
                j = ht[j].Lchild;
            else
                j = ht[j].Rchild;
        }
        if (ht[j].Lchild == 0 && ht[j].Rchild == 0)
        {
            fprintf(fp2, "%c", ht[j].value);
            printf("%c", ht[j].value);
            j = 2 * n - 1;
            str[i++] = ht[j].value;
        }
        i++;
    }
    fclose(fp2);
    fclose(fp);
}

int main()
{
    HuffmanTree ht;
    HuffmanCode hc;
    int n, flag;
    while (1)
    {
        do
        {
            printf("\t\t----------Software Engineering-1901-Liu Yuyang----------\n");
            printf("\t\t------------------------!Welcome!-----------------------\n");
            printf("\t\t1:Encode\n");
            printf("\t\t2:Decode(WARNING:Don't choose this option at first,because there are no Huffmantree!)\n");
            printf("\t\t3:Compress and Uncompress(WARNING:This option will load another program!)\n");
            printf("\t\t4:Quit\n");
            printf("\t\t--------------------------------------------------------\n");
            printf("\t\t\t\tPlz choose:");
            break;
        } while (n);
        scanf("%d", &n);
        switch (n)
        {
        case 1: //文件编码
            CrtHuffmanTree(ht);
            CrtHuffmanCode1(ht, hc);
            printf("\t\t--------------------!!!Coding Success!!!------------------\n");
            printf("\t\tPress any key to show the decode:");
            getch();
            Encoding(ht);
            printf("\n\t\t---------The file \" HuffmanByLYY.code \" has been created --------\n\n\t\t");
            system("pause");
            system("cls");
            break;
        case 2: //文件译码
            Decoding(ht);
            printf("\n\t\t----------------------------!!!Decoding Success!!!--------------------------\n");
            printf("\n\t\t-----------------The file \" DecodeByLYY.decode \" has been created -------------\n\n\t\t");
            system("pause");
            system("cls");
            break;
        case 3:
            printf("\t\tLoading Compress and Uncompress program......\n\n");
            system("main_compress.exe");
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("\n\t\tEnter again!\n");
        }
    }
}