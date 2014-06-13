#include "cmdLog.h"

void intLog(cmdLog* cLog){
	cLog->size= 0;
}

void pushArray(cmdLog* cLog, char* cmd){
	char* tmp1;
	char* tmp2;
	int i;
	char* newstr = (char*)malloc(strlen(cmd)+1);
	memcpy(newstr,cmd,strlen(cmd)+1);
	if(cLog->size<10){
		cLog->str[cLog->size]=newstr;
		cLog->size = (cLog->size)+1;
	}
	else{
		tmp1 = cLog->str[(cLog->size)-1];
		for(i= (cLog->size)-2;i>=0;i--){
			tmp2 = cLog->str[i];
			cLog->str[i]=tmp1;
			tmp1 = tmp2;
		}
		free(tmp1);
		cLog->str[9] = newstr;
	}
}

void printArray(cmdLog* cLog){
	int i;
	for(i=0;i<cLog->size;i++){
		printf("%s\n",cLog->str[i]);
	}
}

void clearArray(cmdLog* cLog){
	int i;
	for(i=0;i<cLog->size;i++){
		free(cLog->str[i]);
	}
}

char* readLog(cmdLog* cLog,int loc){
	if (cLog->size <= loc){
		return 0;
	}
	return cLog->str[loc];
}