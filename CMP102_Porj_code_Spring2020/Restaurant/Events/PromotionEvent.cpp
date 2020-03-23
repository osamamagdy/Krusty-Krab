#include "PromotionEvent.h"
#include "..\Rest\Restaurant.h"




PromotionEvent::PromotionEvent(int eTime, int oID, double extra) :Event(eTime, oID)
{
    ExMoney = extra;
}


void PromotionEvent::Execute(Restaurant* pRest)
{
	//This function is to promote the normal orders only to become vip

  //To Do ::

  // First check that the order type through the passed ID if it is not Normal
  // If it is normal , search for it in the waiting orders only using the passed ID
  // Create a pointer to order
  // If found, change it's type to VIP and add the extra money to it's price (usefunction "AddMoney" in "Order" class)
  // you should use the Restaurant pointer



}