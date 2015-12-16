/*
*   Created By DaiXinjie @2015-12-13
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "PL0.h"
#define true 1
#define false 0


//////////////////////////////////////////////////////////////////////
void getch(void)
{

	if (cc == ll)
	{
		if (feof(infile))
		{
			printf("\nPROGRAM INCOMPLETE\n");
			exit(1);
		}
		ll = cc = 0;
		while (!feof(infile) && (ch = getc(infile))!='\n')
		{
		    printf("%c", ch);
			line[++ll] = ch;
		} // while
		printf("\n");
		line[++ll] = ' ';
	}
	ch = line[++cc];

} // getch

//////////////////////////////////////////////////////////////////////
// gets a symbol from input stream.
void getsym(void)
{
	int i, k;
	char a[MAXIDLEN + 1];

	while (ch == ' '||ch == '\t')
		getch();

	if (isalpha(ch))
	{ // symbol is a reserved word or an identifier.
		k = 0;
		do
		{
			if (k < MAXIDLEN)
				a[k++] = ch;
			getch();
		}
		while (isalpha(ch) || isdigit(ch));
		a[k] = 0;
		strcpy(id, a);
		word[0] = id;
		i = NRW;
		while (strcmp(id, word[i--]));
		if (++i)
			sym = wsym[i]; // symbol is a reserved word
		else
			sym = SYM_IDENTIFIER;   // symbol is an identifier
	}
	else if (isdigit(ch))
	{ // symbol is a number.
		k = num = 0;
		sym = SYM_NUMBER;
		do
		{
			num = num * 10 + ch - '0';
			k++;
			getch();
		}
		while (isdigit(ch));
		if (k > MAXNUMLEN)
			error(25);     // The number is too great.
	}
	else if (ch == ':')
	{
		getch();
		if (ch == '=')
		{
			sym = SYM_BECOMES; // :=
			getch();
		}
		else
		{
			sym = SYM_NULL;       // illegal?
		}
	}
	else if (ch == '>')
	{
		getch();
		if (ch == '=')
		{
			sym = SYM_GEQ;     // >=
			getch();
		}
		else
		{
			sym = SYM_GTR;     // >
		}
	}
	else if (ch == '<')
	{
		getch();
		if (ch == '=')
		{
			sym = SYM_LEQ;     // <=
			getch();
		}
		else if (ch == '>')
		{
			sym = SYM_NEQ;     // <>
			getch();
		}
		else
		{
			sym = SYM_LES;     // <
		}
	}
	else
	{ // other tokens
		i = NSYM;
		csym[0] = ch;
		while (csym[i--] != ch);
		if (++i)
		{
			sym = ssym[i];
			getch();
		}
		else
		{
			printf("Fatal Error: Unknown character.\n");
			exit(1);
		}
	}

	/*if(sym == SYM_IDENTIFIER){
            printf("getsym read: ");
            printf(id);
        }
        else if(sym == SYM_NUMBER)
            printf("getsym read: %d",num);
        else
            printf("getsym read %d",sym);
            printf("\n");*/
}

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
        //printf(allItems[0].before);
        //printf("\n");
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
    //printf("%c",c);
    return c;
}//测试成功
//获取项目集闭包
void getItemClosure(Item* item,Items* itemset){
    char c = get_after_dot(item);
    //printf("%c",c);
    if(is_VT(c) == 1){
        return;
    }
    else if(is_VT(c) == 0){
        int i;
        for(i = 0;i<allProductionLength;i++){
            if(allProduction[i].before[0] == c){
                if(in_items(itemset,&allItems[allProduction[i].start]) == 0){
                    printf(allItems[allProduction[i].start].before);
                    printf("~");
                    printf(allItems[allProduction[i].start].after);
                    printf("%d\n",allItems[allProduction[i].start].dot_location);
                    Append(itemset->item_list,&allItems[allProduction[i].start],NULL);
                    itemset->length++;
                    getItemClosure(&allItems[allProduction[i].start],itemset);
                }

            }
        }

    }
    return ;
}//测试成功
//获取项目集闭包
void getClosure(Items* itemset){
    List itemset_list;//记得释放！！！！！！！！！！
    InitList(&itemset_list,sizeof(Item));
    Iterator temp_it = Begin(itemset->item_list);
    itemset->length = 0;
    itemset->main_length = 0;
    printf("主项目：\n");
    while(temp_it!=itemset->item_list->head){

        printf(((Item*)temp_it->data)->before);
        printf("~");
        printf(((Item*)temp_it->data)->after);
        printf("%d\n",((Item*)temp_it->data)->dot_location);
        Append(itemset_list,temp_it->data,NULL);
        itemset->length++;
        itemset->main_length++;
        Next(&temp_it);
    }
    printf("子项目：\n");
    Iterator it = Begin(itemset_list);
    int i = 0;
    while(it!=itemset_list->head){

       getItemClosure((Item*)it->data,itemset);
       i++;
       Next(&it);
    }

    itemset->main_length = i;
}//测试成功
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
    //printf("in first or follow:\n");
    Iterator it = Begin(list);
    int i = 0;
    while(it!=list->head){
        //printf("%d\n",i);
        i++;
        //printf("goal   %c   it   %c\n ",goal,*(char*)it->data);
        if(goal == *(char*)it->data){
            return 1;
        }
        Next(&it);
    }
    //printf("return 0");
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
            //printf("%c  %d\n",allProduction[i].after[0],index_of_char(allProduction[i].after[0])-VTLength);
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
        //printf("father %c  son   %c goal    %c\n",*(char*)it->data,c,*goal);
        if(in_first_or_follow(FIRST[index_of_char(*(char*)it->data)-VTLength],*goal) == 0){
            //printf("add   %c    to   first   of   %c\n",*goal,index_of_char(*(char*)it->data)-VTLength+65);
            Append(FIRST[index_of_char(*(char*)it->data)-VTLength],goal,NULL);
           //Iterator temp = Begin(FIRST[index_of_char('B')-VTLength]);
           // printf("after append the first of %c is %c\n",index_of_char('B')-VTLength+65,*(char*)temp->data);
            append_first_father(*(char*)it->data,goal);
        }
        Next(&it);
    }
}

