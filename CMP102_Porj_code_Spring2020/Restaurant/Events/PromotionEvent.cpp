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
    Order* nptr = nullptr;
    pRest->Seacrh(EventTime, OrderID, nptr);
    if (nptr)
    {
        nptr->AddMoney(ExMoney);
        //increase money
        nptr->SetType(TYPE_VIP);
        //change the type
        if (ExMoney == 0 && nptr->GetAUto() > nptr->GetIncrese()) //auto promtion
        {
            nptr->increase_promotion();
            pRest->AddOrders(nptr);
        }
        else if (ExMoney > 0)
        {
            pRest->AddOrders(nptr);
        }
        else
        {
            delete nptr;
            nptr = NULL;
        }
        //add to VIP orders
    }
  // First check that the order type through the passed ID if it is not Normal
  // If it is normal , search for it in the waiting orders only using the passed ID
  // Create a pointer to order
  // If found, change it's type to VIP and add the extra money to it's price (usefunction "AddMoney" in "Order" class)
  // you should use the Restaurant pointer



}