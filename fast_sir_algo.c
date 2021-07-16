#include "graph.h"
#include "Pqueue.h"
#include "lists.h"
#include <time.h>
#include <limits.h>


#define HEADS 1
#define population 10000
#define max_of_edges 3000
#define maximum_time 300
#define trans_prob 0.1
#define rec_prob 0.2

int biased_coin(double prob)											//Tosses a Biased Coin with a probability
{
	int i = (int)(prob*100);
	if(rand()%100 <= i)
		return HEADS;
	else
		return (1 - HEADS);
}

int get_heads_at(double prob)											//Return the day at which we get head
{
	int count = 0;
	if(prob == 0)
		return INT_MAX;
	while(1)
	{
		if(biased_coin(prob) == 1)
			break;
		else
			count +=1;
	}
	return count+1;
}


void find_trans_SIR(event_node** Q,Graph* G,int t,double tau,node* u,int t_max)		//Transmission across infected vertex
{
	node* ngbs = get_ngbs(G,u->vertex);
	int inf_time;
	
	while(ngbs)
	{
		if(ngbs->status == 0)
		{
			inf_time = t + get_heads_at(tau);

			if(inf_time < u->rec_time && inf_time < ngbs->inf_time && inf_time < t_max)
			{
				ngbs->inf_time = inf_time;
				ngbs->status = 1;
				if(search_queue(ngbs,Q) == 0)
					Q = push(Q,inf_time,1,ngbs);
			}
		}
		ngbs = ngbs->next;
	}
}
				
box** process_trans_SIR(Graph* G,node* u,int t,double tau,double gamma,int *times,box* S,box* I,box* R,event_node** Q,int t_max,int status)		//Transmists the virus and get the recovery time
{
	box** LISTS = (box**)malloc(3*sizeof(box*));
	if(status == 1)
		*times += t;
	int f = get_heads_at(gamma);
	S = erase(S,u);
	I = add(I,u);
	G = changeStatus(G,u->vertex,1);
	LISTS[0] = S;
	LISTS[1] = I;
	LISTS[2] = R;
	if(f == INT_MAX)
		u->rec_time = INT_MAX;
	else
		u->rec_time = t + f;
	if(u->rec_time < t_max)
	{	
		if(search_queue(u,Q) == 0)
			Q = push(Q,u->rec_time,0,u);
	}
	find_trans_SIR(Q,G,t,tau,u,t_max);
	return LISTS;
}

box** process_rec_SIR(Graph* G,node* u,int t,int *times,box* S,box* I,box* R,int status,int t_max)	//Recovers the node
{
	box** LISTS = (box**)malloc(3*sizeof(box*));
	if(status == 1)
	{
		if((*times + t) > t_max)
			*times = t_max;
		else
			*times += t;
	}
	I = erase(I,u);
	R = add(R,u);
	G = changeStatus(G,u->vertex,-1);
	LISTS[0] = S;
	LISTS[1] = I;
	LISTS[2] = R;
	return LISTS;
}

void fast_SIR(Graph* G,double tau,double gamma,int t_max,int intial_infetants)				//Fast_SIR algorithms which initliasation of the Queue and Susceptibles
{
	event_node** Q = NULL;
	Q = (event_node**)malloc(sizeof(event_node*));
	event_node* Event = NULL;
	box* S = NULL;
	box* I = NULL;
	box* R = NULL;
	box** LISTS = NULL;
	int j = 0;
	int times = 0;
	int status = 1;
	FILE* ft = fopen("times.dat","w");
	FILE* fi = fopen("infections.dat","w");
	FILE* fs = fopen("susceptibles.dat","w");
	FILE* fr = fopen("recoveries.dat","w");
	node* temp = NULL;
	int initial_time;

	
	for(int i = 0;i<G->no_of_vertices;i++)
	{
		if(G->adjlists[i])
		{
			temp = G->adjlists[i];
			while(temp)
			{
				temp->inf_time = INT_MAX;
				if(search_list(temp,S) == 0)
					S = add(S,temp);

				if(temp->status == 1)
				{
					temp->inf_time = 0;
					if( j == 0)
					{
						event_node* intial = newnode(0,1,temp);
						(*Q) = intial;
						j++;
					}
					else
					{
						if(search_queue(temp,Q) == 0)
							Q = push(Q,0,1,temp);
					}
				}
				temp = temp->next;
			}
		}
		
	}
	fprintf(ft,"%d\n",0);
	fprintf(fs,"%d\n",list_len(S));
	fprintf(fi,"%d\n",intial_infetants);
	fprintf(fr,"%d\n",0);
	while(*Q)
	{
		if(times < t_max)
		{
			initial_time = times;
			Event = pop(Q);
			if((*Q))
			{
				if((*Q)->time == Event->time)
					status = 0;
				else
					status =1;
			}
			if(Event->action == 1)
			{
				if(Event->Node->status == 1)							//0 for susescptible
				{								
					LISTS = process_trans_SIR(G,Event->Node,Event->time,tau,gamma,&times,S,I,R,Q,t_max,status);
					S = LISTS[0]; 
					I = LISTS[1]; 
					R = LISTS[2]; 
				}
			}
			else
			{
				LISTS = process_rec_SIR(G,Event->Node,Event->time,&times,S,I,R,status,t_max);
				S = LISTS[0];
				I = LISTS[1];
				R = LISTS[2];
			}
			if(status == 1 && initial_time != times)
			{
				fprintf(ft,"%d\n",times);
				fprintf(fs,"%d\n",list_len(S));
				fprintf(fi,"%d\n",list_len(I));
				fprintf(fr,"%d\n",list_len(R));
			}
		}
		else
			break;
	}
	int i = list_len(I);
	int r = list_len(R);
	int s = list_len(S);
	printf("Final summary after %d days of simulation.\n\n",t_max);

	printf("No of Infecteds are: %d\n",i);
	printf("No of Recovered are: %d\n",r);
	printf("No of Susceptibles are: %d\n",s);
	fclose(ft);
	fclose(fs);
	fclose(fi);
	fclose(fr);
}


int main()
{
	Graph* graph = randomise_graph(population,max_of_edges);
	int intial_infetants;

	printf("Enter number of intial infecteds:");
	scanf("%d",&intial_infetants);
	printf("\n\n\n");
	if(intial_infetants <= population)
	{
		graph = initial_infecteds(graph,intial_infetants);
		printf("Intial status summary.\n\n");
		printf("Total Population : %d\n",population);
		printf("Time for simulation : %d\n",maximum_time);
		printf("Maximum number of edges  : %d\n",max_of_edges);
		printf("Transmiting Probability : %f\n",trans_prob);
		printf("Recovering Probability : %f\n",rec_prob);
		printf("Number of intial infected : %d\n\n\n",intial_infetants);
		fast_SIR(graph,trans_prob,rec_prob,maximum_time,intial_infetants);
	}
	else
		printf("Impossiple scenario!\n\tMore number of infecteds than population\n");
}