void append_follow_father(char c,char* goal){
    Iterator it = Begin(follow_include[index_of_char(c)-VTLength]);
    while(it!=follow_include[index_of_char(c)-VTLength]->head){
        //printf("father %c  son   %c goal    %c\n",*(char*)it->data,c,*goal);
        if(in_first_or_follow(FOLLOW[index_of_char(*(char*)it->data)-VTLength],*goal) == 0){
            //printf("add   %c    to   follow   of   %c\n",*goal,index_of_char(*(char*)it->data)-VTLength+65);
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

         //printf("下一个产生式\n");
         if(is_VT(allProduction[i].after[0]) == 1){
            //printf("如果是终结符\n");
            if(in_first_or_follow(FIRST[index_of_char(allProduction[i].before[0])-VTLength],allProduction[i].after[0]) == 0){
                Append(FIRST[index_of_char(allProduction[i].before[0])-VTLength],&allProduction[i].after[0],NULL);
                //printf("给%c加入了%c\n",index_of_char(allProduction[i].before[0])-VTLength+65,allProduction[i].after[0]);
                append_first_father(allProduction[i].before[0],&allProduction[i].after[0]);
            }

         }
    }
}//测试成功
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
       // printf("下一个产生式:");
        //printf(allProduction[i].before);
       // printf("~");
       // printf(allProduction[i].after);
       // printf("\n");
        int k;
        for(k = 0;k<allProduction[i].afterLength-1;k++){
            if(is_VT(allProduction[i].after[k+1]) == 1&&is_VN(allProduction[i].after[k]) == 1){
                //printf("如果是终结符%c\n",allProduction[i].after[k+1]);
                if(in_first_or_follow(FOLLOW[index_of_char(allProduction[i].after[k])-VTLength],allProduction[i].after[k+1]) == 0){
                    Append(FOLLOW[index_of_char(allProduction[i].after[k])-VTLength],&allProduction[i].after[k+1],NULL);
                    //printf("给%c加入了%c\n",index_of_char(allProduction[i].after[k])-VTLength+65,allProduction[i].after[k+1]);
                    append_follow_father(allProduction[i].after[k],&allProduction[i].after[k+1]);
                }

            }else if(is_VN(allProduction[i].after[k+1]) == 1&&is_VN(allProduction[i].after[k]) == 1){
                //printf("如果是非终结符:%c\n",allProduction[i].after[k+1]);
                Iterator it = Begin(FIRST[index_of_char(allProduction[i].after[k+1])-VTLength]);
                while(it!=FIRST[index_of_char(allProduction[i].after[k+1])-VTLength]->head){
                    if(in_first_or_follow(FOLLOW[index_of_char(allProduction[i].after[k])-VTLength],*(char*)it->data) == 0){
                        Append(FOLLOW[index_of_char(allProduction[i].after[k])-VTLength],(char*)it->data,NULL);
                        //printf("给%c加入了%c\n",index_of_char(allProduction[i].after[k])-VTLength+65,*(char*)it->data);
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
            /*if(strcmp(((Item*)ait->data)->before,((Item*)bit->data)->before) == 0 && strcmp(((Item*)ait->data)->after,((Item*)bit->data)->after) == 0 && ((Item*)ait->data)->dot_location == ((Item*)bit->data)->dot_location){

            }*/
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
        //printf("%c",index_of_char(c));
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
        printf("第%d次\n",mm);
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
        Items* temp_list[VTLength+VNLength];//记得释放！！！！！！！！！！！！！！！！！！！！！
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
                printf("求出状态%d\n",C_num);
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
        printf("״̬%d\n",j);
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

/*以上是构造预测分析表所需要的*/
/*以下是分析源程序时需要的*/


//查找符号表
Identifier* lookup(char* id){
    List temp_list = current_id_list;
    while(temp_list!=NULL){
        //printf("lookupaddress:%x\n",temp_list);
        Iterator it = Begin(temp_list);
        while(it!=temp_list->head){
            char* name = ((Identifier*)it->data)->name;
            if(strcmp(name,id) == 0){
                return (Identifier*)it->data;
            }
            Next(&it);
        }
        temp_list = ((Proc_head*)temp_list->head->data)->father;
    }


    return NULL;
}
//创建对应于某个过程的符号表
List create_identifier_list(char* proc,List father){//构建proc对应Identifier时执行，执行后设置到id里
    List temp_list;
    InitList(&temp_list,sizeof(Identifier));
    Proc_head* head = (Proc_head*)malloc(sizeof(Proc_head));
    head->father = father;
    head->level = current_level;
    head->name = proc;
    temp_list->head->data = head;
    return temp_list;
}


//产生目标代码
Statement* add_statement(int f,int l,int a){
    Statement* temp_statement = (Statement*)malloc(sizeof(Statement));
    temp_statement->a = a;
    temp_statement->l = l;
    temp_statement->op_code = f;
    temp_statement->index = ++current_statement_index;
    Append(aim_statements,temp_statement,NULL);
    return temp_statement;
}

/*
*   以下都是语义分析规约时调用的函数
*/
void constance_definition(){
    Identifier* temp_identifier = (Identifier*)malloc(sizeof(Identifier));
    //无符号整数弹出
    Pop(status_stack);
    Pop(X_stack);
    int* number = (int*)Top(attribute_stack)->data;
    Pop(attribute_stack);
    temp_identifier->value = *number;
    temp_identifier->kind = ID_CONSTANT;
    free(number);
    //等号弹出
    Pop(status_stack);
    Pop(X_stack);
    Pop(attribute_stack);
    //标识符弹出
    Pop(status_stack);
    Pop(X_stack);
    char* id_name = (char*)Top(attribute_stack)->data;
    Pop(attribute_stack);
    temp_identifier->name = id_name;
    Append(current_id_list,temp_identifier,NULL);
    Push(X_stack,&allProduction[12].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[12].before[0])].index);
    Push(attribute_stack,NULL);
}


void add_variables(){
    //弹出分号
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出G一个或多个标识符
    Pop(X_stack);
    Pop(status_stack);
    List ids = (List)Top(attribute_stack)->data;
    int current_address = 3;
    Iterator it = Begin(ids);
    while(it!=ids->head){
        ((Identifier*)it->data)->address = current_address;
        Append(current_id_list,it->data,NULL);
        Next(&it);
        current_address++;
    }
    Pop(attribute_stack);
    free(ids);
    //弹出VAR
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    Push(X_stack,&allProduction[13].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[13].before[0])].index);
    Push(attribute_stack,NULL);
}

void merge_variables(){

    //弹出标识符
    Pop(X_stack);
    Pop(status_stack);
    char* id_name = (char*)Top(attribute_stack)->data;
    Identifier* temp_identifier = (Identifier*)malloc(sizeof(Identifier));
    temp_identifier->name = id_name;
    temp_identifier->kind = ID_VARIABLE;
    temp_identifier->level = current_level;
    Pop(attribute_stack);
    //弹出逗号
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出G多个标识符
    Pop(X_stack);
    Pop(status_stack);
    List temp_list = (List)Top(attribute_stack)->data;
    Append(temp_list,temp_identifier,NULL);
    Pop(attribute_stack);
    Push(X_stack,&allProduction[14].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[14].before[0])].index);
    Push(attribute_stack,temp_list);
}


