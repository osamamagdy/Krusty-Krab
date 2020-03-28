#include <cstdlib>
#include <time.h>
#include <iostream>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancellationEvent.h" 
#include "..\Events\PromotionEvent.h" 
#include"Cook.h"
#include "Order.h"
Restaurant::Restaurant() 
{
	pGUI = NULL;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();

	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		break;
	case MODE_STEP:
		break;
	case MODE_SLNT:
		break;
	case MODE_DEMO:
		Just_A_Demo();

	};

}

void Restaurant::loadfile()
{
	int Ncook, Gcook, Vcook, Nspeed, Gspeed, Vspeed, OrdersToBreak, NBreak, Gbreak, Vbreak, AutoP, EventsNum;
	char EventType;
	char OTYP; int TS, ID, SIZE;
	ORD_TYPE TYP;
	double MONY;
	
	ifstream Input;
	Input.open("Input.txt");

	Input >> Ncook >> Gcook >> Vcook;
	Input >> Nspeed >> Gspeed >> Vspeed;
	Input >> OrdersToBreak >> NBreak >> Gbreak >> Vbreak;
	Input >> AutoP;
	Input >> EventsNum;
	
	//creating Ncooks
	Cook* newNcooks = new Cook[Ncook];
	//creating Gcooks
	Cook* newGcooks = new Cook[Gcook];
	//creating Vcooks
	Cook* newVcook = new Cook[Vcook];

	//setting static data type  order to break 
	Cook::setordertobreak(OrdersToBreak);
	//setting static data type autopormotion
	Order::setautopormotion(AutoP);
	//setting data for normal cooks and enqueue them
	for (int i = 0; i < Ncook; i++)
	{
		newNcooks[i].setSpeed[Nspeed];
		newNcooks[i].setType[TYPE_NRM];
		newNcooks[i].setBreakduration(NBreak);
		//we should ask for bonous that every cook has different periorty depend on different speed
		Ncooks.enqueue(&newNcooks[i], Nspeed);
	}
	//setting data for vegan cooks and enqueue them
	for (int i = 0; i < Gcook; i++)
	{
		newGcooks[i].setSpeed[Gspeed];
		newGcooks[i].setType[TYPE_VGAN];
		newGcooks[i].setBreakduration(Gbreak);
		//we should ask for bonous that every cook has different periorty depend on different speed
		Gcooks.enqueue(&newGcooks[i], Gspeed);
	}
	//setting data for VIP cooks and enqueue them
	for (int i = 0; i < Vcook; i++)
	{
		newVcook[i].setSpeed[Vspeed];
		newVcook[i].setType[TYPE_VIP];
		newVcook[i].setBreakduration(Vbreak);
		//we should ask for bonous that every cook has different periorty depend on different speed
		Vcooks.enqueue(&newVcook[i], Vspeed);
	}
	//reading events lines and set its data the enqueue them in events queue

	//pointer to base class event 
	Event* newevent;

	for (int i = 0; i < EventsNum; i++)
	{
	   Input >> EventType;

	   switch (EventType)
	   {
		   //arrival event
	   case 'R':
	   {
		   Input >> OTYP >> TS >> ID >> SIZE >> MONY;
		   switch (OTYP)
		   {
		   case 'N':
			   TYP = TYPE_NRM;
			   break;
		   case 'G':
			   TYP = TYPE_VGAN;
			   break;
		   case 'V':
			   TYP = TYPE_VIP;
			   break;
		   }
		   newevent = new ArrivalEvent(TS, ID, TYP, MONY, SIZE);
		   break;
	   }
	   //cancellation event
	   case 'X':
	   {
		   Input >> TS >> ID;
		   newevent = new CancellationEvent(TS, ID);
		   break;
	   }
	   //Promotion event
	   case 'P':
	   {
		   Input >> TS >> ID >> MONY;
		   newevent = new PromotionEvent (TS,ID,MONY);
		   break;
	   }

	   }
	   EventsQueue.enqueue(newevent);
	}
	Input.close();

}

