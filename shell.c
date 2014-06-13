#include "List.h"
#include "cmdLog.h"
#include "pipeTools.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <linux/limits.h>
#include <sys/wait.h>



List* execString(cmdLog* cLog, List* env, char* incoming);

void execute(cmdLine *pCmdLine){
	if (pCmdLine->inputRedirect){
		close(STDIN_FILENO);
		if (!fopen(pCmdLine->inputRedirect, "r")){
			_exit(EXIT_FAILURE);
		}
	}
	if (pCmdLine->outputRedirect){
		close(STDOUT_FILENO);
		if (!fopen(pCmdLine->outputRedirect, "w")){
			_exit(EXIT_FAILURE);	
		}
	}
	execvp(pCmdLine->arguments[0],pCmdLine->arguments);
	perror("illegal command");
	_exit(EXIT_FAILURE);
}

void cmdCD(cmdLog* cLog, cmdLine* line, char* incoming){
	pushArray(cLog, incoming);
	if(chdir(line->arguments[1])!=0)
		perror("illegal directory");
}

void cmdShowLog(cmdLog* cLog){
	printArray(cLog);
}

List* cmdReadLog(cmdLog* cLog, List* env, cmdLine* line){
	char* prevcmd;
	if ((strlen(line->arguments[0])>2) || (line->arguments[0][1]<'0' || line->arguments[0][1]>'9')){
    	perror("illegal arguments after '!'\n");
    }
	else{
		prevcmd = readLog(cLog,(line->arguments[0][1])-48);
		if (!prevcmd){
			perror("illegal arguments after '!'\n");	
		}
		else {
			return execString(cLog, env, prevcmd);
		}
	}
	return env;
}

List* cmdAssign(List* env, cmdLine* line){
	if (line->argCount != 3){
		perror("'assign' should have only 2 arguments\n");
	}
	else {
		env= addToList(env, line->arguments[1], line->arguments[2]);
	}
	return env;
}

List* cmdUnassign(List* env, cmdLine* line){
	if (line->argCount != 2){
		perror("'unassign' should have only one arguments\n");
	}
	else {
		env= deleteFromList(env, line->arguments[1]);
	}
	return env;
}

void cmdEnv(List* env){
	printList(env);
}

cmdLine* cmdEnvReplace(List* env, cmdLine* line){
	char* value;
	cmdLine* tempLine= line;
	int i;
	for (i= 0; i < tempLine->argCount; i++){
		if (tempLine->arguments[i][0] == '$'){
			value= valueOfName(env, &(tempLine->arguments[i][1]));
			if (value == (char*)-1){
				printf("the argument '%s' was not found\n", tempLine->arguments[i]);
				return 0;
			}
			if (replaceCmdArg(tempLine, i, value) == -1){
				printf("error while replacing argument '%s'\n", tempLine->arguments[i]);
				return 0;
			}
		}
	}
	return tempLine;
}

void coreExec(cmdLog* cLog, List* env, cmdLine* line){
	if (!strcmp(line->arguments[0], "env")){
	    	cmdEnv(env);
    }
    else if (!strcmp(line->arguments[0], "log")){
		cmdShowLog(cLog);
    }
    else{
		execute(line);
    }
    _exit(EXIT_SUCCESS);
}

List* execString(cmdLog* cLog, List* env, char* incoming){
	cmdLine* line;
	cmdLine* firstLine;
	cmdLine* currLine;
	int pipeCounter = 0;
	int * pid;
	int pid2;
	int i;
	int j;
	int **pipefd;
	line = parseCmdLines(incoming);
	currLine = line;
	firstLine = line;
    if (line==NULL || !strcmp(line->arguments[0], "quit")){
    	freeList(env);
    	freeCmdLines(line);
    	return (List*)-1;
    }
    line= cmdEnvReplace(env, line);
    if (!strcmp(line->arguments[0], "assign")){
    	env= cmdAssign(env, line);
    }
    else if (!strcmp(line->arguments[0], "unassign")){
    	env= cmdUnassign(env, line);
    }
    else if (!strcmp(line->arguments[0], "cd")){
    	cmdCD(cLog, line, incoming);
    }
    else if (line->arguments[0][0]=='!'){
    	env= cmdReadLog(cLog, env, line);
	}
    else {
    	j = 0;
    	pushArray(cLog, incoming);
    	if(line->next){
    		while(currLine != NULL){
    			currLine = currLine->next;
    			pipeCounter++;
    		}
    		pipefd= createPipes(pipeCounter-1);
    		pid = (int *)malloc(pipeCounter * sizeof(int));
    		if (!(pid[j] =fork())){ /* first son*/
				close(STDOUT_FILENO);          /* Close unused write end */
				dup(rightPipe(pipefd, line)[1]);
				close(rightPipe(pipefd, line)[1]);
				coreExec(cLog, env, line);
			}
			close(rightPipe(pipefd, line)[1]);
			j++;
			line = line->next;
			for (i=0; i<pipeCounter-2; i++){
				if (!(pid[j] =fork())){ /* first son*/
					close(STDOUT_FILENO);          /* Close unused write end */
					dup(rightPipe(pipefd, line)[1]);
					close(rightPipe(pipefd, line)[1]);
					close(STDIN_FILENO);
					dup(leftPipe(pipefd, line)[0]);
					close(leftPipe(pipefd, line)[0]);
					coreExec(cLog, env, line);
				}
				close(leftPipe(pipefd, line)[0]);
				close(rightPipe(pipefd, line)[1]);
				j++;
				line = line->next;
			}

			if (!(pid[j] =fork())){ /* first son*/
				close(STDIN_FILENO);          /* Close unused write end */
				dup(leftPipe(pipefd, line)[0]);
				close(leftPipe(pipefd, line)[0]);
				coreExec(cLog, env, line);
			}
			close(leftPipe(pipefd, line)[0]);
			for(i=0; i<=j; i++){
				if(line->blocking!=0){
					waitpid(pid[i],0,0);
				}
			}
			releasePipes(pipefd, pipeCounter-1);
		} else {
			if (!(pid2=fork())){
				coreExec(cLog, env, line);
			}
		}
		if(line->blocking!=0){
			waitpid(pid2,0,0);
		}
	}
    freeCmdLines(firstLine);
    free(pid);
    return env;
}

int main (int argc , char* argv[], char* envp[]){
	char my_cwd[PATH_MAX];
	char buffer[2048];
	cmdLog cLog;
	List* env= 0;
	intLog(&cLog);
	while(1){
	    getcwd(my_cwd, PATH_MAX);
	    printf("%s$ ", my_cwd);
	    fgets(buffer, 2048, stdin);
	    if ((buffer[0] != '\n') && ((env= execString(&cLog, env, buffer)) == (List*)-1)){
	    	break;
	    }
	}
	clearArray(&cLog);
	return 0;
}