void add_variable(){
    //弹出标识符
    Pop(X_stack);
    Pop(status_stack);
    char* id_name = (char*)Top(attribute_stack)->data;
    Identifier* temp_identifier = (Identifier*)malloc(sizeof(Identifier));
    temp_identifier->name = id_name;
    temp_identifier->kind = ID_VARIABLE;
    temp_identifier->level = current_level;
    List temp_list;
    InitList(&temp_list,sizeof(Identifier));
    Append(temp_list,temp_identifier,NULL);
    Pop(attribute_stack);
    Push(X_stack,&allProduction[15].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[15].before[0])].index);
    Push(attribute_stack,temp_list);
}

void finish_procedure_explain(){
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    Push(X_stack,&allProduction[16].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[16].before[0])].index);
    Push(attribute_stack,NULL);
}


void merge_procedures(){//关于数据区开辟还未确定
    //弹出分号
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出分程序
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出程序首部
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出多个过程说明
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    current_level--;
    current_id_list = ((Proc_head*)current_id_list->head->data)->father;
    Push(X_stack,&allProduction[17].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[17].before[0])].index);
    Push(attribute_stack,NULL);
}

void add_procedure(){
    //弹出分号
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出分程序
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出过程首部
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    current_level--;
    Proc_head* head = (Proc_head*)current_id_list->head->data;
    current_id_list = ((Proc_head*)current_id_list->head->data)->father;
    Push(X_stack,&allProduction[18].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[18].before[0])].index);
    Push(attribute_stack,NULL);
}

