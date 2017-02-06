// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include <string.h>

//******code edit by Toks Ipaye
#include "synch.h"


Semaphore** semaphore = new Semaphore*[100];	//semaphore array for chopsticks
bool stickAvail[100];	//array of chopsticks
int totPhil;		//the total amount of philosophers
int totMeal;		//the total amount of meals
int finished = 0;    //the philosophers done eating


void
DinePhil2(int which)
{
    bool imDone = FALSE;		//boolean checks when all threads are out of while loop
    char* name = currentThread->getName();	//the name of the current thread
    printf("%s sits at the table\n", name);	//the philosopher sits at table
    currentThread->Yield();			//yield for next philosopher to sit

    while (totMeal>0){									//while meals left
    	printf("%s is attempting to pick up chopstick %d on his left\n", name, which);
	semaphore[which]->P();							//for chopstick on left
	printf("%s successfully picks up chopstick %d on his left\n", name, which);
	printf("%s is attempting to pick up chopstick %d on his right\n", name, (which +1)%totPhil);
	(semaphore[(which+1)%totPhil])->P();					//for chopstick on right
	printf("%s successfully picks up chopstick %d on his right\n", name, (which +1)%totPhil);

        int randVal = Random()%4 +2;				//generate randome value between 2 and 5

	if (totMeal >0){					//if there still are meals left
	    printf("%s is starting to eat\n", name);		//eat
	    totMeal--;						//decrement meal quantity
	    printf("meal left: %d\n", totMeal);
	    for(int j = 0; j < randVal; j++)			
	    	currentThread->Yield();				//eating
	    printf("%s has finished eating\n", name);
	}else printf("the food is finished :(\n");		//print if food is finished

	printf("%s is putting chopstick %d down\n", name, which); //putting chopstick down
	semaphore[which]->V();						//making resource available
	printf("%s is putting chopstick %d down\n", name, (which+1)%totPhil);//putting chopstick down
	(semaphore[(which+1)%totPhil])->V();				//putting chopstick down

	printf("%s is starting to think\n", name);		
	randVal = Random()%4 +2;				//generate random value between 2 and 5
	for(int j = 0; j < randVal; j++)			
	    currentThread->Yield();				//thinking 
	printf("%s is done thinking\n", name);
    }

    while(finished != totPhil){				//while somethreads are still in upper while loop
        if(imDone != TRUE){		//loop and yield so that all threads can leave at the same time
	    imDone = TRUE;
            finished++;
        }
        currentThread->Yield();   
    }
    printf("%s leaves the table\n", name);
    currentThread->Yield();     
    currentThread->Finish();
}

