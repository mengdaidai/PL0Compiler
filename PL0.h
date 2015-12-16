/*
*  Created by DaiXinjie @2015-12-16
*  源程序语法语义分析相关变量声明
*/

#ifndef PL0_H
#define PL0_H


#include <stdio.h>
#include "NodeList.h"
#include "Stack.h"
#include "Common.h"

#define NRW        13     // number of reserved words
#define TXMAX      500    // length of identifier table
#define MAXNUMLEN  14     // maximum number of digits in numbers
#define NSYM       11     // maximum number of symbols in array ssym and csym
#define MAXIDLEN   10     // length of identifiers

#define MAXADDRESS 32767  // maximum address
#define MAXLEVEL   32     // maximum depth of nesting block
#define CXMAX      500    // size of code array

#define MAXSYM     30     // maximum number of symbols

#define STACKSIZE  1000   // maximum storage


#define C_NUM 119

typedef int bool ;
#define true 1
#define false 0





enum symtype
{
	SYM_NULL,
	SYM_IDENTIFIER,//a
	SYM_NUMBER,//b
	SYM_ODD,//c
	SYM_NEQ,//d
	SYM_LEQ,//e
	SYM_GEQ,//f
	SYM_BECOMES,//g
    SYM_BEGIN,//h
	SYM_END,//i
	SYM_IF,//j
	SYM_THEN,//k
	SYM_WHILE,//l
	SYM_DO,//m
	SYM_CALL,//n
	SYM_CONST,//o
	SYM_VAR,//p
	SYM_PROCEDURE,//q
	SYM_READ,//r
	SYM_WRITE,//s
	SYM_LPAREN,//  (
    SYM_RPAREN,//  )
    SYM_TIMES,//  *
	SYM_PLUS,//  +
	SYM_COMMA,//  ,
	SYM_MINUS,//  -
	SYM_PERIOD,//  .
	SYM_SEMICOLON,//  ;
    SYM_LES,//  <
    SYM_EQU,//  =
	SYM_GTR,//  >
    SYM_SLASH,//
    SYM_jinghao,
    SYM_ALL_END
};

enum idtype
{
	ID_CONSTANT, ID_VARIABLE, ID_PROCEDURE
};

enum opcode
{
	LIT, OPR, LOD, STO, CAL, INT, JMP, JPC
};

enum oprcode
{
	OPR_RET, OPR_NEG, OPR_ADD, OPR_MIN,
	OPR_MUL, OPR_DIV, OPR_ODD, OPR_EQU,
	OPR_NEQ, OPR_LES, OPR_LEQ, OPR_GTR,
	OPR_GEQ,OPR_READ,OPR_WRITE,OPR_LINE
};








//////////////////////////////////////////////////////////////////////
char ch;         // last character read
int  sym;        // last symbol read
char id[MAXIDLEN + 1]; // last identifier read
int  num;        // last number read
int  cc ;         // character count
int  ll ;         // line length
int  kk;

char line[80];


char* word[NRW + 1] =
{
	"", /* place holder */
	"begin", "call", "const", "do", "end","if",
	"odd", "procedure", "then", "var", "while","read","write"
};

int wsym[NRW + 1] =
{
	SYM_NULL, SYM_BEGIN, SYM_CALL, SYM_CONST, SYM_DO, SYM_END,
	SYM_IF, SYM_ODD, SYM_PROCEDURE, SYM_THEN, SYM_VAR, SYM_WHILE,SYM_READ,SYM_WRITE
};

int ssym[NSYM + 1] =
{
	SYM_NULL, SYM_PLUS, SYM_MINUS, SYM_TIMES, SYM_SLASH,
	SYM_EQU, SYM_COMMA, SYM_SEMICOLON,SYM_LPAREN,SYM_RPAREN,SYM_PERIOD,SYM_jinghao
};

char csym[NSYM + 1] =
{
	' ', '+', '-', '*', '/', '=', ',', ';','(',')','.','#'
};

#define MAXINS   8

char* mnemonic[MAXINS] =
{
	"LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC"
};















typedef struct
{
    char* name;
    int kind;//在idtype中有表示
    int level;
    int address;
    int value;
    List son;
}Identifier;

typedef struct
{
    char* name;
    List father;
    int level;
}Proc_head;

typedef struct
{
    int op_code;
    int l;
    int a;
    int index;
}Statement;

Action** all_actions;


char Xs[VTLength+VNLength] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','(',')','*','+',
                              ',','-','.',';','<','=','>','/','#'};



Stack status_stack;
Stack X_stack;
Stack attribute_stack;


List main_id_list;
List current_id_list;
int current_level ;
int current_statement_index;
List aim_statements;

FILE* infile;

#endif // PL0_H
