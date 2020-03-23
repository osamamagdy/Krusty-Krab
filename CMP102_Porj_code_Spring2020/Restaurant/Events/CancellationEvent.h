#pragma once
#include "Event.h"
class CancellationEvent : public Event  //class for the cancellation event
{

public:
	CancellationEvent(int eTime, int oID);

	virtual void Execute(Restaurant* pRest);	//override execute function




};



