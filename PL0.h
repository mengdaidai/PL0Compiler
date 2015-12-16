/*
*   Created By DaiXinjie @2015-12-13
*/

#include <stdio.h>
#include "NodeList.h"
#include "Stack.h"

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
#define MAXPRONUM 100   //  maximum production length
#define MAXITEMNUM 500  //maximum item num
#define MAXPROSIZE 10

typedef int bool ;
#define true 1
#define false 0

#define VTLength 33
#define VNLength 26
#define C_NUM 119


bool isAnnotation = false;

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




typedef struct
{
	int f; // function code
	int l; // level
	int a; // displacement address
} instruction;

//////////////////////////////////////////////////////////////////////
char* err_msg[] =
{
/*  0 */    "",
/*  1 */    "Found ':=' when expecting '='.",
/*  2 */    "There must be a number to follow '='.",
/*  3 */    "There must be an '=' to follow the identifier.",
/*  4 */    "There must be an identifier to follow 'const', 'var', or 'procedure'.",
/*  5 */    "Missing ',' or ';'.",
/*  6 */    "Incorrect procedure name.",
/*  7 */    "Statement expected.",
/*  8 */    "Follow the statement is an incorrect symbol.",
/*  9 */    "'.' expected.",
/* 10 */    "';' expected.",
/* 11 */    "Undeclared identifier.",
/* 12 */    "Illegal assignment.",
/* 13 */    "':=' expected.",
/* 14 */    "There must be an identifier to follow the 'call'.",
/* 15 */    "A constant or variable can not be called.",
/* 16 */    "'then' expected.",
/* 17 */    "';' or 'end' expected.",
/* 18 */    "'do' expected.",
/* 19 */    "Incorrect symbol.",
/* 20 */    "Relative operators expected.",
/* 21 */    "Procedure identifier can not be in an expression.",
/* 22 */    "Missing ')'.",
/* 23 */    "The symbol can not be followed by a factor.",
/* 24 */    "The symbol can not be as the beginning of an expression.",
/* 25 */    "The number is too great.",
/* 26 */    "",
/* 27 */    "",
/* 28 */    "",
/* 29 */    "",
/* 30 */    "",
/* 31 */    "",
/* 32 */    "There are too many levels."
};

//////////////////////////////////////////////////////////////////////
char ch;         // last character read
int  sym;        // last symbol read
char id[MAXIDLEN + 1]; // last identifier read
int  num;        // last number read
int  cc = -1;         // character count
int  ll = -1;         // line length
int  kk;
int  err;
int  cx;         // index of current instruction to be generated.
int  level = 0;
int  tx = 0;

char line[80];

instruction code[CXMAX];

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
	char  name[MAXIDLEN + 1];
	int   kind;
	short level;
	short address;
} mask;

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
    int main_length;//记录主项目个数，为判断项目集是否相等提供便利，即只判断主项目即可判断两个项目集是否相等
}Items;

typedef struct
{
    char* before;
    char* after;
    int start;
    int afterLength;
}Production;

typedef struct
{
    char action;
    int index;
}Action;


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



Production allProduction[MAXPRONUM];
int allProductionLength;
Item allItems[MAXITEMNUM];
int allItemLength;
int C_num;
List C;
List table;
Action** all_actions;


char Xs[VTLength+VNLength] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','(',')','*','+',
                              ',','-','.',';','<','=','>','/','#'};

List FIRST[VNLength];
List FOLLOW[VNLength];

List first_include[VNLength];
List follow_include[VNLength];

Stack status_stack;
Stack X_stack;
Stack attribute_stack;


List main_id_list;
List current_id_list;
int current_level = 0;
int current_statement_index;
List aim_statements;

FILE* infile;