void new_procedure(){
    //弹出分号
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    Identifier* temp_identifier = (Identifier*)malloc(sizeof(Identifier));
    //弹出标识符
    Pop(X_stack);
    Pop(status_stack);
    char* proc_name = (char*)Top(attribute_stack)->data;
    Pop(attribute_stack);
    temp_identifier->name = proc_name;
    temp_identifier->level = current_level;
    current_level++;
    temp_identifier->kind = ID_PROCEDURE;
    temp_identifier->address = current_statement_index+1;
    List son = create_identifier_list(proc_name,current_id_list);
    temp_identifier->son = son;
    //弹出procedure
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    Append(current_id_list,temp_identifier,NULL);
    current_id_list = son;
    Push(X_stack,&allProduction[19].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[19].before[0])].index);
    Push(attribute_stack,NULL);
}

void assign_statement(){
    //弹出表达式
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出赋值号
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出标识符
    Pop(X_stack);
    Pop(status_stack);
    char* id_name = (char*)Top(attribute_stack)->data;
    Identifier* temp_identifier = lookup(id_name);
    add_statement(STO,current_level-temp_identifier->level,temp_identifier->address);
    Pop(attribute_stack);
    Push(X_stack,&allProduction[27].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[27].before[0])].index);
    Push(attribute_stack,NULL);
}

