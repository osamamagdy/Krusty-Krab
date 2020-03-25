#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType):Event(eTime, oID)
{
	OrdType = oType;
}  // Didn't need it anyway but refused to delete it





ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int distance, double money, int size) :Event(eTime, oID)
{
	OrdType = oType;
	OrdDistance = distance;
	OrdMoney = money;
	OrderSize = size;
}


void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create an order and fills its data 
	// Then adds it to normal, vegan, or VIP order lists that you will create in phase1
	Order* pOrd = new Order(OrderID,OrdType,OrdMoney,OrderSize,EventTime);
        pRest->AddOrders(pOrd);
}
