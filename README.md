# PL0Compiler
PL0Compiler
 Compiler of PL0

使用SLR(1)分析法
产生预测分析表的函数调用：
FILE* BNFfile = fopen("BNF.txt","r");
getItems(BNFfile);
scan_first_include();
create_first();
scan_follow_include();
create_follow();
getTable();

最后程序将把产生的预测分析表写入SLRTable.txt文件，其中e000代表出错，gn代表程序转到n状态，rn代表用产生式n进行规约，sn代表移进并转到状态n

分析源程序的函数调用：
FILE* BNFfile = fopen("BNF.txt","r");
getProductions(BNFfile);
grammarAnalyze();

示例结果：
0 LOD 1 3
1 LIT 0 10
2 OPR 0 2
3 STO 1 4
4 OPR 0 13
5 STO 0 3
6 LOD 0 3
7 LIT 0 0
8 OPR 0 8
9 JPC 0 17
10 CAL 0 0
11 LOD 0 4
12 OPR 0 14
13 OPR 0 15
14 OPR 0 13
15 STO 0 3
16 JMP 0 6
17 OPR 0 0

附：
1、没有处理数据区开辟相关
2、BNF原.txt中是产生式及相关语义动作，部分解释与源代码有出入，一切以源代码为准
