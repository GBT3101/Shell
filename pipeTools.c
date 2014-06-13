#include "pipeTools.h"


int **createPipes(int nPipes){
	int **pipes = (int **)malloc(nPipes * sizeof(int*));
	int i;
	for(i=0; i<nPipes; i++){
		pipes[i]= (int *)malloc(2 * sizeof(int));
		pipe (pipes[i]);
	}
	return pipes;
}

void releasePipes(int **pipes, int nPipes){
	int i;
	for(i=0; i<nPipes; i++){
		free (pipes[i]);
	}
	free(pipes);
}

int *leftPipe(int **pipes, cmdLine *pCmdLine){
	if(pCmdLine->idx > 0){
		return pipes[pCmdLine->idx - 1];
	} else return NULL;
}

int *rightPipe(int **pipes, cmdLine *pCmdLine){
	if(pCmdLine->next != NULL){
		return pipes[pCmdLine->idx]; 
	} else return NULL;
}