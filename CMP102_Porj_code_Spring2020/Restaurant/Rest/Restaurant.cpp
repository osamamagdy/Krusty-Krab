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
		//Just_A_Demo();
		loadfile();
		ExecuteEvents(30);
		FillDrawingList();
	

	};

}

void Restaurant::loadfile()
{
	int Ncook, Gcook, Vcook, NspeedMin, NspeedMax,GspeedMin, GspeedMax, VspeedMin, VspeedMax, OrdersToBreak, NBreakMin,NBreakMax, GbreakMin,GbreakMax, VbreakMin, VbreakMax,AutoP, EventsNum;
	char EventType;
	char OTYP; int TS, ID, SIZE;
	ORD_TYPE TYP;
	double MONY;
	int  RstPrd, VIP_WT;
	float InjProp ;
	ifstream Input;
	Input.open("Input.txt");

	Input >> Ncook >> Gcook >> Vcook;
	Input >> NspeedMin >> NspeedMax >> GspeedMin >> GspeedMax >> VspeedMin >> VspeedMax;
	Input >> OrdersToBreak >> NBreakMin >> NBreakMax >> GbreakMin>> GbreakMax >>VbreakMin>> VbreakMax;
	Input >> InjProp >> RstPrd;
	Input >> AutoP >> VIP_WT;
	Input >> EventsNum;

	
	//creating Ncooks
	Cook* newNcooks = new Cook[Ncook];
	//creating Gcooks
	Cook* newGcooks = new Cook[Gcook];
	//creating Vcooks
	Cook* newVcook = new Cook[Vcook];

	//setting static data type order to break 
	Cook::setordertobreak(OrdersToBreak);
	//setting static data type autopormotion
	Order::setautopormotion(AutoP);
	//setting data for normal cooks and enqueue them
	for (int i = 0; i < Ncook; i++)
	{
		newNcooks[i].setMinSpeed(NspeedMin);
		newNcooks[i].SetRstPrd(RstPrd);
		newNcooks[i].SetInjProp(InjProp);
		newNcooks[i].setMaxSpeed(NspeedMax);
		newNcooks[i].setType(TYPE_NRM);
		newNcooks[i].setMinBreakduration(NBreakMin);
		newNcooks[i].setMaxBreakduration(NBreakMax);
		newNcooks[i].setID(i + 1);
		//we should ask for bonous that every cook has different periorty depend on different speed
		Ncooks.enqueue(&newNcooks[i], NspeedMax);
	}
	//setting data for vegan cooks and enqueue them
	for (int i = 0; i < Gcook; i++)
	{
		newGcooks[i].setMinSpeed(GspeedMin);
		newGcooks[i].SetRstPrd(RstPrd);
		newGcooks[i].SetInjProp(InjProp);
		newGcooks[i].setMaxSpeed(GspeedMax);
		newGcooks[i].setType(TYPE_VGAN);
		newGcooks[i].setMinBreakduration(GbreakMin);
		newGcooks[i].setMaxBreakduration(GbreakMax);
		newGcooks[i].setID(i + 1);
		//we should ask for bonous that every cook has different periorty depend on different speed
		Gcooks.enqueue(&newGcooks[i], GspeedMax);
	}
	//setting data for VIP cooks and enqueue them
	for (int i = 0; i < Vcook; i++)
	{
		newVcook[i].setMinSpeed(VspeedMin);
		newVcook[i].setMaxSpeed(VspeedMax);
		newVcook[i].SetRstPrd(RstPrd);
		newVcook[i].SetInjProp(InjProp);
		newVcook[i].setType(TYPE_VIP);
		newVcook[i].setMinBreakduration(VbreakMin);
		newVcook[i].setMaxBreakduration(VbreakMax);
		newVcook[i].setID(i + 1);
		//we should ask for bonous that every cook has different periorty depend on different speed
		Vcooks.enqueue(&newVcook[i], VspeedMax);
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
		   newevent = new ArrivalEvent(TS, ID, TYP, MONY, SIZE, VIP_WT);
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
	Order* orderptr;
	Cook* cookptr;
	//orders copy 
	PQueue<Order*> Vorderscopy;	
    Queue<Order*> Norderscopy;  
	Queue<Order*> Gorderscopy;
	//cooks copy
	PQueue<Cook*> Vcookscopy;   
	PQueue<Cook*> Ncookscopy;   
	PQueue<Cook*> Gcookscopy;  
	//Drawing orders using add to drawing list and copy it 
	while (Vorders.dequeue(orderptr))
	{
		pGUI->AddToDrawingList(orderptr);
		Vorderscopy.enqueue(orderptr, orderptr->getPriority());

	}
	while (Norders.dequeue(orderptr))
	{
		pGUI->AddToDrawingList(orderptr);
		Norderscopy.enqueue(orderptr);

	}
	while (Gorders.dequeue(orderptr))
	{
		pGUI->AddToDrawingList(orderptr);
		Gorderscopy.enqueue(orderptr);

	}
	//filling orders with copied data 
	while (Vorderscopy.dequeue(orderptr))
	{
		Vorders.enqueue(orderptr, orderptr->getPriority());
	}
	while (Norderscopy.dequeue(orderptr))
	{
		Norders.enqueue(orderptr);
	}
	while (Gorderscopy.dequeue(orderptr))
	{
		Gorders.enqueue(orderptr);
	}
	//Drawing cooks using add to drawing list and copy it 
	while (Vcooks.dequeue(cookptr))
	{
		pGUI->AddToDrawingList(cookptr);
		Vcookscopy.enqueue(cookptr, cookptr->getMaxspeed());
	}
	while (Ncooks.dequeue(cookptr))
	{
		pGUI->AddToDrawingList(cookptr);
		Ncookscopy.enqueue(cookptr, cookptr->getMaxspeed());
	}
	while (Gcooks.dequeue(cookptr))
	{
		pGUI->AddToDrawingList(cookptr);
		Gcookscopy.enqueue(cookptr, cookptr->getMaxspeed());
	}
	//filling cooks with copied data 
	while (Vcookscopy.dequeue(cookptr))
	{
		Vcooks.enqueue(cookptr, cookptr->getMaxspeed());
	}
	while (Ncookscopy.dequeue(cookptr))
	{
		Ncooks.enqueue(cookptr, cookptr->getMaxspeed());
	}
	while (Gcookscopy.dequeue(cookptr))
	{
		Gcooks.enqueue(cookptr, cookptr->getMaxspeed());
	}
	//update interface 
	pGUI->UpdateInterface();
	pGUI->ResetDrawingList();
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
	
//	int EventCnt;	
//	Order* pOrd;
//	Event* pEv;
//	srand(time(NULL));
//
//	pGUI->PrintMessage("Just a Demo. Enter EVENTS Count(next phases should read I/P filename):");
//	EventCnt = atoi(pGUI->GetString().c_str());	//get user input as a string then convert to integer
//
//	pGUI->PrintMessage("Generating Events randomly... In next phases, Events should be loaded from a file...CLICK to continue");
//	pGUI->waitForClick();
//		
//	//Just for sake of demo, generate some cooks and add them to the drawing list
//	//In next phases, Cooks info should be loaded from input file
//	int C_count = 12;	
//	Cook *pC = new Cook[C_count];
//	int cID = 1;
//
//	for(int i=0; i<C_count; i++)
//	{
//		cID+= (rand()%15+1);	
//		pC[i].setID(cID);
//		pC[i].setType((ORD_TYPE)(rand()%TYPE_CNT));
//	}	
//
//		
//	int EvTime = 0;
//
//	int O_id = 1;
//	
//	//Create Random events and fill them into EventsQueue
//	//All generated event will be "ArrivalEvents" for the demo
//	for(int i=0; i<EventCnt; i++)
//	{
//		O_id += (rand()%4+1);		
//		int OType = rand()%TYPE_CNT;	//Randomize order type		
//		EvTime += (rand()%5+1);			//Randomize event time
//		pEv = new ArrivalEvent(EvTime,O_id,(ORD_TYPE)OType);
//		EventsQueue.enqueue(pEv);
//
//	}	
//
//	// --->   In next phases, no random generation is done
//	// --->       EventsQueue should be filled from actual events loaded from input file
//
//	
//	
//	
//	
//	//Now We have filled EventsQueue (randomly)
//	int CurrentTimeStep = 1;
//	
//
//	//as long as events queue is not empty yet
//	while(!EventsQueue.isEmpty())
//	{
//		//print current timestep
//		char timestep[10];
//		itoa(CurrentTimeStep,timestep,10);	
//		pGUI->PrintMessage(timestep);
//
//
//		//The next line may add new orders to the DEMO_Queue
//		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
//		
//
///////////////////////////////////////////////////////////////////////////////////////////
//		/// The next code section should be done through function "FillDrawingList()" once you
//		/// decide the appropriate list type for Orders and Cooks
//		
//		//Let's add ALL randomly generated Cooks to GUI::DrawingList
//		for(int i=0; i<C_count; i++)
//			pGUI->AddToDrawingList(&pC[i]);
//		
//		//Let's add ALL randomly generated Ordes to GUI::DrawingList
//		int size = 0;
//		//Order** Demo_Orders_Array = DEMO_Queue.toArray(size);
//		
//		for(int i=0; i<size; i++)
//		{
//			pOrd = Demo_Orders_Array[i];
//			pGUI->AddToDrawingList(pOrd);
//		}
///////////////////////////////////////////////////////////////////////////////////////////
//
//		pGUI->UpdateInterface();
//		Sleep(1000);
//		CurrentTimeStep++;	//advance timestep
//		pGUI->ResetDrawingList();
//	}
//
//	delete []pC;
//
//
//	pGUI->PrintMessage("generation done, click to END program");
//	pGUI->waitForClick();

	
}
////////////////

//void Restaurant::AddtoDemoQueue(Order *pOrd)
//{
//	DEMO_Queue.enqueue(pOrd);
//}

/// ==> end of DEMO-related function
//////////////////////////////////////////////////////////////////////////////////////////////
void Restaurant::AddOrders(Order* po)
{
	if (po->GetType() == TYPE_NRM)
	{
		Norders.enqueue(po);
		//add order for norma;
	}
	else if (po->GetType() == TYPE_VGAN)
	{
		Gorders.enqueue(po);
		//add order for vegan
	}
	else if (po->GetType() == TYPE_VIP)
	{
		po->CalPriority();
		Vorders.enqueue(po,po->getPriority());
		//add order for Vip
	}

}
void Restaurant::Seacrh(int Time, int ID,Order *& frntEntry)
{
	Queue<Order*> qtemp;
	Order* Otemp;
	//search about order
	while (Norders.dequeue(Otemp))
	{
		if (Otemp->GetID() != ID)
			qtemp.enqueue(Otemp);
		else
			frntEntry = Otemp;
		
	}

	while (qtemp.dequeue(Otemp))
		Norders.enqueue(Otemp);
}

