#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct node														//Data type for node in graph
{
	int vertex;
	int status;															//1 for infected,0 for susceptible and -1 for recovered
	int inf_time;
	int rec_time;													
	struct node* next;
}node;
	
typedef struct Graph													//Data type for graph consists of edges
{
	int no_of_vertices;
	node** adjlists;
}Graph;



node* createNode(int vtx)												//Creates a node for graph
{
	node* Node = (node*)malloc(sizeof(node));
	Node->vertex = vtx;
	Node->status = 0;
	Node->next = NULL;
	return Node;
}

void addEdge(Graph* graph,int end1,int end2)							//Adds edges between two nodes
{
	node* temp = createNode(end2);
	if(graph->adjlists[end1] != NULL)
		temp->next = graph->adjlists[end1];
	else
		temp->next =NULL;
	graph->adjlists[end1] = temp;
	
	temp = createNode(end1);
	if(graph->adjlists[end2] != NULL)
		temp->next = graph->adjlists[end2];
	else
		temp->next =NULL;
	graph->adjlists[end2] = temp;
}

Graph* createGraph(int Vertices)										//Creates Graph with memory allocation
{
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	srand(time(NULL));
	graph->no_of_vertices = Vertices;
	graph->adjlists = (node**)malloc(Vertices*sizeof(node*));
	for( int i = 0;i<Vertices;i++)
		graph->adjlists[i] = NULL;
	return graph;
}

node* get_ngbs(Graph* graph,int vertex)									//Gets neigbhours for a vertex in the graph
{
	return graph->adjlists[vertex];
}

int no_of_ngbs(Graph* graph,int vertex)									//Returns number of neighbhours for a vertex in graph
{
	node* temp = get_ngbs(graph,vertex);
	if (!temp)
		return 0;
	int i=0;
	while(temp)
	{
		i++;
		temp = temp->next;
	}
	return i;
}



Graph* randomise_graph(int Vertices,int MaxEdges)						//Creates Randomised graph with max number of vertices
{
	struct Graph* graph = createGraph(Vertices);
	int ngbs;
	int random;
	//srand(time(NULL));
	for(int i =0 ;i<Vertices;i++)
	{
		ngbs = no_of_ngbs(graph,i);
		if(ngbs < 3)
		{
			for(int j =0;j< MaxEdges - ngbs;j++)
			{
				if(rand()%2 == 0)
				{
					again:
						random = rand()%Vertices;
						if(i != random)
							addEdge(graph,i,random);
						else
							goto again;
				}
			}
		}
	}
	return graph;
}



Graph* initial_infecteds(Graph* graph,int no_of_infecteds)				//Randomises infecteds in the graph
{
	node* temp = NULL; 
	int r;
	
	//~ if(no_of_infecteds == 1)
		//~ no_of_infecteds++;
	for(int j = 0;j< no_of_infecteds;j++)
	{
		//srand(time(NULL));
		r = random()%graph->no_of_vertices;
		for(int i =0;i < graph->no_of_vertices;i++)
		{
			if(graph->adjlists[i])
			{
				temp = graph->adjlists[i];
				while(temp)
				{
					if(temp->vertex == r)
						temp->status =1;
					temp = temp->next;
				}
			}
		}		
	}
	return graph;
}

void printGraph(Graph* graph)											//Prints graphs
{
	node* temp = NULL;
	for(int v =0;v<graph->no_of_vertices;v++)
	{
		printf("%d : ",v);
		temp = graph->adjlists[v];
		while(temp != NULL)
		{
			printf("(%d,%d) ",temp->vertex,temp->status);
			temp = temp->next;
		}
		printf("\n");
	}
}

Graph* changeStatus(Graph* graph,int u_vertex,int status)				//For changing the status of A vertex in the graph
{
	node* Nde = graph->adjlists[u_vertex];
	node* temp = NULL;
	while(Nde)
	{
		temp = graph->adjlists[Nde->vertex];
		while(temp)
		{
			if(temp->vertex == u_vertex)
				temp->status = status;
			temp = temp->next;
		}
		Nde = Nde->next;
	}
	return graph;
}

void printNgbs(node* Node)												//Prints neighbhours
{
	node* temp = Node;
	while(temp)
	{
		printf("%d ",temp->vertex);
		temp = temp->next;
	}
	printf("\n");
}
