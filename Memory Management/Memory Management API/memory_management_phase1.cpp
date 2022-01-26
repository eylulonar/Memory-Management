#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h> 
#include <queue> 
#include <semaphore.h>
using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150

struct node
{
	int id;
	int size;
};


queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char  memory[MEMORY_SIZE]; // memory size





void my_malloc(int thread_id, int size)
{
	//This function will add the struct to the queue
	pthread_mutex_lock(&sharedLock); // locks the mutex
	node nd;
	nd.id=thread_id;
	nd.size=size;
	myqueue.push(nd); // node is pushed to the queue with thread_id and size 
	pthread_mutex_unlock(&sharedLock); //unlocks the mutex
}

void * server_function(void *)
{
	//This function should grant or decline a thread depending on memory size.
	int index=0;
	bool check=true;
	int count=0;
	while (check) // reads from the queue
	{

		if(!myqueue.empty())
		{
				
			pthread_mutex_lock(&sharedLock);	//locked until the memory server responses to the thread request

			node nd=myqueue.front();
			myqueue.pop();
			count ++;
			 if(nd.size<MEMORY_SIZE-index)// available memory size
			{
				thread_message[nd.id]=index; // grant the request
				index= index + nd.size + 1;
		
				
			}
			else//no enough memory
			{				
				thread_message[nd.id]=-1;	// decline the request
	
			}
			 	sem_post(&(semlist[nd.id])); //up on semaphore

				
				pthread_mutex_unlock(&sharedLock); //unlocked after response is written to thread_message array
				
				if(count==10)
				
				check=false; // when all requests done, server thread terminates
	}
		
	
	}
}

void * thread_function(void * id) 
{
	//This function will create a random size, and call my_malloc
	//Block
	//Then fill the memory with id's or give an error prompt
	int* id_ptr=(int*) id;
	int size= rand() % (25) + 1 ; //create random memory size (memorysize/6)

	my_malloc(*id_ptr , size);
	
	sem_wait (&(semlist[*id_ptr]));// down on semaphore
	pthread_mutex_lock(&sharedLock); //lock

	
	if(thread_message[*id_ptr]== -1)
	{
		cout<<"Thread "<< *id_ptr <<": No available memory. \n";
	}
	else  //interpret as the starting point in the memory
	{
		for (int i=0; i<= size ; i++)
		{
			char num= *id_ptr + '0';
			memory[thread_message[*id_ptr]+i]=num;
		}
	}
	
	pthread_mutex_unlock(&sharedLock); //unlock
}

void init()	 
{
	pthread_mutex_lock(&sharedLock);	//lock
	for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
	{sem_init(&semlist[i],0,0);}
	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory 
  	{char zero = '0'; memory[i] = zero;}
   	pthread_create(&server,NULL,server_function,NULL); //start server 
	pthread_mutex_unlock(&sharedLock); //unlock
}



void dump_memory() 
{
 // You need to print the whole memory array here.
	for(int i =0; i<MEMORY_SIZE; i++)
	{
		cout<<memory[i]<< " " ;
	}
}

int main (int argc, char *argv[])
 {

 	//You need to create a thread ID array here
	 int  thread_id[NUM_THREADS];
	 for(int i=0; i<NUM_THREADS;i++)
	 {
		 thread_id[i]=i;
	 }
 	init();	// call init

 	//You need to create threads with using thread ID array, using pthread_create()

 	//You need to join the threads
	pthread_t threads [NUM_THREADS];
	for(int i=0;i<NUM_THREADS;i++)
	{
		pthread_create (&threads[i], NULL, &thread_function, (void*) &thread_id[i]);
	}


	for(int i=0;i<NUM_THREADS;i++)
	{
		pthread_join (threads[i],NULL);
	}
		pthread_join(server,NULL);

 	dump_memory(); // this will print out the memory
 	
 	printf("\nMemory Indexes:\n" );
 	for (int i = 0; i < NUM_THREADS; i++)
 	{
 		printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
 	}
 	printf("\nTerminating...\n");
 }