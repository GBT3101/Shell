#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct List
{
    char* name;
    char* value;
    struct List* nextLink;
} List;


List* createNode(char* name, char* value, List* next);

void deleteNode(List* node);

int searchInList(List* list, char* name);

char* valueOfName(List* list, char* name);

void changeListValueInLoc(List* list, char* name, char* value, int loc);

List* addToList(List* list, char* name, char* value);

List* deleteFromList(List* list, char* name);

void freeList(List* list);

void printList(List* list);