void condition_(){
    //弹出表达式
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出关系运算符
    Pop(X_stack);
    Pop(status_stack);
    int* opr_type = (int*)Top(attribute_stack)->data;
    if(*opr_type == SYM_LES){
        add_statement(OPR,0,OPR_LES);
    }else if(*opr_type == SYM_LEQ){
        add_statement(OPR,0,OPR_LEQ);
    }else if(*opr_type == SYM_GEQ){
        add_statement(OPR,0,OPR_GEQ);
    }else if(*opr_type == SYM_GTR){
        add_statement(OPR,0,OPR_GTR);
    }else if(*opr_type == SYM_EQU){
        add_statement(OPR,0,OPR_EQU);
    }else if(*opr_type == SYM_NEQ){
        add_statement(OPR,0,OPR_NEQ);
    }
    Statement* huitian_statement = add_statement(JPC,0,0);
    Pop(attribute_stack);
    free(opr_type);
    //弹出表达式
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    Push(X_stack,&allProduction[31].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[31].before[0])].index);
    Push(attribute_stack,huitian_statement);
}

void ji_ou(){
    //弹出表达式
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出odd
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    add_statement(OPR,0,OPR_ODD);
    Statement* huitian_statement = add_statement(JPC,0,0);
    Push(X_stack,&allProduction[32].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[32].before[0])].index);
    Push(attribute_stack,huitian_statement);
}

void count_operation(int type){
    //弹出项或因子
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出运算符号
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出附加
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    int production_index;
    if(type == SYM_PLUS){
        type = OPR_ADD;
        production_index = 35;
    }
    else if(type == SYM_MINUS){
        type = OPR_MIN;
        production_index = 36;
    }
    else if(type == SYM_TIMES){
        type = OPR_MUL;
        production_index = 45;
    }
    else if(type == SYM_SLASH){
        type = OPR_DIV;
        production_index = 46;
    }
    add_statement(OPR,0,type);
    Push(X_stack,&allProduction[production_index].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[production_index].before[0])].index);
    Push(attribute_stack,NULL);
}


void add_count_fujia(int type){
    //弹出项或因子
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出运算符号
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    int production_index;
    if(type == SYM_PLUS){
        type = OPR_ADD;
        production_index = 37;
    }
    else if(type == SYM_MINUS){
        type = OPR_MIN;
        production_index = 38;
    }
    else if(type == SYM_TIMES){
        type = OPR_MUL;
        production_index = 47;
    }
    else if(type == SYM_SLASH){
        type = OPR_DIV;
        production_index = 48;
    }
    if(type == OPR_ADD||type == OPR_MIN){
        /*if((*(char*)X_stack->s_list->head->last->last->last->last->data) == '-'){
            add_statement(LIT,0,0);
            if(type == OPR_ADD)
                add_statement(OPR,0,OPR_MIN);
            else
                add_statement(OPR,0,OPR_ADD);
            add_statement(OPR,0,OPR_ADD);
        }else{
            add_statement(OPR,0,type);
        }*/
        add_statement(OPR,0,type);
    }else if(type == OPR_MUL||type == OPR_MIN){
        add_statement(OPR,0,type);
    }
    Push(X_stack,&allProduction[production_index].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[production_index].before[0])].index);
    Push(attribute_stack,NULL);
}

