/*
*   Created By DaiXinjie @2015-12-13
*/


#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include "NodeList.h"


struct stack{
    List s_list;
};

typedef struct stack* Stack;

void InitStack(Stack* stack,int data_size);

void Push(Stack stack,void* data);

void Pop(Stack stack);

Node* Top(Stack stack);




#endif // STACK_H_INCLUDED
