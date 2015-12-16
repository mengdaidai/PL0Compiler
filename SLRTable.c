/*
*  Created by DaiXinjie @2015-12-16
*  产生预测分析表相关函数的定义
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "SLRTable.h"
#define true 1
#define false 0






//检查Item是否在Items中
int in_items(Items* itemset,Item* item){

    Iterator i = Begin(itemset->item_list);
    while(i!=itemset->item_list->head){
        if((Item*)i->data == item){
            return 1;
        }
        Next(&i);
    }
    return 0;
}

//根据产生式的文件将产生式和项目保存在数组中，以备后面使用
void getItems(FILE* file){
    char* line = (char*)malloc(MAXPROSIZE);
    allItemLength = 0;
    allProductionLength = 0;

    while(fgets(line,MAXPROSIZE,file)){
        int len = strlen(line);
        int i ;
        int j = 0;
        int isBefore = true;
        char* before = (char*)malloc(MAXPROSIZE);
        char* after = (char*)malloc(MAXPROSIZE);

        for(i = 0;i<len-1;i++){

            if(line[i] == '~'){
                isBefore = false;
                before[j] = '\0';
                j = 0;
            }
            else if(isBefore){
                before[j] = line[i];
                j++;
            }else{
                after[j] = line[i];
                j++;
            }

        }
        after[j] = '\0';
        allProduction[allProductionLength].before = before;
        allProduction[allProductionLength].after = after;
        allProduction[allProductionLength].start = allItemLength;
        allProduction[allProductionLength].afterLength = j;
        for(i = 0;i<=j;i++){
            allItems[allItemLength+i].before = before;
            allItems[allItemLength+i].after= after;
            allItems[allItemLength+i].dot_location = i;
            allItems[allItemLength+i].production_index = allProductionLength;
            allItems[allItemLength+i].afterLength = j;
        }
        allItemLength += j+1;
        allProductionLength++;


    }

}
//根据文件获取所有产生式放在数组中
void getProductions(FILE* file){
    char* line = (char*)malloc(MAXPROSIZE);
    allProductionLength = 0;

    while(fgets(line,MAXPROSIZE,file)){
        int len = strlen(line);
        int i ;
        int j = 0;
        int isBefore = true;
        char* before = (char*)malloc(MAXPROSIZE);
        char* after = (char*)malloc(MAXPROSIZE);
        //printf(line);

        for(i = 0;i<len-1;i++){

            if(line[i] == '~'){
                isBefore = false;
                before[j] = '\0';
                j = 0;
            }
            else if(isBefore){
                before[j] = line[i];
                j++;
            }else{
                after[j] = line[i];
                j++;
            }

        }
        after[j] = '\0';
        allProduction[allProductionLength].before = before;
        allProduction[allProductionLength].after = after;
        allProduction[allProductionLength].start = allItemLength;
        allProduction[allProductionLength].afterLength = j;
        allProductionLength++;
    }
}
//获得文法符号对应的位置
int index_of_char(char c){
    if(c>='a'&&c<='s'){
        return c-97;
    }else if(c>='A'&&c<='Z'){
        return c-65+VTLength;
    }else if(c>='('&&c<='.'){
        return c-40+19;
    }else if(c>=';'&&c<='>'){
        return c-59+26;
    }else if(c == '/'){
        return 30;
    }else if(c == '#'){
        return 31;
    }else if(c == '$'){
        return 32;
    }
}
//判断是不是终结符
int is_VT(char c){
    if(index_of_char(c)<VTLength)
        return 1;
    return 0;
}
//判断是不是非终结符
int is_VN(char c){
    if(index_of_char(c)>=VTLength)
        return 1;
    return 0;
}
//获取项目中点后的第一个文法符号
char get_after_dot(Item* item){
    char c = item->after[item->dot_location];
    return c;
}
//获取项目集闭包
void getItemClosure(Item* item,Items* itemset){
    char c = get_after_dot(item);
    if(is_VT(c) == 1){
        return;
    }
    else if(is_VT(c) == 0){
        int i;
        for(i = 0;i<allProductionLength;i++){
            if(allProduction[i].before[0] == c){
                if(in_items(itemset,&allItems[allProduction[i].start]) == 0){
                    Append(itemset->item_list,&allItems[allProduction[i].start],NULL);
                    itemset->length++;
                    getItemClosure(&allItems[allProduction[i].start],itemset);
                }

            }
        }

    }
    return ;
}
//获取项目集闭包
void getClosure(Items* itemset){
    List itemset_list;
    InitList(&itemset_list,sizeof(Item));
    Iterator temp_it = Begin(itemset->item_list);
    itemset->length = 0;
    itemset->main_length = 0;
    while(temp_it!=itemset->item_list->head){
        Append(itemset_list,temp_it->data,NULL);
        itemset->length++;
        itemset->main_length++;
        Next(&temp_it);
    }
    Iterator it = Begin(itemset_list);
    int i = 0;
    while(it!=itemset_list->head){

       getItemClosure((Item*)it->data,itemset);
       i++;
       Next(&it);
    }

    itemset->main_length = i;
}
//判断产生式右部是否包含某个文法符号
int in_after(char c,Production* production){
    int i;
    for(i = 0;i<production->afterLength;i++){
        if(c == production->after[i])
            return i;
    }
    return -1;
}//
//判断是否在first或follow集中
int in_first_or_follow(List list,char goal){
    Iterator it = Begin(list);
    int i = 0;
    while(it!=list->head){
        i++;
        if(goal == *(char*)it->data){
            return 1;
        }
        Next(&it);
    }
    return 0;
}
//获取first集的父子包含关系
void scan_first_include(){
    int j;
    for(j = 0;j<VNLength;j++){

        InitList(&first_include[j],sizeof(char));
    }
    int i ;
    for(i = 0;i<allProductionLength;i++){
        if(is_VN(allProduction[i].after[0]) == 1){
            if(allProduction[i].after[0]!=allProduction[i].before[0]&&in_first_or_follow(first_include[index_of_char(allProduction[i].after[0])-VTLength],allProduction[i].before[0]) == 0)
            Append(first_include[index_of_char(allProduction[i].after[0])-VTLength],&allProduction[i].before[0],NULL);
        }
    }
}
//获取follow集的父子包含关系
void scan_follow_include(){
    int j;
    for(j = 0;j<VNLength;j++){
        InitList(&follow_include[j],sizeof(char));
    }
    int i;
    for(i = 0;i<allProductionLength;i++){
         if(is_VN(allProduction[i].after[allProduction[i].afterLength-1]) == 1&&in_first_or_follow(follow_include[index_of_char(allProduction[i].before[0])-VTLength],allProduction[i].after[allProduction[i].afterLength-1]) == 0){
            Append(follow_include[index_of_char(allProduction[i].before[0])-VTLength],&allProduction[i].after[allProduction[i].afterLength-1],NULL);
        }
    }
}

void append_first_father(char c,char* goal){
    Iterator it = Begin(first_include[index_of_char(c)-VTLength]);
    while(it!=first_include[index_of_char(c)-VTLength]->head){
        if(in_first_or_follow(FIRST[index_of_char(*(char*)it->data)-VTLength],*goal) == 0){
            Append(FIRST[index_of_char(*(char*)it->data)-VTLength],goal,NULL);
            append_first_father(*(char*)it->data,goal);
        }
        Next(&it);
    }
}

void append_follow_father(char c,char* goal){
    Iterator it = Begin(follow_include[index_of_char(c)-VTLength]);
    while(it!=follow_include[index_of_char(c)-VTLength]->head){
        if(in_first_or_follow(FOLLOW[index_of_char(*(char*)it->data)-VTLength],*goal) == 0){
            Append(FOLLOW[index_of_char(*(char*)it->data)-VTLength],goal,NULL);
            append_follow_father(*(char*)it->data,goal);
        }
        Next(&it);
    }
}
//求每个文法符号的first集
void create_first(){

    int j;
    for(j = 0;j<VNLength;j++){
        InitList(&FIRST[j],sizeof(char));
    }

    int i;
    for(i = 0;i<allProductionLength;i++){

         if(is_VT(allProduction[i].after[0]) == 1){
            if(in_first_or_follow(FIRST[index_of_char(allProduction[i].before[0])-VTLength],allProduction[i].after[0]) == 0){
                Append(FIRST[index_of_char(allProduction[i].before[0])-VTLength],&allProduction[i].after[0],NULL);
                append_first_father(allProduction[i].before[0],&allProduction[i].after[0]);
            }

         }
    }
}
//求每个文法符号的follow集
void create_follow(){
     int j;
    for(j = 0;j<VNLength;j++){
        InitList(&FOLLOW[j],sizeof(char));
    }
    char temp = '$';
    Append(FOLLOW[0],&temp,NULL);
    int i;
    for(i = 0;i<allProductionLength;i++){
        int k;
        for(k = 0;k<allProduction[i].afterLength-1;k++){
            if(is_VT(allProduction[i].after[k+1]) == 1&&is_VN(allProduction[i].after[k]) == 1){
                if(in_first_or_follow(FOLLOW[index_of_char(allProduction[i].after[k])-VTLength],allProduction[i].after[k+1]) == 0){
                    Append(FOLLOW[index_of_char(allProduction[i].after[k])-VTLength],&allProduction[i].after[k+1],NULL);
                    append_follow_father(allProduction[i].after[k],&allProduction[i].after[k+1]);
                }

            }else if(is_VN(allProduction[i].after[k+1]) == 1&&is_VN(allProduction[i].after[k]) == 1){
                Iterator it = Begin(FIRST[index_of_char(allProduction[i].after[k+1])-VTLength]);
                while(it!=FIRST[index_of_char(allProduction[i].after[k+1])-VTLength]->head){
                    if(in_first_or_follow(FOLLOW[index_of_char(allProduction[i].after[k])-VTLength],*(char*)it->data) == 0){
                        Append(FOLLOW[index_of_char(allProduction[i].after[k])-VTLength],(char*)it->data,NULL);
                        append_follow_father(allProduction[i].after[k],(char*)it->data);
                    }
                    Next(&it);
                }

            }
        }
    }
}
//判断两个项目集是否相等
int equalItems(const Items* a,const Items* b){
    if(a->length != b->length){
        return 0;
    }
    else if(a->main_length != b->main_length){
        return 0;
    }
    else {
        bool equal1 = false;
        Iterator ait = Begin(a->item_list);
        Iterator bit = Begin(b->item_list);
        int i;
        for(i = 0;i<a->main_length;i++){
            int j = 0;
            bit = Begin(b->item_list);
            for(j = 0;j<a->main_length;j++){
                if(((Item*)ait->data) == ((Item*)bit->data)){
                    equal1 = true;
                    break;
                }
                Next(&bit);
            }
            if(equal1 == false)
                return 0;
            Next(&ait);
        }
    }
    return 1;
}

int in_list(List C_list,Items* goal_items){
    Iterator it = Begin(C_list);
    int i = 0;
    while(it!=C_list->head){

        if(equalItems((Items*)it->data,goal_items) == 1){
            return i;
        }
        Next(&it);
        i++;
    }
    return -1;
}

int in_added(List added_list,char c){
    Iterator it = Begin(added_list);
    while(it!=added_list->head){
        if(index_of_char(c) == *(int*)it->data)
            return 1;
        Next(&it);
    }
    return 0;

}
//获取预测分析表
void getTable(){
    InitList(&table,sizeof(Action)*(VTLength+VNLength));
    InitList(&C,sizeof(Items));

    Items* start_items = (Items*)malloc(sizeof(Items));
    InitList(&start_items->item_list,sizeof(Item));
    Append(start_items->item_list,&allItems[0],NULL);
    getClosure(start_items);
    Append(C,start_items,NULL);
    C_num = 1;
    Iterator it = Begin(C);
    int mm = 0;
    while(it!=C->head){
        Action* actions = (Action*)malloc((VTLength+VNLength)*sizeof(Action));
        int action_num ;
        for(action_num = 0;action_num<VTLength+VNLength;action_num++){
            actions[action_num].action = 'e';
            actions[action_num].index = 0;
        }
        Append(table,actions,NULL);
        Iterator it1 = Begin(((Items*)it->data)->item_list);
        List added_X;
        InitList(&added_X,sizeof(int));
        Items* temp_list[VTLength+VNLength];
        while(it1!=((Items*)it->data)->item_list->head){
            if(((Item*)it1->data)->dot_location == ((Item*)it1->data)->afterLength){//此处规约
                if(((Item*)it1->data)->before[0] == 'A'){
                    int pos = index_of_char('$');
                    actions[pos].action = 'a';
                    actions[pos].index = 0;
                }else{
                    List my_follow = FOLLOW[index_of_char(((Item*)it1->data)->before[0])-VTLength];
                    Iterator my_it = Begin(my_follow);
                    while(my_it!=my_follow->head){
                        int position = index_of_char(*(char*)my_it->data);
                        actions[position].action = 'r';
                        actions[position].index = ((Item*)it1->data)->production_index;
                        Next(&my_it);
                    }
                }


            }
            else{
                if(in_added(added_X,get_after_dot(it1->data)) == 1){
                    Append(temp_list[index_of_char(get_after_dot(it1->data))]->item_list,&allItems[allProduction[((Item*)it1->data)->production_index].start+((Item*)it1->data)->dot_location+1],NULL);

                }else{
                    int* temp_x = malloc(sizeof(int));
                    *temp_x = index_of_char(get_after_dot(it1->data));
                    Append(added_X,temp_x,NULL);
                    temp_list[index_of_char(get_after_dot(it1->data))] = (Items*)malloc(sizeof(Items));
                    InitList(&temp_list[index_of_char(get_after_dot(it1->data))]->item_list,sizeof(Item));
                    Append(temp_list[index_of_char(get_after_dot(it1->data))]->item_list,&allItems[allProduction[((Item*)it1->data)->production_index].start+((Item*)it1->data)->dot_location+1],NULL);
                }
            }
            Next(&it1);
        }
        Iterator added_it;
        added_it = Begin(added_X);
        while(added_it!=added_X->head){
            getClosure(temp_list[*(int*)added_it->data]);
            int position = in_list(C,temp_list[*(int*)added_it->data]);
            if(position == -1){
                if(*(int*)added_it->data<VTLength){
                    actions[*(int*)added_it->data].action = 's';
                    actions[*(int*)added_it->data].index = C_num;

                }else if(*(int*)added_it->data>=VTLength&&*(int*)added_it->data<VTLength+VNLength){
                    actions[*(int*)added_it->data].action = 'g';
                    actions[*(int*)added_it->data].index = C_num;
                }
                C_num++;
                Append(C,temp_list[*(int*)added_it->data],NULL);
            }else{
                if(*(int*)added_it->data<VTLength){
                    actions[*(int*)added_it->data].action = 's';
                    actions[*(int*)added_it->data].index = position;

                }else if(*(int*)added_it->data>=VTLength&&*(int*)added_it->data<VTLength+VNLength){
                    actions[*(int*)added_it->data].action = 'g';
                    actions[*(int*)added_it->data].index = position;
                }
            }
            Next(&added_it);

        }
        Next(&it);
        mm++;
    }
    FILE* tablefile = fopen("SLRTable.txt","w");
    Iterator table_it = Begin(table);
    int j = 0;
    while(table_it!=table->head){
        int i ;
        for(i = 0;i<VTLength+VNLength;i++){
            if(((Action*)table_it->data)[i].index>=0&&((Action*)table_it->data)[i].index<=9)
                fprintf(tablefile,"%c%d%d%d ",((Action*)table_it->data)[i].action,0,0,((Action*)table_it->data)[i].index);
            else if(((Action*)table_it->data)[i].index>=10&&((Action*)table_it->data)[i].index<=99)
                fprintf(tablefile,"%c%d%d ",((Action*)table_it->data)[i].action,0,((Action*)table_it->data)[i].index);
            else
                fprintf(tablefile,"%c%d ",((Action*)table_it->data)[i].action,((Action*)table_it->data)[i].index);
        }
        fprintf(tablefile,"\n");
        Next(&table_it);
        j++;
    }

}
