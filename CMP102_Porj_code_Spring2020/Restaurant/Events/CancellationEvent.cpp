#include "CancellationEvent.h"
#include "..\Rest\Restaurant.h"


CancellationEvent::CancellationEvent(int eTime, int oID) :Event(eTime, oID)
{
	
}


void CancellationEvent::Execute(Restaurant* pRest)
{
      //This function is to cancel the normal orders only 

	//To Do ::

	// First check that the order type through the passed ID if it is not Normal
	// If it is normal , search for it in the waiting orders only using the passed ID and delete it 
	// you should use the Restaurant pointer



}