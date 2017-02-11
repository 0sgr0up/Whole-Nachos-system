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



Semaphore** semaphore;	//semaphore array for chopsticks
bool* stickAvail;	//array of chopsticks
int totPeep;		//the total amount of philosophers/people
int totMeal;		//the total amount of meals
int finished = 0;    //the philosophers done eating
int entered = 0;	//check to for entering
int readyToEat = 0;	//check for seated
int hereCheck = 0;	//check for accessing shared resource meal


int** mailBoxes;
int totMess;

/*
void
ReadMessage(int which)
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

    while(totMess>0){
    	printf("Person %d reads message %s", which, mailbox[which][])
    	currentThread->Yield();
    }
}

void
PostOffice( int which)
{

    char* name = currentThread->getName();	//the name of the current thread
    entered++;
    Entered(name);
    ReadMessage(which);


}
*/
void
Entered(char* name)
{
 
    while(entered != totPeep){				//while somethreads are not here yet
    	currentThread->Yield();			//yield for next philosopher to sit
    }
    switch(CMD){				
    	case 3:
	case 4: printf("%s sits at the table\n", name);	//the philosopher sits at table
		break;
	case 5: printf("%s enters the postoffice\n", name); //person enters postoffice
    }
}

void
ReadyToEat()
{
 
    while(readyToEat != totPeep){				//while somethreads are not here yet
    	currentThread->Yield();			//yield for next philosopher to sit
    }				

}

void
Pick(char* name, char lOr, int whichHand, bool* possession)
{
     char* hand;

    if (lOr == 'l')					//sets string for whether left or right
        hand = "left";
    else if (lOr == 'r')
        hand = "right";

    	printf("%s is attempting to pick up chopstick %d on his %s\n", name, whichHand, hand);
	if(totPeep>1 || lOr != 'r'){
	    while (*possession != TRUE){				//while no possession of chopstick...
	    	if (stickAvail[whichHand]== TRUE){	//if chopstick is available...

	    	    stickAvail[whichHand] = FALSE;	//pick it up, chopstick no longer available
	    	    *possession = TRUE;
						//there is possession
	    	    printf("%s successfully picks up chopstick %d on his %s\n", name, whichHand, hand);
	    	}else {
		    printf("%s was unsuccessful picking up chopstick %d\n", name, whichHand);  
	            currentThread->Yield();		//if unable to pick, remain in loop, yield to next thread
                }
	    }
        }else printf("%s successfully picks up chopstick %d on his %s\n", name, whichHand, hand); //for the case of one philosopher and right hand
}
void
PickS(char* name, char lOr, int whichHand)				//pick function for Semaphores
{
    char* hand;								//string for what hand

    if (lOr == 'l')
        hand = "left";							//setting hand
    else if (lOr == 'r')
        hand = "right";
    printf("%s is attempting to pick up chopstick %d on his %s\n", name, whichHand, hand);
    if (totPeep>1 || lOr != 'r')
    	(semaphore[whichHand])->P();		//for chopstick on right
    printf("%s successfully picks up chopstick %d on his %s\n", name, whichHand, hand);
}

void
LetsEat(char* name, int meals)
{
    int randVal = Random()%4 +2;		//generate random variable
    while (hereCheck>0){
	currentThread->Yield();		
    }
		//loop
    hereCheck++;
    if (totMeal >0){				//if a meal is available...
	totMeal--;
	printf("%s is starting to eat\n", name);	//eat
	    
	printf("meal(s) eaten: %d\nmeal(s) left: %d\n", (meals - totMeal), totMeal);	
	hereCheck--;	
	for(int j = 0; j < randVal; j++)		
	    currentThread->Yield();			//eating
	printf("%s has finished eating\n", name);

    }else {
	printf("%s can't eat because...\n the food is finished :(\n", name);//print if there's attempt to...
	hereCheck--;
    }	
										//...eat and no food available
}
void
Drop(char* name, char lOr, int whichHand, bool* possession)
{
    char* hand;

    if (lOr == 'l')
        hand = "left";
    else if (lOr == 'r')
        hand = "right";
	printf("%s is putting chopstick %d down\n", name, whichHand);
	if (totPeep>1 || lOr != 'r')
	    stickAvail[whichHand] = TRUE;			//make right chopstick available again
	*possession = FALSE;						//right hand no longer possesses chopstick


}

