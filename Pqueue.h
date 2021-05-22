#include <stdio.h>
#include <stdlib.h>


struct node;

typedef struct event_node
{
	int time;
	int action;     														//1 for transmit and 0 for recover
	node* Node;
	struct event_node* next;
}event_node;

event_node* newnode(int tme,int act,node* Nde)							//Creates a new node for Priority Queue
{
	event_node* temp = (event_node*)malloc(sizeof(event_node));
	
	temp->time = tme;
	temp->action = act;
	temp->Node = Nde;
	temp->next = NULL;
	
	return temp;
}
		
event_node* pop(event_node** head)										//Pops the node with least time 
{
	event_node* temp =(*head);
	(*head) = (*head)->next;
	return temp;
}

event_node** push(event_node** head,int tme,int evt,node* Nde)			//Pushes the node according to time
{
	event_node* new_node = newnode(tme,evt,Nde);
	event_node* temp = (*head);
	if(*head)
	{
		if((*head)->time > tme)
		{
			new_node->next = (*head);
			(*head) = new_node;
			return head;
		}
		while(temp->next != NULL && temp->next->time < tme)
			temp = temp->next;
		new_node->next = temp->next;
		temp->next = new_node;
	}
	else
	{
		*head = new_node;
	}

	return head;
}

int search_queue(node* Node,event_node** head)							//Searches for a node in the queue 
{
	event_node* temp = (*head);
	while(temp != NULL)
	{
		if(temp->Node->vertex == Node->vertex)
			return 1;
		temp = temp->next;
	}
	return 0;
}

void printQueue(event_node** head)										//Prints Queue
{
	event_node* temp = (*head);
	int i =0;
	while(temp != NULL)
	{
		printf("%d : Time is %d , Event is %d & {Node:%d , status:%d , inf_time: %d , rec_time: %d}\n",i+1,temp->time,temp->action,temp->Node->vertex,temp->Node->status,temp->Node->inf_time,temp->Node->rec_time);
		temp = temp->next;
		i++;
	}
}
