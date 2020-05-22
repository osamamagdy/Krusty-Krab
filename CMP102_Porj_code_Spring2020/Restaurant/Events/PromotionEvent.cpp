#include "PromotionEvent.h"
#include "..\Rest\Restaurant.h"




PromotionEvent::PromotionEvent(int eTime, int oID, double extra) :Event(eTime, oID)
{
    ExMoney = extra>0?extra:0;
}


void PromotionEvent::Execute(Restaurant* pRest)
{
	//This function is to promote the normal orders only to become vip

  //To Do ::


    if (ExMoney <= 0)
        return;
    Order* nptr = nullptr;
    pRest->Seacrh( OrderID, nptr);
    if (nptr)
    {
        nptr->AddMoney(ExMoney);
        //increase money
        nptr->SetType(TYPE_VIP);
        //change the type
        pRest->AddOrders(nptr);
        //add to VIP orders
        Order::setNOrderscount(Order::getNordercount() - 1);

        Order::setVOrderscount(Order::getVordercount() + 1);


        nptr->set_time_when_became_VIP(EventTime);
        
        //if (ExMoney == 0) //auto promtion
        //{
        //    nptr->increase_promotion();
        //    pRest->AddOrders(nptr);
        //}
        //else
    }
  // First check that the order type through the passed ID if it is not Normal
  // If it is normal , search for it in the waiting orders only using the passed ID
  // Create a pointer to order
  // If found, change it's type to VIP and add the extra money to it's price (usefunction "AddMoney" in "Order" class)
  // you should use the Restaurant pointer



}