#include <stdlib.h>
#include <stdio.h>

struct node;

typedef struct box														//Data type for list
{
	struct node* Node;
	struct box* next;
}box;

box* createList(node* Node)												//Creates list
{
	box* head = (box*)malloc(sizeof(box));
	head->Node = Node;
	head->next = NULL;
	
	return head;
}

box* add(box* head,node* Node)											//Inserts Node to the list
{
	box* temp = head;
	if(head == NULL)
	{
		head = createList(Node);
		return head;
	}
	while(temp->next != NULL)
		temp = temp->next;
	temp->next = createList(Node);
	
	return head;
}

int list_len(box* list)													//Returns list length
{
	box* temp = NULL;
	int i=1;
	temp = list;
	if(!list)
		return 0;
	while(temp->next != NULL)
	{
		i = i+1;
		temp = temp->next;
	}
	return i;
}

int search_list(node* Node,box* list)									//Searches for node in list
{
	box* temp = list;
	while(temp)
	{
		if(Node->vertex == temp->Node->vertex)
			return 1;
		temp = temp->next;
	}
	return 0;
}


void printList(box* list)												//Prints list 
{
	box* temp = list;
	while(temp != NULL)
	{
		printf("%d ",temp->Node->vertex);
		temp = temp->next;
	}
	printf("\n");
}


box* find(box* list,int vertex)											//For Finding a Vertex in the List
{
	box* temp = list;
	while(temp != NULL)
	{
		if(temp->Node->vertex == vertex)
			return temp;
		temp = temp->next;
	}
	return NULL;
}

box* del(box* list,box* position)										//For deleting the struct in the list
{
	if(position == list)
	{
		list = list->next;
		return list;
	}
	box* temp =list;
	
	while(temp->next != position)
		temp = temp ->next;
	temp->next = position->next;
	return list;
}

box* erase(box* list,node* Node)										//Finds a Node and deletes it 
{
	box* finder = find(list,Node->vertex);
	if(!finder)
	{
		printf("Not Found!!\n");
		return list;
	}
	list = del(list,finder);
	return list;
}

		
