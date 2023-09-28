#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DICT_MAX 1000
#define WORD 255

struct dictionary
{
    char word[WORD];
};

typedef struct dictionary dict;

void compress(dict* table)
{
    int i;
    char input[] = "BABAABAAA";
    char w[WORD] = {input[0],'\0'};
    int out[DICT_MAX];
    i = 1;
    int dict_num = 256;
    int g = 0;
    while (1)
    {
        char k[WORD] = {input[i],'\0'}; 
        int get = 0;
        int j =0;
        if ((k[0]=='\n') || (k[0]=='\0')){
            for (j = 0;j < DICT_MAX;j++){
                if (strcmp(w,table[j].word)==0)
                    out[g++] = j;
            }
            break;
        }
        
        char wk[WORD];
        strcpy(wk,w);
        strcat(wk,k);
        for (j = 0;j < DICT_MAX;j++){
            if (strcmp(wk,table[j].word)==0){
                out[g++] = j;
                strcpy(w,wk);
                strcat(wk,k);
                strcpy(table[dict_num].word,wk); 
                get = 1;
                break;
            }
        }
        if (get == 1){
            i++;
            continue;
        }
            for (int v = 0;v < DICT_MAX;v++){
                if (w[0]==v){
                    out[g++] = v;
                    break;
                    }
            }
            strcpy(table[dict_num].word,wk);
            dict_num++;
            strcpy(w,k);
            //break;                   
    i++;
    }
    for (int v = 0;v < DICT_MAX;v++){
        if (strcmp(w,table[v].word)==0){
            out[g] = v;
            break;
            }
    }
    for (int m = 0;m < g;m++)
    printf("%d,",out[m]);
}

int main()
{
    dict* table = (dict*)malloc(sizeof(dict) * DICT_MAX);
    for (int i = 0;i < 256;i++){
        char j[] = {(char)i,'\n'};
        strcpy(table[i].word, j);
    }
    printf("\n");
    compress(table);

    return 0;
}