void
DinePhil(int which)
{
    bool left=FALSE;			//boolean to indicate left hand possession of chopstick
    bool  right = FALSE;		//boolean to indicate right hand possession of chopstick

    char* name = currentThread->getName();	//name of current thread
    bool imDone = FALSE;			//boolean to indicate readiness to leave the room
    printf("%s sits at the table\n", name);	//philosopher sits at table
    currentThread->Yield();			//yields for next philosopher to sit
    while (totMeal>0){				//while there are still meals...


    	printf("%s is attempting to pick up chopstick %d on his left\n", name, which);

	while (left != TRUE){				//while no left possession of chopstick...

	    if (stickAvail[which]== TRUE){		//if chopstick is available...
	    	stickAvail[which] = FALSE;		//pick it up, chopstick no longer available
	    	left = TRUE;				//there is left possession
	    	printf("%s successfully picks up chopstick %d on his left\n", name, which);
	    }else {
		printf("%s was unsuccessful picking up chopstick %d\n", name, which);  
	        currentThread->Yield();		//if unable to pick, remain in loop, yield to next thread
	    }
	}

    	printf("%s is attempting to pick up chopstick %d on his right\n", name, (which+1)%totPhil);
	while (right != TRUE){				//while no right possession of chopstick...
	    if (stickAvail[(which + 1)%totPhil]== TRUE){	//if chopstick is available...
	    	stickAvail[(which+1)%totPhil] = FALSE;	//pick it up, chopstick no longer available
	    	right = TRUE;				//there is right possession
	    	printf("%s successfully picks up chopstick %d on his right\n", name, (which+1)%totPhil);
	    }else {
		printf("%s was unsuccessful picking up chopstick %d\n", name, (which+1)%totPhil);  
	        currentThread->Yield();		//if unable to pick, remain in loop, yield to next thread
            }
	}

        int randVal = Random()%4 +2;		//generate random variable

	if (totMeal >0){				//if a meal is available...
	    printf("%s is starting to eat\n", name);	//eat
	    totMeal--;					//reduce by 1
	    printf("meal left: %d\n", totMeal);		
	    for(int j = 0; j < randVal; j++)		
	    	currentThread->Yield();			//eating
	    printf("%s has finished eating\n", name);

	    
	}else printf("the food is finished :(\n");   //print if there's attempt to eat and no food available

	printf("%s is putting chopstick %d down\n", name, which);
	stickAvail[which] = TRUE;				//make left chopstick available again
	left = FALSE;						//left hand no longer possesses chopstick
	printf("%s is putting chopstick %d down\n", name, (which+1)%totPhil);
	stickAvail[(which+1)%totPhil] = TRUE;			//make right chopstick available again
	right = FALSE;						//right hand no longer possesses chopstick

	printf("%s is starting to think\n", name);		//start to think
	randVal = Random()%4 +2;				//generate random value between 2 and 5
	for(int j = 0; j < randVal; j++)
	    currentThread->Yield();				//thinking
	printf("%s is done thinking\n", name);
	
    }
    while(finished != totPhil){			//while some threads may still be in upper while loop...
        if(imDone != TRUE){			//if out of while loop set imDone to true...
	    imDone = TRUE;
            finished++;				//increment the amount of done threads
        }
        currentThread->Yield();   		//yield to next thread
    }						//everyone leaves the one after each other
    printf("%s leaves the table\n", name);
    currentThread->Yield();     
    currentThread->Finish();
}
///End of code edit by Toks Ipaye

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------
void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//Begin code changes by Hoang Pham
void
CheckInput(int which)
{
	char input[100];
	printf("\nPlease enter your input: ");
	gets(input);
	printf("\nYou just entered: %s\n", input);
	bool isDigit = true;
//	bool isNeg = false;
	bool hasDot = false;
	bool isChar = false;
	
	for(int i = 1 ; input[i] != '\0'; i++){
		char temps[1];
		temps[0] = input[i];
		//printf("\n%d",atoi(temps));
		int temp = atoi(temps);

		if(temp==0){
			if(input[i] == '.' && !hasDot)
				hasDot = true;
			else if(input[i] == '0')
				continue;
			else{
				isDigit = false;
				isChar = true;
				break;
			} 
		}
		else {
			//isDigit = true;
			continue;
		}

	}
	char *sign;
	char atemp[1];
	atemp[0] = input[0];
	if(input[0] == '-'){
		sign = "negative";
	}
	else if(atoi(atemp) != 0 || input[0] == '0'){
		sign = "positive";
		isDigit = true;	
	}
	else
		isChar = true;

	if(isChar)
		printf("\nYour input is just characters");
	else if(isDigit){
		if(!hasDot)
			printf("\nYour input is an %s integer", sign);
		else 
			printf("\nYour input is a %s decimal", sign);
	}	
	 
	printf("\n");
}

