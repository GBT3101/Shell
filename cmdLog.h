#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct cmdLog
{
    char * str[10];
    int size;
} cmdLog;


void intLog(cmdLog* cLog);

void pushArray(cmdLog* cLog, char* cmd);

void printArray(cmdLog* cLog);

void clearArray(cmdLog* cLog);

char* readLog(cmdLog* cLog,int loc);