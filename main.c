#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PL0.c"
#include "NodeList.h"
#include "Stack.h"






int main()
{
    //infile = fopen("sourcecode.txt", "r");
    /*getch();
    int i = 0;
    while(i<150){
    getsym();
    printf(id);
    printf("%d",sym);
    printf("%d",isAnnotation);
    printf("%s","\n");
    i++;
    }
    getsym();
    printf(id);
    printf("%d",sym);*/


    FILE* BNFfile = fopen("BNF.txt","r");
    //getItems(BNFfile);
    /*int k;
    for(k = 0;k<allItemLength;k++){
        printf(allItems[k].before);
        printf("~");
        printf(allItems[k].after);
        printf("%d\n",allItems[k].dot_location);
    }*/
    //scan_first_include();
   /* int i;
    for(i = 0;i<VNLength;i++){
        Iterator it = Begin(first_include[i]);
        printf("father of %c\n",i+65);
        while(it!=first_include[i]->head){
            printf("%c\n",*(char*)it->data);
            Next(&it);
        }
    }*/
   // create_first();
    /*FILE* first_file = fopen("first.txt","w");
    int i ;
    for(i = 0;i<VNLength;i++){
        Iterator it = Begin(FIRST[i]);
        fprintf(first_file,"%c: ",i+65);
        while(it!=FIRST[i]->head){
            fprintf(first_file,"%c,",*(char*)it->data);
            Next(&it);
        }
        fprintf(first_file,"%c",'\n');
    }*/

   // scan_follow_include();
    /*int j;
    for(j = 0;j<VNLength;j++){
        Iterator it = Begin(follow_include[j]);
        printf("father of %c\n",j+65);
        while(it!=follow_include[j]->head){
            printf("%c\n",*(char*)it->data);
            Next(&it);
        }
    }*/
    //create_follow();
    /*FILE* follow_file = fopen("follow.txt","w");
    int j ;
    for(j = 0;j<VNLength;j++){
        Iterator it = Begin(FOLLOW[j]);
        fprintf(follow_file,"%c: ",j+65);
        while(it!=FOLLOW[j]->head){
            fprintf(follow_file,"%c,",*(char*)it->data);
            Next(&it);
        }
        fprintf(follow_file,"%c",'\n');
    }*/
    /*printf("first !\n");
    int i;
    for(i = 0;i<VNLength;i++){
        printf("first of %c :\n",i+65);
        Iterator it = Begin(FIRST[i]);
        while(it!=FIRST[i]->head){
            printf("%c",*(char*)it->data);
            Next(&it);
        }
        printf("\n");
    }*/
    /*int i;
    for(i = 0;i<VNLength;i++){
        printf("follow of %c :\n",i+65);
        Iterator it = Begin(FOLLOW[i]);
        while(it!=FOLLOW[i]->head){
            printf("%c",*(char*)it->data);
            Next(&it);
        }
        printf("\n");
    }*/
    //Items items;
    //InitList(&(items.item_list),sizeof(Item));
    //Append(items.item_list,&allItems[0],NULL);
   // printf(allItems[0].before);
    /*getClosure(&items);
    Iterator it = Begin(items.item_list);
    while(it!=items.item_list->head){
        printf((char*)((Item*)it->data)->before);
        printf("~");
            printf((char*)((Item*)it->data)->after);
            printf("%d",((Item*)it->data)->dot_location);

            printf("\n");
            Next(&it);
    }*/

    //getTable();
    /*FILE* xiangmujizu_file = fopen("xiangmujizu.txt","w");
    Iterator it = Begin(C);
    int k = 0;
    while(it!=C->head){
        Iterator it1 = Begin(((Items*)it->data)->item_list);
        fprintf(xiangmujizu_file,"状态%d:\n",k);
        while(it1!=((Items*)it->data)->item_list->head){
            fprintf(xiangmujizu_file,"%c~",((Item*)it1->data)->before[0]);
            fprintf(xiangmujizu_file,((Item*)it1->data)->after);
            fprintf(xiangmujizu_file,"%d",((Item*)it1->data)->dot_location);
            fprintf(xiangmujizu_file,"\n");
            Next(&it1);
        }
        k++;
        Next(&it);
    }*/
    /*Iterator it = Begin(table);
    int j = 0;
    while(it!=table->head){
        int i ;
        printf("״̬%d\n",j);
        for(i = 0;i<VTLength+VNLength;i++){

            printf("%c%d  ",((Action*)it->data)[i]);
        }
        printf("\n");
        Next(&it);
        j++;
    }*/
    /*FILE* file = fopen("SLRTable.txt","r");
    char* line = (char*)malloc(sizeof(char)*6);
    while(fgets(line,6,file)){
        if(line[0] == '\n')
            printf("\n");
        else
            printf("%d ",line[2]-48);
    }*/

    getProductions(BNFfile);
    grammarAnalyze();
    Iterator it = Begin(aim_statements);
    while(it!=aim_statements->head){
        printf("%d ",((Statement*)it->data)->index);
        printf(mnemonic[((Statement*)it->data)->op_code]);
        printf(" ");
        printf("%d ",((Statement*)it->data)->l);
        printf("%d",((Statement*)it->data)->a);
        printf("\n");
        Next(&it);
    }
    return 0;
}