void 
ShoutOutLoud(int which)
{
	const char *a[8];
	a[0] = "We are watching the Packers and Falcons game";
	a[1] = "We are drinking rasperry sprite";
	a[2] = "This is a shouting";
	a[3] = "I like to talk to you";
	a[4] = "This girl likes my dog";
	a[5] = "Such a inspiration";
	a[6] = "He is a billionaire nerd";
	a[7] = "I will drive to visit you";
	
	int i = 0;
	int time = 1;
	
	while(i < which){
		int index = Random()%8;
		printf("%s said %s, and has shouted %d time(s)\n",currentThread->getName(),a[index],time);
		for(int j = 0; j < (Random()%5+2); j++)
			currentThread->Yield();	
		time++;
		i++;
	}
}	

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
	
    DEBUG('t', "Entering ThreadTest");
	if(CMD == 1){ 
		//printf("hola and CMD = %d", CMD);
		Thread *t = new Thread("forked thread");
    		t->Fork(CheckInput,1);
	}
	else if(CMD ==2){
		//printf("Bonjour and CMD = %d",CMD);
		printf("\nPlease enter number of threads: ");
		char num[10];
		gets(num);
		while(atoi(num)==0){
			printf("\nIncorrect Input\nPlease enter number of threads: ");
			gets(num);
		}
		printf("\nPlease enter number of shout for each thread: ");
		char shout[10];
		gets(shout);
		while(atoi(shout)==0){
			printf("\nIncorrect Input\nPlease enter number of shout for each thread: ");
			gets(shout);
		}
		Thread *thread;// = new Thread("shouted thread");
	
		int num1 = atoi(num);
		int shout1 = atoi(shout);
		int ii = 1;
		char * which;
		if(num1 && shout1){
		//printf("%d %d",num1,shout1);
			for(int i = 1; i <= num1; i++){
				//printf("%s\n",a[i]);
				which = new char[10];
				sprintf(which, "Thread %d",ii);
				ii++;
		//		printf("%s\n",which);
				thread = new Thread(which);	
				thread->Fork(ShoutOutLoud,shout1);
			}
		}
		else{
			printf("\nYou have entered invalid value for number of thread or/and number of shouting each thread");
		}	

	}
//toks Ipaye code edit starts
	else if(CMD == 3 || CMD == 4 ){
		printf("\nPlease enter number of Philosophers: ");
		char phils[10];
		gets(phils);						//get number of philosophers
		while(atoi(phils)==0){					//check input
			printf("\nIncorrect Input\nPlease enter number of Philosophers: ");//error & try again
			gets(phils);					
		}
		printf("\nPlease enter number of meals: ");
		char meals[10];
		gets(meals);						//get number of meals
		while(atoi(meals)==0){					//check input
			printf("\nIncorrect Input\nPlease enter number of meals: ");	//error & try again
			gets(meals);
		}
		Thread *thread;				// = new Thread("shouted thread");
	
		int phil = atoi(phils);			//convert string to int
		int meal = atoi(meals);
		char * which;				//variable for philospher identification
		char* what;				//variable for chopstick identification
		totPhil = phil;				//setting number of total philosphers to global value
		totMeal = meal;				//setting number of total meals to global value
		for (int i = 0; i<phil; i++)
			stickAvail[i] = 1;   //initializing chopsticks as available
		if(phil && meal){
			if (CMD == 3){				//Task 3
				for(int i = 0; i < phil; i++){
					which = new char[15];
					sprintf(which, "Philosopher %d",i);//saving string into variable "which"
				
					printf("%s enters the room\n",which);
					thread = new Thread(which);	
					thread->Fork(DinePhil,i);
				}
			}else if(CMD == 4){
				for(int i = 0; i < phil; i++){
					which = new char[15];
					what = new char[14];
					sprintf(which, "Philosopher %d",i);
					sprintf(what, " Chopstick %d", i);
					semaphore[i] = new Semaphore(what, 1);
					printf("%s enters the room\n",which);
					thread = new Thread(which);	
					thread->Fork(DinePhil2,i);
				}
			}
		}
		else{
			printf("\nYou have entered invalid value for number of thread or/and number of shouting each thread");
		}
	}
///toks Ipaye code edits end
}
//End code changes by Hoang Pham
