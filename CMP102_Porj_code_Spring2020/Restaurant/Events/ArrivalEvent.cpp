#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType):Event(eTime, oID)
{
	OrdType = oType;
}  // Didn't need it anyway but refused to delete it





ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, double money, int size) :Event(eTime, oID)
{
	OrdType = oType;
	OrdMoney = money>0 ? money :0;
	OrderSize = size>0 ?size:0;
}


void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create an order and fills its data 
	// Then adds it to normal, vegan, or VIP order lists that you will create in phase1
	Order* pOrd = new Order(OrderID,OrdType,OrdMoney,OrderSize,EventTime);
	//add orders
        pRest->AddOrders(pOrd);
}
