#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "LineParser.h"

int **createPipes(int nPipes);

void releasePipes(int **pipes, int nPipes);

int *leftPipe(int **pipes, cmdLine *pCmdLine);

int *rightPipe(int **pipes, cmdLine *pCmdLine);
