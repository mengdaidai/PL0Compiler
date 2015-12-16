/*
*   Created By DaiXinjie @2015-12-13
*   栈相关函数的定义
*/
#include "Stack.h"
#include "NodeList.h"
void InitStack(Stack* stack,int data_size){
    Stack new_stack = (Stack)malloc(sizeof(struct stack));
    *stack = new_stack;
    InitList(&((*stack)->s_list),data_size);
}

void Push(Stack stack,void* data){
    Append(stack->s_list,data,NULL);
}

void Pop(Stack stack){
    Node* node = stack->s_list->head->last;
    RemoveNode(stack->s_list,node);
    free(node);
}

Node* Top(Stack stack){
    Node* node = stack->s_list->head->last;
    return node;
}