/*void minus_R_to_P(){
    //弹出R
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出负号
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    add_statement(LIT,0,0);
    printf("add statement:\n");
    printf(mnemonic[((Statement*)aim_statements->head->last->data)->op_code]);
    printf(" ");
    printf("%d ",((Statement*)aim_statements->head->last->data)->l);
    printf("%d",((Statement*)aim_statements->head->last->data)->a);
    printf("\n");
    add_statement(OPR,0,OPR_MIN);
    printf("add statement:\n");
    printf(mnemonic[((Statement*)aim_statements->head->last->data)->op_code]);
    printf(" ");
    printf("%d ",((Statement*)aim_statements->head->last->data)->l);
    printf("%d",((Statement*)aim_statements->head->last->data)->a);
    printf("\n");
    Push(X_stack,&allProduction[39].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[39].before[0])].index);
    Push(attribute_stack,NULL);
}
*/
void a_to_T(){
    //弹出a
    Pop(X_stack);
    Pop(status_stack);
    char* id_name = (char*)Top(attribute_stack)->data;
    //printf("要找的idname:");
    //printf(id_name);
    Identifier* temp_id = lookup(id_name);
    int address = temp_id->address;
    if(temp_id->kind == ID_VARIABLE){
        add_statement(LOD,current_level-temp_id->level,address);
    }else if(temp_id->kind == ID_CONSTANT){
        add_statement(LIT,0,temp_id->value);
    }

    Pop(attribute_stack);
    Push(X_stack,&allProduction[49].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[49].before[0])].index);
    Push(attribute_stack,NULL);

}

void b_to_T(){
    //弹出b
    Pop(X_stack);
    Pop(status_stack);
    int* number = (int*)Top(attribute_stack)->data;
    add_statement(LIT,0,*number);
    Pop(attribute_stack);
    free(number);
    Push(X_stack,&allProduction[50].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[50].before[0])].index);
    Push(attribute_stack,NULL);
}

void guanxiyunsuanfu(int pro_index){
    //弹出运算符
    Pop(X_stack);
    Pop(status_stack);
    int* type = (int*)Top(attribute_stack)->data;
    Pop(attribute_stack);
    Push(X_stack,&allProduction[pro_index].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[pro_index].before[0])].index);
    Push(attribute_stack,type);
}


void condition_statement(){
    //弹出语句
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出then
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出条件
    Pop(X_stack);
    Pop(status_stack);
    Statement* temp_statement = (Statement*)Top(attribute_stack)->data;
    temp_statement->a = current_statement_index+1;
    Pop(attribute_stack);
    //弹出if
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    Push(X_stack,&allProduction[58].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[58].before[0])].index);
    Push(attribute_stack,NULL);
}

void call_statement(){
     //弹出a
    Pop(X_stack);
    Pop(status_stack);
    Identifier* temp_identifier = lookup((char*)Top(attribute_stack)->data);
    add_statement(CAL,current_level-temp_identifier->level,temp_identifier->address);
    Pop(attribute_stack);
    //弹出call
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    Push(X_stack,&allProduction[59].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[59].before[0])].index);
    Push(attribute_stack,NULL);
}

void while_statement(){
     //弹出语句
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出do
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
     //弹出条件
    Pop(X_stack);
    Pop(status_stack);
    Statement* temp_statement = (Statement*)Top(attribute_stack)->data;
    temp_statement->a = current_statement_index+2;
    Pop(attribute_stack);
    //弹出while
    Pop(X_stack);
    Pop(status_stack);
    add_statement(JMP,0,*(int*)Top(attribute_stack)->data);
    Pop(attribute_stack);
    Push(X_stack,&allProduction[60].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[60].before[0])].index);
    Push(attribute_stack,NULL);
}

void read_statement(){
    //弹出右括号
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
     //弹出G
    Pop(X_stack);
    Pop(status_stack);
    List temp_list = (List)Top(attribute_stack)->data;
    Iterator it = Begin(temp_list);
    while(it!=temp_list->head){
        add_statement(OPR,0,OPR_READ);
        Identifier* temp = lookup(((Identifier*)it->data)->name);
        add_statement(STO,current_level-temp->level,temp->address);
        Next(&it);
    }
    Pop(attribute_stack);
    //弹出左括号
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出read
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);

    Push(X_stack,&allProduction[61].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[61].before[0])].index);
    Push(attribute_stack,NULL);
}

