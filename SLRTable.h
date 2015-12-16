/*
*  Created by DaiXinjie @2015-12-16
*  ����Ԥ���������ر�������
*/

#ifndef SLRTABLE_H_INCLUDED
#define SLRTABLE_H_INCLUDED

#include "Common.h"
#include "Stack.h"
#include "NodeList.h"
#define MAXITEMNUM 500  //maximum item num

typedef int bool ;


typedef struct
{
    int dot_location;
    char* before;
    char* after;
    int production_index;
    int afterLength;
}Item;

typedef struct
{
    List item_list;
    int length;
    int main_length;//��¼����Ŀ������Ϊ�ж���Ŀ���Ƿ�����ṩ��������ֻ�ж�����Ŀ�����ж�������Ŀ���Ƿ����
}Items;


Item allItems[MAXITEMNUM];
int allItemLength;
int C_num;
List C;
List table;

List FIRST[VNLength];
List FOLLOW[VNLength];

List first_include[VNLength];
List follow_include[VNLength];





#endif // SLRTABLE_H_INCLUDED
