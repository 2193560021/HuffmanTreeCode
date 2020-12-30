#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Maxvalue 1000000
typedef struct
{
    unsigned char vaule;  //
    unsigned long weight; //
} TmpNode;
typedef struct
{
    unsigned char value;
    unsigned long weight;
    int parent, Lchild, Rchild;
    char *code;      //
} HufNode, *HufTree; //

void select(HufTree ht, unsigned int end, int *s1, int *s2) //
{
    unsigned int i;
    unsigned int m1, m2;
    m1 = m2 = Maxvalue;
    for (i = 0; i < end; i++)
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
void CrtHuffmanTree(HufTree ht, unsigned int char_kinds, unsigned int node_num) //
{
    int s1, s2;
    unsigned int i;

    for (i = char_kinds; i < node_num; i++)
    {
        select(ht, i, &s1, &s2); //

        ht[i].weight = ht[s1].weight + ht[s2].weight;
        ht[i].Lchild = s1;
        ht[i].Rchild = s2; //
        ht[s1].parent = i;
        ht[s2].parent = i; //
    }
}

void CrtHuffmanCode(HufTree ht, unsigned int char_kinds)
{
    unsigned int i;
    char *cd;
    int start;
    cd = (char *)malloc(256 * sizeof(char));
    cd[256 - 1] = '\0';
    for (i = 0; i < char_kinds; i++)
    {
        start = 256 - 1;
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
        ht[i].code = (char *)malloc((256 - start) * sizeof(char));
        strcpy(ht[i].code, &cd[start]);
    }
    free(cd);
}

int compress(char *ifname, char *ofname)
{
    unsigned int i, j;
    unsigned int char_kinds;    //
    unsigned char char_temp;    //
    unsigned long file_len = 0; //
    FILE *infile, *outfile;
    TmpNode node_temp;
    unsigned int node_num;
    HufTree ht;

    char code_buf[256] = "\0"; //
    unsigned int code_len;

    //

    TmpNode *tmp_nodes = (TmpNode *)malloc(256 * sizeof(TmpNode));

    for (i = 0; i < 256; i++) //初始化
    {
        tmp_nodes[i].weight = 0;
        tmp_nodes[i].vaule = i;
    }

    infile = fopen(ifname, "rb");
    if (infile == NULL)
        return -1;
    fread((char *)&char_temp, sizeof(unsigned char), 1, infile);
    while (!feof(infile))
    {
        ++tmp_nodes[char_temp].weight; //
        ++file_len;
        fread((char *)&char_temp, sizeof(unsigned char), 1, infile);
    }
    fclose(infile);

    //
    for (i = 0; i < 256 - 1; ++i)
        for (j = i + 1; j < 256; ++j)
            if (tmp_nodes[i].weight < tmp_nodes[j].weight)
            {
                node_temp = tmp_nodes[i];
                tmp_nodes[i] = tmp_nodes[j];
                tmp_nodes[j] = node_temp;
            }

    //
    for (i = 0; i < 256; ++i)
        if (tmp_nodes[i].weight == 0)
            break;
    char_kinds = i;

    if (char_kinds == 1) //
    {
        outfile = fopen(ofname, "wb");                                           //
        fwrite((char *)&char_kinds, sizeof(unsigned int), 1, outfile);           //
        fwrite((char *)&tmp_nodes[0].vaule, sizeof(unsigned char), 1, outfile);  //
        fwrite((char *)&tmp_nodes[0].weight, sizeof(unsigned long), 1, outfile); //

        free(tmp_nodes); //
        fclose(outfile);
    }

    else
    {
        node_num = 2 * char_kinds - 1;                      //
        ht = (HufNode *)malloc(node_num * sizeof(HufNode)); //

        //
        for (i = 0; i < char_kinds; i++)
        {

            ht[i].weight = tmp_nodes[i].weight;
            ht[i].value = tmp_nodes[i].vaule;
            ht[i].parent = 0;
        }

        free(tmp_nodes); //

        for (; i < node_num; i++) //
            ht[i].parent = 0;
        CrtHuffmanTree(ht, char_kinds, node_num); //
        CrtHuffmanCode(ht, char_kinds);           //

        outfile = fopen(ofname, "wb");

        fwrite((char *)&char_kinds, sizeof(unsigned int), 1, outfile); //
        for (i = 0; i < char_kinds; i++)
        {
            fwrite((char *)&ht[i].value, sizeof(unsigned char), 1, outfile);  //
            fwrite((char *)&ht[i].weight, sizeof(unsigned long), 1, outfile); //
        }

        fwrite((char *)&file_len, sizeof(unsigned long), 1, outfile); //

        infile = fopen(ifname, "rb"); //

        fread((char *)&char_temp, sizeof(unsigned char), 1, infile); //
        while (!feof(infile))
        {
            for (i = 0; i < char_kinds; i++)
            {
                if (char_temp == ht[i].value)
                    strcat(code_buf, ht[i].code);
            }

            while (strlen(code_buf) >= 8) //
            {
                char_temp = '\0'; //
                for (i = 0; i < 8; i++)
                {
                    char_temp <<= 1; //
                    if (code_buf[i] == '1')
                        char_temp |= 1; //
                }
                fwrite((char *)&char_temp, sizeof(unsigned char), 1, outfile);
                strcpy(code_buf, code_buf + 8);
            }
            fread((char *)&char_temp, sizeof(unsigned char), 1, infile);
        }

        code_len = strlen(code_buf);
        if (code_len > 0)
        {
            char_temp = '\0';
            for (i = 0; i < code_len; i++)
            {
                char_temp <<= 1;
                if (code_buf[i] == '1')
                    char_temp |= 1;
            }
            char_temp <<= 8 - code_len;
            fwrite((char *)&char_temp, sizeof(unsigned char), 1, outfile);
        }
        fclose(infile);
        fclose(outfile);

        for (i = 0; i < char_kinds; i++) //释放内存
            free(ht[i].code);
        free(ht);
    }
    return 0;
}
int extract(char *ifname, char *ofname)
{
    unsigned int i;
    unsigned long file_len;
    unsigned long write_len = 0;
    FILE *infile, *outfile;
    unsigned int char_kinds;
    unsigned int node_num;
    HufTree ht;
    unsigned char code_temp;
    unsigned int root;

    infile = fopen(ifname, "rb");

    if (infile == NULL)
        return -1;

    fread((char *)&char_kinds, sizeof(unsigned int), 1, infile);
    if (char_kinds == 1)
    {
        fread((char *)&code_temp, sizeof(unsigned char), 1, infile);
        fread((char *)&file_len, sizeof(unsigned long), 1, infile);
        outfile = fopen(ofname, "wb");
        while (file_len--)
        {
            fwrite((char *)&code_temp, sizeof(unsigned char), 1, outfile);
        }
        fclose(infile);
        fclose(outfile);
    }

    else
    {
        node_num = 2 * char_kinds - 1;
        ht = (HufNode *)malloc(node_num * sizeof(HufNode));

        for (i = 0; i < char_kinds; i++)
        {
            fread((char *)&ht[i].value, sizeof(unsigned char), 1, infile);  //
            fread((char *)&ht[i].weight, sizeof(unsigned long), 1, infile); //
            ht[i].parent = 0;
        }
    }

    for (; i < node_num; i++)
        ht[i].parent = 0;
    CrtHuffmanTree(ht, char_kinds, node_num); //

    fread((char *)&file_len, sizeof(unsigned long), 1, infile); //
    outfile = fopen(ofname, "wb");                              //
    root = node_num - 1;
    while (1)
    {
        fread((char *)&code_temp, sizeof(unsigned char), 1, infile);

        for (i = 0; i < 8; i++)
        {
            if (code_temp & 128)
                root = ht[root].Rchild;
            else
                root = ht[root].Lchild;

            if (root < char_kinds)
            {
                fwrite((char *)&ht[root].value, sizeof(unsigned char), 1, outfile);
                ++write_len;
                if (write_len == file_len)
                    break;
                root = node_num - 1; //
            }
            code_temp <<= 1;
        }
        if (write_len == file_len)
            break;
    }

    fclose(infile);
    fclose(outfile);

    free(ht);
    return 0;
}
int main()
{
    int opt, flag = 0;                   //
    char ifname[256] = {0}, ofname[256]; //
    printf("\t\t--------------------------------------------\n");
    printf("\t\t----Software Engineering-1901-Liu Yuyang----\n");
    printf("\t\t\t Huffman (un)Compress System\n");
    printf("\t\t1:Compress\n");
    printf("\t\t2:Uncompress\n");
    printf("\t\t3:Return Encode Program\n");
    printf("\t\t--------------------------------------------\n");
    printf("\t\t\t\tPlz choose:");
    scanf("%d", &opt);
    while (opt != 1 || opt != 2 || opt != 3)
    {
        switch (opt)
        {
        case 1:
            printf("\n\t\tplz input the file path to compress: ");
            fflush(stdin); //
            gets(ifname);
            printf("\n\t\tplz input the outfile name:");
            fflush(stdin);
            gets(ofname);
            printf("\n\t\t----------Compressing...----------\n");
            flag = compress(ifname, ofname); //
            break;
        case 2:
            printf("\n\t\tplz input the file path to uncompress: ");
            fflush(stdin); //
            gets(ifname);
            printf("\n\t\tplz input the outfile name: ");
            fflush(stdin);
            gets(ofname);
            printf("\n\t\t----------Uncompressing...----------\n");
            flag = extract(ifname, ofname); //
            break;
        case 3:
            printf("\n\t\t----------Returning...----------\n\n");
            system("main.exe");
            break;
        }
        break;
    }
    if (flag == -1)
        printf("\t\t----------Sorry, infile \"%s\" doesn't exist!----------\n", ifname); //
    else
    {
        printf("\t\t----------The compress-file \" %s \" has been created!----------\n\t\t", ofname);
        system("pause");
    }
    return 0;
}
