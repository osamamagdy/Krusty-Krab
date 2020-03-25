#include "Event.h"


Event::Event(int eTime, int ordID)
{
	EventTime = eTime>0?eTime:0;
	OrderID	= ordID>0?ordID:0;
}


int Event::getEventTime()
{
	return EventTime;
}
int Event::getOrderID()
{
	return OrderID;
}


Event::~Event()
{

}