//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event *pE;
	while( EventsQueue.peekFront(pE) )	//as long as there are more events
	{
		if(pE->getEventTime() > CurrentTimeStep )	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}


Restaurant::~Restaurant()
{
		if (pGUI)
			delete pGUI;
}

void Restaurant::FillDrawingList()
{
	//This function should be implemented in phase1
	//It should add ALL orders and Cooks to the drawing list
	//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)
	//To add orders it should call function  void GUI::AddToDrawingList(Order* pOrd);
	//To add Cooks it should call function  void GUI::AddToDrawingList(Cook* pCc);

}




//////////////////////////////////////////////////////////////////////////////////////////////
/// ==> 
///  DEMO-related functions. Should be removed in phases 1&2

//Begin of DEMO-related functions

//This is just a demo function for project introductory phase
//It should be removed starting phase 1
void Restaurant::Just_A_Demo()
{
	
	//
	// THIS IS JUST A DEMO FUNCTION
	// IT SHOULD BE REMOVED IN PHASE 1 AND PHASE 2
	
	int EventCnt;	
	Order* pOrd;
	Event* pEv;
	srand(time(NULL));

	pGUI->PrintMessage("Just a Demo. Enter EVENTS Count(next phases should read I/P filename):");
	EventCnt = atoi(pGUI->GetString().c_str());	//get user input as a string then convert to integer

	pGUI->PrintMessage("Generating Events randomly... In next phases, Events should be loaded from a file...CLICK to continue");
	pGUI->waitForClick();
		
	//Just for sake of demo, generate some cooks and add them to the drawing list
	//In next phases, Cooks info should be loaded from input file
	int C_count = 12;	
	Cook *pC = new Cook[C_count];
	int cID = 1;

	for(int i=0; i<C_count; i++)
	{
		cID+= (rand()%15+1);	
		pC[i].setID(cID);
		pC[i].setType((ORD_TYPE)(rand()%TYPE_CNT));
	}	

		
	int EvTime = 0;

	int O_id = 1;
	
	//Create Random events and fill them into EventsQueue
	//All generated event will be "ArrivalEvents" for the demo
	for(int i=0; i<EventCnt; i++)
	{
		O_id += (rand()%4+1);		
		int OType = rand()%TYPE_CNT;	//Randomize order type		
		EvTime += (rand()%5+1);			//Randomize event time
		pEv = new ArrivalEvent(EvTime,O_id,(ORD_TYPE)OType);
		EventsQueue.enqueue(pEv);

	}	

	// --->   In next phases, no random generation is done
	// --->       EventsQueue should be filled from actual events loaded from input file

	
	
	
	
	//Now We have filled EventsQueue (randomly)
	int CurrentTimeStep = 1;
	

	//as long as events queue is not empty yet
	while(!EventsQueue.isEmpty())
	{
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep,timestep,10);	
		pGUI->PrintMessage(timestep);


		//The next line may add new orders to the DEMO_Queue
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		

/////////////////////////////////////////////////////////////////////////////////////////
		/// The next code section should be done through function "FillDrawingList()" once you
		/// decide the appropriate list type for Orders and Cooks
		
		//Let's add ALL randomly generated Cooks to GUI::DrawingList
		for(int i=0; i<C_count; i++)
			pGUI->AddToDrawingList(&pC[i]);
		
		//Let's add ALL randomly generated Ordes to GUI::DrawingList
		int size = 0;
		Order** Demo_Orders_Array = DEMO_Queue.toArray(size);
		
		for(int i=0; i<size; i++)
		{
			pOrd = Demo_Orders_Array[i];
			pGUI->AddToDrawingList(pOrd);
		}
/////////////////////////////////////////////////////////////////////////////////////////

		pGUI->UpdateInterface();
		Sleep(1000);
		CurrentTimeStep++;	//advance timestep
		pGUI->ResetDrawingList();
	}

	delete []pC;


	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();

	
}
////////////////

void Restaurant::AddtoDemoQueue(Order *pOrd)
{
	DEMO_Queue.enqueue(pOrd);
}

/// ==> end of DEMO-related function
//////////////////////////////////////////////////////////////////////////////////////////////