void write_statement(){
    //弹出右括号
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
     //弹出G
    Pop(X_stack);
    Pop(status_stack);
    List temp_list = (List)Top(attribute_stack)->data;
    Iterator it = Begin(temp_list);
    while(it!=temp_list->head){
        Identifier* temp = lookup(((Identifier*)it->data)->name);
        add_statement(LOD,current_level-temp->level,temp->address);
        add_statement(OPR,0,OPR_WRITE);
        add_statement(OPR,0,OPR_LINE);
        Next(&it);
    }
    Pop(attribute_stack);
    //弹出左括号
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);
    //弹出write
    Pop(X_stack);
    Pop(status_stack);
    Pop(attribute_stack);

    Push(X_stack,&allProduction[62].before[0]);
    Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(allProduction[62].before[0])].index);
    Push(attribute_stack,NULL);
}
//语法分析、语义分析（语法制导翻译）
void grammarAnalyze(){
    current_statement_index = -1;
    InitList(&main_id_list,sizeof(Identifier));
    InitList(&current_id_list,sizeof(Identifier));
    Proc_head* main_head = (Proc_head*)malloc(sizeof(Proc_head));
    current_id_list->head->data = main_head;
    ((Proc_head*)current_id_list->head->data)->father = NULL;
    InitList(&aim_statements,sizeof(Statement));
    char* input_string = (char*)malloc(sizeof(char)*3000);
    infile = fopen("sourcecode1.txt","r");
    all_actions = (Action**)malloc(sizeof(Action*)*C_NUM);
    int act ;
    for(act = 0;act<C_NUM;act++){
        all_actions[act] = (Action*)malloc(sizeof(Action)*(VTLength+VNLength));
    }
    FILE* tablefile = fopen("SLRTable.txt","r");
    int row = 0;
    int col = 0;
    char* line = (char*)malloc(sizeof(char)*6);
    while(fgets(line,6,tablefile)){
        if(line[0] == '\n'){
            row++;
            col = 0;
            continue;
        }
        else{
            int index = (line[1]-48)*100+(line[2]-48)*10+(line[3]-48)*1;
            all_actions[row][col].action = line[0];
            all_actions[row][col].index = index;
            col++;
        }
    }
    /*int m,n;
    for(m = 0;m<C_NUM;m++){
        for(n = 0;n<VTLength+VNLength;n++){
            printf("%c",all_actions[m][n].action);
            printf("%d ",all_actions[m][n].index);
        }
        printf("\n");
    }*/
    InitStack(&status_stack,sizeof(int));
    InitStack(&X_stack,sizeof(char));
    InitStack(&attribute_stack,sizeof(int));
    int init_status = 0;
    Push(status_stack,&init_status);
    Push(X_stack,&Xs[index_of_char('$')]);
    Push(attribute_stack,NULL);
    bool successful = false;
    getch();
    getsym();

    while(!successful){
        if(sym == SYM_IDENTIFIER){
            printf("current string ");
            printf(id);
        }
        else if(sym == SYM_NUMBER)
            printf("current num %d",num);
        else
            printf("current else %d",sym);
            printf("\n");
            printf("\n");
       // Iterator curr_it= Begin(current_id_list);
       // printf("address of list %x:\n",current_id_list);
       // printf("list_length:%d\n",current_id_list->length);
        //printf("current_id_list  :\n");
       /* while(curr_it!=current_id_list->head){
            printf(((Identifier*)curr_it->data)->name);
            printf(",");
            Next(&curr_it);
        }*/
        printf("\n");
       // printf(id);
        //Node* node = Top(status_stack);
        //printf("%d",*(int*)node->data);
        //printf("\n");
        //printf("%d%d\n",*(int*)Top(status_stack)->data,sym-1);
        if(all_actions[*(int*)Top(status_stack)->data][sym-1].action == 's'){
            Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][sym-1].index);
            Push(X_stack,&Xs[sym-1]);
            if(sym == SYM_WHILE){
                int* temp_address = (int*)malloc(sizeof(int));
                *temp_address = current_statement_index+1;
                Push(attribute_stack,temp_address);
            }else if(sym == SYM_IDENTIFIER){
                char* temp_id = (char*)malloc(sizeof(char)*MAXIDLEN);
                strcpy(temp_id,id);
                Push(attribute_stack,temp_id);
            }else if(sym == SYM_NUMBER){
                int* number = (int*)malloc(sizeof(int));
                *number = num;
                Push(attribute_stack,number);
            }else if(sym == SYM_EQU||sym == SYM_LES||sym == SYM_NEQ||sym == SYM_LEQ||sym == SYM_GTR||sym == SYM_GEQ){
                int* type = (int*)malloc(sizeof(int));
                *type = sym;
                Push(attribute_stack,type);
            } else{
                Push(attribute_stack,NULL);
            }
            printf("yijin %c convert to %d\n",*(char*)Top(X_stack)->data,*(int*)Top(status_stack)->data);
            if(sym-1 != index_of_char('.')){
                getsym();
            }else{
                sym = SYM_ALL_END;
            }
        }else if(all_actions[*(int*)Top(status_stack)->data][sym-1].action == 'r'){
            char* c;
            int i;
            int r;
            printf("call production %d\n",all_actions[*(int*)Top(status_stack)->data][sym-1].index);
            switch(all_actions[*(int*)Top(status_stack)->data][sym-1].index){
            case 12:
                constance_definition();
                break;
            case 13:
                add_variables();
                break;
            case 14:
                merge_variables();
                break;
            case 15:
                add_variable();
                break;
            case 16:
                finish_procedure_explain();
                break;
            case 17:
                merge_procedures();
                break;
            case 18:
                add_procedure();
                break;
            case 19:
                new_procedure();
                break;
            case 27:
                assign_statement();
                break;
            case 31:
                condition_();
                break;
            case 32:
                ji_ou();
                break;
           /* case 39:
                minus_R_to_P();
                break;*/
            case 49:
                a_to_T();
                break;
            case 50:
                b_to_T();
                break;
            case 52:
                guanxiyunsuanfu(52);
                break;
            case 53:
                guanxiyunsuanfu(53);
                break;
            case 54:
                guanxiyunsuanfu(54);
                break;
            case 55:
                guanxiyunsuanfu(55);
                break;
            case 56:
                guanxiyunsuanfu(56);
                break;
            case 57:
                guanxiyunsuanfu(57);
                break;
            case 58:
                condition_statement();
                break;
            case 59:
                call_statement();
                break;
            case 60:
                while_statement();
                break;
            case 61:
                read_statement();
                break;
            case 62:
                write_statement();
                break;
            case 35:
                count_operation(SYM_PLUS);
                break;
            case 36:
                count_operation(SYM_MINUS);
                break;
            case 37:
                add_count_fujia(SYM_PLUS);
                break;
            case 38:
                add_count_fujia(SYM_MINUS);
                break;
            case 45:
                count_operation(SYM_TIMES);
                break;
            case 46:
                count_operation(SYM_SLASH);
                break;
            case 47:
                add_count_fujia(SYM_TIMES);
                break;
            case 48:
                add_count_fujia(SYM_SLASH);
                break;
            default:
                printf("enter default!!!!!\n");
                c = &allProduction[all_actions[*(int*)Top(status_stack)->data][sym-1].index].before[0];
                r = allProduction[all_actions[*(int*)Top(status_stack)->data][sym-1].index].afterLength;
                for(i = 0;i<r;i++){
                    printf("pop status %d  X  %c\n",*(int*)Top(status_stack)->data,*(char*)Top(X_stack)->data);
                    Pop(status_stack);
                    Pop(X_stack);
                    Pop(attribute_stack);
                    //printf("now top status %d  X  %c\n",*(int*)Top(status_stack)->data,*(char*)Top(X_stack)->data);
                }
                Push(X_stack,c);
                Push(status_stack,&all_actions[*(int*)Top(status_stack)->data][index_of_char(*c)].index);
                Push(attribute_stack,NULL);
                break;
                }
            printf("guiyue convert to %d\n",*(int*)Top(status_stack)->data);
            printf("%c  ruzhan\n",*(char*)Top(X_stack)->data);
        }else if(all_actions[*(int*)Top(status_stack)->data][sym-1].action == 'e'){
            printf("error!\n");
            break;
        }else if(all_actions[*(int*)Top(status_stack)->data][sym-1].action == 'a'){
            printf("success!\n");
            successful = true;
            add_statement(OPR,0,0);
        }
    }
}
