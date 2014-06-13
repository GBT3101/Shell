#include "List.h"


List* createNode(char* name, char* value, List* next){
	char* newName;
	char* newValue;
	List* newNode;
	newName= (char*)malloc(strlen(name)+1);
	strcpy(newName, name);
	newValue= (char*)malloc(strlen(value)+1);
	strcpy(newValue, value);
	newNode= (List*)malloc(sizeof(List));
	newNode->name= newName;
	newNode->value= newValue;
	newNode->nextLink= next;
	return newNode;
}

void deleteNode(List* node){
	free(node->name);
	free(node->value);
	free(node);
}

int searchInList(List* list, char* name){
	List* currentLink= list;
	int i= 0;
	while (currentLink){
		if(!strcmp(currentLink->name,name)){
			return i;
		}
		currentLink= currentLink->nextLink;
		i++;
	}
	return -1;
}

char* valueOfName(List* list, char* name){
	List* currentLink= list;
	int i= 0;
	while (currentLink){
		if(!strcmp(currentLink->name,name)){
			return currentLink->value;
		}
		currentLink= currentLink->nextLink;
		i++;
	}
	return (char*)-1;
}

void changeListValueInLoc(List* list, char* name, char* value, int loc){
	List* currentLink= list;
	char* newValue;
	int i;
	for (i= 0;i < loc; i++){
		currentLink= currentLink->nextLink;
	}
	free(currentLink->value);
	newValue= (char*)malloc(strlen(value)+1);
	strcpy(newValue, value);
	currentLink->value= newValue;
}

List* addToList(List* list, char* name, char* value){
	char* newValue;
	List* newNode;
	List* currentLink;
	List* prevLink;
	if (!list){
		newNode= createNode(name, value, 0);
		list= newNode;
	}
	else {
		currentLink= list;
		while (currentLink){
			if(!strcmp(currentLink->name,name)){
				break;
			}
			prevLink= currentLink;
			currentLink= currentLink->nextLink;
		}
		if (currentLink){
			newValue= (char*)malloc(strlen(value)+1);
			strcpy(newValue, value);
			free(currentLink->value);
			currentLink->value= newValue;
		}
		else {
			newNode= createNode(name, value, 0);
			prevLink->nextLink= newNode;
		}
	}
	return list;
}

List* deleteFromList(List* list, char* name){
	int i;
	List* currentLink= list;
	List* tmpNode;
	if (!list){
		return NULL;
	}
	if (!strcmp(currentLink->name, name)){
		list= currentLink->nextLink;
		deleteNode(currentLink);
		return list;
	}
	while (currentLink){
		if((currentLink->nextLink) && (!strcmp(currentLink->nextLink->name,name))){
			break;
		}
		currentLink= currentLink->nextLink;
		i++;
	}
	if (!currentLink){
		return list;
	}
	tmpNode= currentLink;
	currentLink= currentLink->nextLink;
	tmpNode->nextLink= currentLink->nextLink;
	deleteNode(currentLink);
	return list;
}

void freeList(List* list){
	List* currentLink= list;
	List* tempLink;
	while (currentLink){
		tempLink= currentLink->nextLink;
		deleteNode(currentLink);
		currentLink= tempLink;
	}
}

void printList(List* list){
	List* currentLink= list;
	printf("<name,value>:\n");
	while (currentLink){
		printf("\t<%s,%s>\n", currentLink->name, currentLink->value);
		currentLink= currentLink->nextLink;
	}
}