void
DropS(char* name, char lOr, int whichHand)
{
    char* hand;

    if (lOr == 'l')
        hand = "left";
    else if (lOr == 'r')
        hand = "right";

	printf("%s is putting chopstick %d down\n", name, whichHand);//putting chopstick down
	if (totPeep>1 || lOr != 'r')
	   (semaphore[whichHand])->V();				//putting chopstick down


}

void
Think(char* name)
{
	int randVal = Random()%4 +2; 			//generate random value between 2 and 5
	printf("%s is starting to think\n", name);		
					
	for(int j = 0; j < randVal; j++)			
	    currentThread->Yield();				//thinking 
	printf("%s is done thinking\n", name);
}

void
Finished()
{
 
    while(finished != totPeep){				//while somethreads are not here yet
    	currentThread->Yield();			//yield for next philosopher to sit
    }				

}

void
DinePhil2(int which)
{

    char* name = currentThread->getName();	//the name of the current thread
    int left = which;
    int right;
    int meals = totMeal;

    if (totPeep>1)
       right = (which +1)%totPeep;
    else  right = 1;

    entered++;
    Entered(name);

    readyToEat++;
    ReadyToEat();

    while (totMeal>0){									//while meals left

	PickS(name, 'l', left);
	PickS(name, 'r', right);


	LetsEat(name, meals);

	DropS(name, 'l', left);
	DropS(name, 'r', right);

	Think(name);


    }
    finished++;

    Finished();
    printf("%s leaves the table\n", name);
    currentThread->Yield();     
    currentThread->Finish();
}


