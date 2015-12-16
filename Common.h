#ifndef COMMMON_H_INCLUDED
#define COMMMON_H_INCLUDED

#define MAXPROSIZE 10
#define MAXPRONUM 100   //  maximum production length
#define VTLength 33
#define VNLength 26

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

Production allProduction[MAXPRONUM];
int allProductionLength;

#endif // COMMMON_H_INCLUDED