void
DinePhil(int which)
{
    bool left=FALSE;			//boolean to indicate left hand possession of chopstick
    bool  right = FALSE;		//boolean to indicate right hand possession of chopstick
    int left1 = which;
    int right1;
    int meals = totMeal;
    char* name = currentThread->getName();	//name of current thread


    if (totPeep<=1)
	right1 = 1;
    else right1 = (which+1)%totPeep;

    entered++;
    Entered(name);

    readyToEat++;
    ReadyToEat();

    while (totMeal>0){				//while there are still meals...


	Pick(name, 'l', left1, &left);
	Pick(name, 'r', right1, &right);

	LetsEat(name, meals);							

	Drop(name, 'l', left1, &left);
	Drop(name, 'r', right1, &right);

	Think(name);
	
    }
    finished++;

    Finished();
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

bool
InputCheck(char* entry)
{

    char* temp = entry;
    if (atoi(temp) ==0)
	return FALSE;
    if (strlen(temp) < 1 || strlen(temp)>10)
	return FALSE;
    while( *temp != '\0'){
	if (*temp < '0' || *temp > '9'){
	    return FALSE;
	}
	temp++;
    }
	return TRUE;
}
void
getString(char* result)
{
   int strlength = 12;
 //  char result[strlength + 2];
 //  result = results;
 //  bool goodInput = FALSE;
//   while(goodInput == FALSE){
   	int index = 0;
	char c = ' ';
   	while( c != '\n' && index<=(strlength -2)){
   	    c = getchar();
	    *(result+index) = c;
	    if(c == '\n')
	    	*(result+index) = '\0';
    	
	    index++;
   	}

   	
   	if (index >= strlength - 1){
	*(result+index) = '\0';
	    while(c != '\n'){
	   	c = getchar();
	    }	
   	}else {
 //          goodInput = TRUE;
        }
		
  // }
 
}
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
		char num[12];
		getString(num);
		while(!InputCheck(num)){
			printf("\nInvalid Input\nPlease enter number of threads: ");
			getString(num);
		}
		printf("\nPlease enter number of shout for each thread: ");
		char shout[12];
		getString(shout);
		while(!InputCheck(shout)){
			printf("\nInvalid Input\nPlease enter number of shout for each thread: ");
			getString(shout);
		}
		Thread *thread;// = new Thread("shouted thread");
	
		int num1 = atoi(num);
		int shout1 = atoi(shout);
		char * which;
		if(num1 && shout1){
		//printf("%d %d",num1,shout1);
			for(int i = 1; i <= num1; i++){
				//printf("%s\n",a[i]);
				which = new char[10];
				sprintf(which, "Thread %d",i);
				i++;
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
		char phils[12];
		getString(phils);						//get number of philosophers
		while(!InputCheck(phils)){					//check input
			printf("\nInvalid Input\nPlease enter number of Philosophers: ");//error & try again
			getString(phils);					
		}
		printf("\nPlease enter number of meals: ");
		char meals[12];
		getString(meals);						//get number of meals
		while(!InputCheck(meals)){					//check input
			printf("\nInvalid Input\nPlease enter number of meals: ");	//error & try again
			getString(meals);
		}
		Thread *thread;				// = new Thread("shouted thread");
	
		int phil = atoi(phils);			//convert string to int
		int meal = atoi(meals);
		char * which;				//variable for philospher identification
		char* what;				//variable for chopstick identification
		totPeep = phil;				//setting number of total philosphers to global value
		totMeal = meal;				//setting number of total meals to global value


		if (CMD == 3){
		    stickAvail = new bool[phil];
		    for (int i = 0; i<phil; i++)
			stickAvail[i] = TRUE;   //initializing chopsticks as available
		}else if(CMD == 4){
		    semaphore = new Semaphore*[phil];
		}
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
			printf("\nYou have entered an invalid value");
		}
	}/*else if(CMD == 5 || CMD == 6 ){
		printf("\nPlease enter number of People: ");
		char people[10];
		getString(people);						//get number of philosophers
		while(!InputCheck(people)){					//check input
			printf("\nIncorrect Input\nPlease enter number of People: ");//error & try again
			getString(people);					
		}
		printf("\nPlease enter number for message capacity: ");
		char messCap[10];
		getString(messCap);						//get number of meals
		while(!InputCheck(messCap)){					//check input
			printf("\nIncorrect Input\nPlease enter number for message capacity: ");	//error & try again
			getString(messCap);
		}
		printf("\nPlease enter number of messages to be sent: ");
		char toBeSent[10];
		getString(toBeSent);						//get number of philosophers
		while(!InputCheck(toBeSent)){					//check input
			printf("\nIncorrect Input\nPlease enter number of messages to be sent: ");//error & try again
			getString(toBeSent);					
		}
		Thread *thread;// = new Thread("shouted thread");
	
		int people1 = atoi(people);
		int messCap1 = atoi(messCap);
		int toBeSent1 = atoi(toBeSent);
//		int ii = 1;
		totPeep = people1;
		totMess = toBeSent1;
		char * which;
		
		mailBoxes = new int[people1][messCap1];
		if(people1 && messCap1 && toBeSent1){
		//printf("%d %d",num1,shout1);
			for(int i = 0; i < people1; i++){
				//printf("%s\n",a[i]);
				which = new char[10];
				sprintf(which, "Thread %d",i);
				i++;
		//		printf("%s\n",which);
				for(int j = 0; j< messCap1; j++)
					mailBoxes[i][j] = 11;
				thread = new Thread(which);	
				thread->Fork(ShoutOutLoud,i);
			}
		}
		else{
			printf("\nYou have entered invalid value for number of thread or/and number of shouting each thread");
		}	

	}*/
///toks Ipaye code edits end
}
//End code changes by Hoang Pham
