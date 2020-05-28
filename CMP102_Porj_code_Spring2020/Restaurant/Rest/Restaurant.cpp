#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include<Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
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
	loadfile();
	PROG_MODE	mode = pGUI->getGUIMode();

	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		Restaurant_modes(0);
		break;
	case MODE_STEP:
		Restaurant_modes(1);
		break;
	case MODE_SLNT:
		Restaurant_modes(2);
		break;


	};
	outputfile();

}

void Restaurant::Restaurant_modes(int mode)
{
	int timestep = 1;
	switch (mode)
	{
	case 0:
		pGUI->PrintMessage("Welcome To Craspy Crabs Restaurant in interactive Mode.... ");
		PlaySound(TEXT("new_cook.wav"), NULL, SND_SYNC);
		break;
	case 1:
		pGUI->PrintMessage("Welcome To Craspy Crabs Restaurant in step-by-step Mode.... ");
		PlaySound(TEXT("famous.wav"), NULL, SND_SYNC);
		break;
	case 2:
		pGUI->PrintMessage("Welcome To Craspy Crabs Restaurant in silent Mode....");
		PlaySound(TEXT("family.wav"), NULL, SND_SYNC);
		break;
	}
	while (!EventsQueue.isEmpty() || !Vorders.isEmpty() || !Norders.isEmpty() || !Gorders.isEmpty() || !prepare_Order.isEmpty())
	{

		string masg4 = "";
		bool Vflag = true;
		bool Gflag = true;
		bool Nflag = true;
		bool Urgentflag = true;
		ExecuteEvents(timestep);
		assignOrdertofinish(timestep);
		checkunavailblecooks(timestep);
		CheckurgentForVIP(timestep);
		autopormotedForNormal(timestep);
		while (!UrgentOrder.isEmpty() && Urgentflag)
		{
			Urgentflag=urgentForVIP(timestep, masg4);
		}
		while (UrgentOrder.isEmpty() &&!Vorders.isEmpty() && Vflag)
		{
			Vflag = assignOrderVIP(timestep, masg4);
		}
		while (UrgentOrder.isEmpty() && Vorders.isEmpty() && !Gorders.isEmpty() && Gflag)
		{
			Gflag = assignOrderVegan(timestep, masg4);
		}
		while (UrgentOrder.isEmpty() && Vorders.isEmpty() && !Norders.isEmpty() && Nflag)
		{
			Nflag = assignOrderNormal(timestep, masg4);
		}
		
		if (mode == 0 || mode == 1)
		{
			if (mode == 0)
			{
				pGUI->waitForClick();
			}
			else if (mode == 1)
			{
				Sleep(1000);
			}
			FillDrawingList();
			string masg1 = " TS: " + to_string(timestep);
			string masg2 = "Num of Waiting VIP-Order : " + to_string(Order::get_waiting_Vorder()) + "  Num of Waiting Norm-Order : " + to_string(Order::get_waiting_Norder()) + "  Num of Waiting Veg-Order : " + to_string(Order::get_waiting_Gorder());
			string masg3 = "Num of Available-VIP-Cooks : " + to_string(Cook::GetAvailableVcount()) + "  Num of Available-Norm-Cooks :  " + to_string(Cook::GetAvailableNcount()) + "  Num of Available-Veg-Cook :  " + to_string(Cook::GetAvailableGcount());
			string masg5 = "Num of Served VIP-Order : " + to_string(Order::get_Served_Vorder()) + "  Num of Served Norm-Order : " + to_string(Order::get_Served_Norder()) + "  Num of Served Veg-Order : " + to_string(Order::get_Served_Gorder());

			pGUI->PrintMessage(masg1, masg2, masg3, masg4, masg5);
		}


		timestep++;
	}
}


void Restaurant::autopormotedForNormal(int time)
{
	Queue<Order*> temp;
	Order* ptr;
	bool flag = true;
	while (!Norders.isEmpty() && flag)
	{
		Norders.peekFront(ptr);
		int waitTime = time - ptr->getorderarrivaltime();
		int prmotedTime = ptr->GetAUto();
		if (waitTime >= prmotedTime)
		{
			Norders.dequeue(ptr);
			ptr->SetType(TYPE_VIP);
			AddOrders(ptr);

			////////Number of auto promoted orders
			ptr->increase_promotion();


			///////Number of Orders
			Order::setNOrderscount(Order::getNordercount() - 1);
			
			Order::setVOrderscount(Order::getVordercount() + 1);

			ptr->set_time_when_became_VIP(time);


		}
		else
		{
			flag = false;
		}
	}

}
void Restaurant::CheckurgentForVIP(int timestep)
{

	Queue<Order*> temp;
	Order* ptr;
	while (!Vorders.isEmpty())
	{
		Vorders.peekFront(ptr);
		int waitTime = timestep - ptr->get_time_when_became_VIP();
		int urgentTime = ptr->getVIP_WT();
		if (waitTime > urgentTime)
		{
			Vorders.dequeue(ptr);
			UrgentOrder.enqueue(ptr);
		}
		else
			break;


	}

}
bool Restaurant::urgentForVIP(int timestep, string& msg)
{
	

	/////////////////Should be modified by three possible solutions to reduce complexity///////////////////


	/*
	1 - (Which we decided temporarly to make ) Make the priority Equation for each VIP order depends
	greetly on the arrival time. So, there is no possible way to consider an Order to be urgent
	unless it's at the front so we have complexity of one.


	2 - (Has More complexity but it's logical for programming concepts) We make our VIP Orders in a sorted linked list depending on the priority equation.
	We sure have to maintain the order of insertion at the same priority by insertend & deletebeg.
	But when it comes to Urgent orders we make a search for any waiting time that is greater than the VIP_WT.
	So, if we found an order like this we delete it from the list regardless of it's position



	3 - (The first solution to come in mind) Is to empty the whole Queue searching for any order that exceeded the VIP_WAIT
	and fill the remaining orders again



	while (!Vorders.isEmpty())
	{
		Vorders.dequeue(ptr);
		int waitTime = timestep - ptr->getorderarrivaltime();
		int urgentTime = ptr->getVIP_WT();
		if (waitTime >= urgentTime)
		{
			if (!assignOrderVIP(timestep))
			{
				if (!assignOrderBreak(timestep, ptr))
				{
					if (!assignOrderInjured(timestep, ptr))
						temp.enqueue(ptr);
				}
			}
		}
		else
			temp.enqueue(ptr);
	}
	while (!temp.isEmpty())
	{
		temp.dequeue(ptr);
		Vorders.enqueue(ptr, ptr->getPriority());
	}


	*/

	
	
	while (!UrgentOrder.isEmpty())
	{
		Order* ptr;
		UrgentOrder.peekFront(ptr);
		if (!Vcooks.isEmpty())
		{
			Order* the_order;
			Cook* the_cook;
			//dequeue the cook from cooks queue and dequeue the order from order queue
			Vcooks.dequeue(the_cook);
			UrgentOrder.dequeue(the_order);
			//set order new status and waiting time and calculate service time and assign to preprinng orders
			the_order->setStatus(SRV);
			the_order->setwaittime(timestep - the_order->getorderarrivaltime());
			the_order->setservicetime(the_order->getOrderSize() / the_cook->getspeed());
			the_order->CalFinish();
			prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
			//setting cook information 
			the_cook->setStatus(BUSY);
			the_cook->setServedOrder(the_order);
			the_cook->setTimesteptobeavailabale(the_order->getOrderSize() / the_cook->getspeed() + timestep);
			the_cook->CalUnavailabalePriority(/*timestep*/);
			BusyCooks.enqueue(the_cook, the_cook->getUnavailabalePriority());
			Cook::setAvailableVcount(Cook::GetAvailableVcount() - 1);
			Order::set_Served_Vorder(Order::get_Served_Vorder() + 1);
			msg += "  V" + to_string(the_cook->GetID()) + "(V" + to_string(the_order->GetID()) + ") ";
			
			Order::increase_urgent();
		}
		else if (!Ncooks.isEmpty())
		{
			Order* the_order;
			Cook* the_cook;
			Ncooks.dequeue(the_cook);
			UrgentOrder.dequeue(the_order);
			//set order new status and waiting time and calculate service time and assign to preprinng orders
			the_order->setStatus(SRV);
			the_order->setwaittime(timestep - the_order->getorderarrivaltime());
			the_order->setservicetime(the_order->getOrderSize() / the_cook->getspeed());
			the_order->CalFinish();
			prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
			//setting cook information 
			the_cook->setStatus(BUSY);
			the_cook->setServedOrder(the_order);
			the_cook->setTimesteptobeavailabale(the_order->getOrderSize() / the_cook->getspeed() + timestep);
			the_cook->CalUnavailabalePriority(/*timestep*/);
			BusyCooks.enqueue(the_cook, the_cook->getUnavailabalePriority());
			Cook::setAvailableNcount(Cook::GetAvailableNcount() - 1);
			Order::set_Served_Vorder(Order::get_Served_Vorder() + 1);
			msg += "  N" + to_string(the_cook->GetID()) + "(V" + to_string(the_order->GetID()) + ") ";
		
			Order::increase_urgent();
		}
		else if (!Gcooks.isEmpty())
		{
			Order* the_order;
			Cook* the_cook;
			Gcooks.dequeue(the_cook);
			UrgentOrder.dequeue(the_order);
			//set order new status and waiting time and calculate service time and assign to preprinng orders
			the_order->setStatus(SRV);
			the_order->setwaittime(timestep - the_order->getorderarrivaltime());
			the_order->setservicetime(the_order->getOrderSize() / the_cook->getspeed());
			the_order->CalFinish();
			prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
			//setting cook information 
			the_cook->setStatus(BUSY);
			the_cook->setServedOrder(the_order);
			the_cook->setTimesteptobeavailabale(the_order->getOrderSize() / the_cook->getspeed() + timestep);
			the_cook->CalUnavailabalePriority(/*timestep*/);
			BusyCooks.enqueue(the_cook, the_cook->getUnavailabalePriority());
			Cook::setAvailableGcount(Cook::GetAvailableGcount() - 1);
			Order::set_Served_Vorder(Order::get_Served_Vorder() + 1);
			msg += "  G" + to_string(the_cook->GetID()) + "(V" + to_string(the_order->GetID()) + ") ";
			
			Order::increase_urgent();
		}
		else if (assignOrderBreak(timestep, ptr, msg))
		{
			Order::increase_urgent();
		}
		else if (assignOrderInjured(timestep, ptr, msg))
	{
	    
			Order::increase_urgent();
	}
		else
		return false;
	

 
	}
	
		return true;
}


bool Restaurant::assignOrderVIP(int timestep, string& msg)
{
	if (!Vcooks.isEmpty())
	{
		Order* the_order;
		Cook* the_cook;
		//dequeue the cook from cooks queue and dequeue the order from order queue
		Vcooks.dequeue(the_cook);
		Vorders.dequeue(the_order);
		//set order new status and waiting time and calculate service time and assign to preprinng orders
		the_order->setStatus(SRV);
		the_order->setwaittime(timestep - the_order->getorderarrivaltime());
		the_order->setservicetime(the_order->getOrderSize() / the_cook->getspeed());
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cook information 
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getOrderSize() / the_cook->getspeed() + timestep);
		the_cook->CalUnavailabalePriority(/*timestep*/);
		BusyCooks.enqueue(the_cook, the_cook->getUnavailabalePriority());
		Cook::setAvailableVcount(Cook::GetAvailableVcount() - 1);
		Order::set_Served_Vorder(Order::get_Served_Vorder() + 1);
		msg += "  V" + to_string(the_cook->GetID()) + "(V" + to_string(the_order->GetID()) + ") ";
		return true;
	}
	else if (!Ncooks.isEmpty())
	{
		Order* the_order;
		Cook* the_cook;
		Ncooks.dequeue(the_cook);
		Vorders.dequeue(the_order);
		//set order new status and waiting time and calculate service time and assign to preprinng orders
		the_order->setStatus(SRV);
		the_order->setwaittime(timestep - the_order->getorderarrivaltime());
		the_order->setservicetime(the_order->getOrderSize() / the_cook->getspeed());
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cook information 
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getOrderSize() / the_cook->getspeed() + timestep);
		the_cook->CalUnavailabalePriority(/*timestep*/);
		BusyCooks.enqueue(the_cook, the_cook->getUnavailabalePriority());
		Cook::setAvailableNcount(Cook::GetAvailableNcount() - 1);
		Order::set_Served_Vorder(Order::get_Served_Vorder() + 1);
		msg += "  N" + to_string(the_cook->GetID()) + "(V" + to_string(the_order->GetID()) + ") ";
		return true;
	}
	else if (!Gcooks.isEmpty())
	{
		Order* the_order;
		Cook* the_cook;
		Gcooks.dequeue(the_cook);
		Vorders.dequeue(the_order);
		//set order new status and waiting time and calculate service time and assign to preprinng orders
		the_order->setStatus(SRV);
		the_order->setwaittime(timestep - the_order->getorderarrivaltime());
		the_order->setservicetime(the_order->getOrderSize() / the_cook->getspeed());
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cook information 
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getOrderSize() / the_cook->getspeed() + timestep);
		the_cook->CalUnavailabalePriority(/*timestep*/);
		BusyCooks.enqueue(the_cook, the_cook->getUnavailabalePriority());
		Cook::setAvailableGcount(Cook::GetAvailableGcount() - 1);
		Order::set_Served_Vorder(Order::get_Served_Vorder() + 1);
		msg += "  G" + to_string(the_cook->GetID()) + "(V" + to_string(the_order->GetID()) + ") ";
		return true;
	}
	return false;
}
bool Restaurant::assignOrderVegan(int timestep, string& msg)
{
	if (!Gcooks.isEmpty())
	{
		Order* the_order;
		Cook* the_cook;
		Gcooks.dequeue(the_cook);
		Gorders.dequeue(the_order);
		//setting order new status and waiting time and calculate service time and assign to preprinng orders
		the_order->setStatus(SRV);
		the_order->setwaittime(timestep - the_order->getorderarrivaltime());
		the_order->setservicetime(the_order->getOrderSize() / the_cook->getspeed());
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cooks informations
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getOrderSize() / the_cook->getspeed() + timestep);
		the_cook->CalUnavailabalePriority(/*timestep*/);
		BusyCooks.enqueue(the_cook, the_cook->getUnavailabalePriority());
		Cook::setAvailableGcount(Cook::GetAvailableGcount() - 1);
		Order::set_Served_Gorder(Order::get_Served_Gorder() + 1);
		msg += "  G" + to_string(the_cook->GetID()) + "(G" + to_string(the_order->GetID()) + ") ";
		return true;
	}
	return false;
}
bool Restaurant::assignOrderNormal(int timestep, string& msg)
{
	if (!Ncooks.isEmpty())
	{
		Order* the_order;
		Cook* the_cook;
		Ncooks.dequeue(the_cook);
		Norders.dequeue(the_order);
		//setting order new status and waiting time and calculate service time and assign to preprinng orders
		the_order->setStatus(SRV);
		the_order->setwaittime(timestep - the_order->getorderarrivaltime());
		the_order->setservicetime(the_order->getOrderSize() / the_cook->getspeed());
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cook informations 
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getOrderSize() / the_cook->getspeed() + timestep);
		the_cook->CalUnavailabalePriority(/*timestep*/);
		BusyCooks.enqueue(the_cook, the_cook->getUnavailabalePriority());
		Cook::setAvailableNcount(Cook::GetAvailableNcount() - 1);
		Order::set_Served_Norder(Order::get_Served_Norder() + 1);
		msg += "  N" + to_string(the_cook->GetID()) + "(N" + to_string(the_order->GetID()) + ") ";
		return true;
	}
	else if (!Vcooks.isEmpty())
	{
		Order* the_order;
		Cook* the_cook;
		Vcooks.dequeue(the_cook);
		Norders.dequeue(the_order);
		//setting order new status and waiting time and calculate service time and assign to preprinng orders
		the_order->setStatus(SRV);
		the_order->setwaittime(timestep - the_order->getorderarrivaltime());
		the_order->setservicetime(the_order->getOrderSize() / the_cook->getspeed());
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cook informations 
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getOrderSize() / the_cook->getspeed() + timestep);
		the_cook->CalUnavailabalePriority(/*timestep*/);
		BusyCooks.enqueue(the_cook, the_cook->getUnavailabalePriority());
		Cook::setAvailableVcount(Cook::GetAvailableVcount() - 1);
		Order::set_Served_Norder(Order::get_Served_Norder() + 1);
		msg += "  V" + to_string(the_cook->GetID()) + "(N" + to_string(the_order->GetID()) + ") ";
		return true;
	}
	return false;
}










bool Restaurant::assignOrderInjured(int timestep, Order* the_order, string& msg)
{
	bool IsAssined = false;
	Queue<Cook*> temp;
	Cook* the_cook;
	if (!InjuredCooks.isEmpty())
	{
		InjuredCooks.dequeue(the_cook);
		UrgentOrder.dequeue(the_order);
		the_order->setStatus(SRV);
		the_order->setwaittime(timestep - the_order->getorderarrivaltime());
		the_order->setservicetime(the_order->getOrderSize() / the_cook->getspeed());
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cook information 
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getOrderSize() / the_cook->getspeed() + timestep);
		the_cook->CalUnavailabalePriority(/*timestep*/);
		BusyCooks.enqueue(the_cook, the_cook->getUnavailabalePriority());
		Order::set_Served_Vorder(Order::get_Served_Vorder() + 1);
		switch (the_cook->GetType())
		{
		case TYPE_NRM:
			msg += " N" + to_string(the_cook->GetID()) + "(V" + to_string(the_order->GetID()) + ") ";
			break;
		case TYPE_VIP:
			msg += " V" + to_string(the_cook->GetID()) + "(V" + to_string(the_order->GetID()) + ") ";
			break;
		case TYPE_VGAN:
			msg += " G" + to_string(the_cook->GetID()) + "(V" + to_string(the_order->GetID()) + ") ";
			break;
		}
		IsAssined = true;

	}
	return IsAssined;
}

bool Restaurant::assignOrderBreak(int timestep, Order* the_order, string& msg)
{
	bool IsAssined = false;
	Queue<Cook*> temp;
	Cook* the_cook;
	if (!BreakCooks.isEmpty())
	{
		BreakCooks.dequeue(the_cook);


		UrgentOrder.dequeue(the_order);

		the_order->setStatus(SRV);
		the_order->setwaittime(timestep - the_order->getorderarrivaltime());
		the_order->setservicetime(the_order->getOrderSize() / the_cook->getspeed());
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cook information 
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getOrderSize() / the_cook->getspeed() + timestep);
		the_cook->CalUnavailabalePriority(/*timestep*/);
		BusyCooks.enqueue(the_cook, the_cook->getUnavailabalePriority());
		Order::set_Served_Vorder(Order::get_Served_Vorder() + 1);
		switch (the_cook->GetType())
		{
		case TYPE_NRM:
			msg += " N" + to_string(the_cook->GetID()) + "(V" + to_string(the_order->GetID()) + ") ";
			break;
		case TYPE_VIP:
			msg += " V" + to_string(the_cook->GetID()) + "(V" + to_string(the_order->GetID()) + ") ";
			break;
		case TYPE_VGAN:
			msg += " G" + to_string(the_cook->GetID()) + "(V" + to_string(the_order->GetID()) + ") ";
			break;
		}

		IsAssined = true;

	}
	return IsAssined;
}

void Restaurant::assignOrdertofinish(int timestep)
{
	int finished_at_this_time_step = 0;
	bool flag = true;
	Order* the_order;
	Order** FinishedArr = new Order * [100];


	//////////////Store all the Orders that finished at this time step////////////////////////////
	while (!prepare_Order.isEmpty() && flag)
	{
		prepare_Order.peekFront(the_order);
		the_order->CalFinish();
		if (the_order->getFinshtime() == timestep)
		{
			prepare_Order.dequeue(the_order);

			finished_at_this_time_step++;
			FinishedArr[finished_at_this_time_step - 1] = the_order;
			the_order->setStatus(DONE);

			Order::setFinishedOrdersCount(Order::getFinishedOrdersCount() + 1);

		}
		else
		{
			flag = false;
		}
	}

	////////////Sort all the orders according to thier service time/////////////////////////

	shellSort(FinishedArr, finished_at_this_time_step);

	//////////////////////Store them inside the finishe_order queue /////////////////////////

	for (int i = 0; i < finished_at_this_time_step; i++)
	{
		finished_order.enqueue(FinishedArr[i]);
	}

	///////////////////Delete the used array ////////////////////////////////

	delete[] FinishedArr;
	FinishedArr = nullptr;

}

void Restaurant::checkunavailblecooks(int timestep)
{

	bool flag = true;
	Cook* the_cook;


	////////////////////////////////////Check for Busy Cooks ////////////////////////////////////////

	flag = true;
	while (!(BusyCooks.isEmpty()) && flag)
	{
		BusyCooks.peekFront(the_cook);

		if (the_cook->getTimesteptobeavailabale() == timestep)
		{

			BusyCooks.dequeue(the_cook);
			the_cook->setN_orders_Finished(the_cook->getN_orders_Finished() + 1);

			if (the_cook->GetStatus() == INJURD)
			{

				the_cook->setServedOrder(nullptr);
				if (the_cook->getN_orders_Finished() >= the_cook->get_order_to_break())
				{
					the_cook->setN_orders_Finished(0);
				}
				the_cook->setTimesteptobeavailabale(timestep + the_cook->getRstPrd());
				the_cook->CalUnavailabalePriority();
				InjuredCooks.enqueue(the_cook, the_cook->getUnavailabalePriority());

			}

			else if (the_cook->getN_orders_Finished() >= the_cook->get_order_to_break())
			{
				the_cook->setStatus(BREAK);
				the_cook->setServedOrder(nullptr);
				the_cook->setN_orders_Finished(0);
				the_cook->setTimesteptobeavailabale(timestep + the_cook->getBreakduration());
				the_cook->CalUnavailabalePriority();
				the_cook->setSpeed(the_cook->getOriginalSpeed());
				BreakCooks.enqueue(the_cook, the_cook->getUnavailabalePriority());
			}
			else
			{
				the_cook->setStatus(AVAILABLE);
				switch (the_cook->GetType())
				{
				case TYPE_NRM:
					Cook::setAvailableNcount(Cook::GetAvailableNcount() + 1);
					Ncooks.enqueue(the_cook, the_cook->getspeed());
					break;
				case TYPE_VGAN:
					Cook::setAvailableGcount(Cook::GetAvailableGcount() + 1);
					Gcooks.enqueue(the_cook, the_cook->getspeed());
					break;
				case TYPE_VIP:
					Cook::setAvailableVcount(Cook::GetAvailableVcount() + 1);
					Vcooks.enqueue(the_cook, the_cook->getspeed());
					break;
				}
			}
		}
		else
		{
			flag = false;
		}
	}




	/////////////////////////////Check for Break Cooks///////////////////////////////



	flag = true;
	while (!(BreakCooks.isEmpty()) && flag)
	{
		BreakCooks.peekFront(the_cook);
		if (the_cook->getTimesteptobeavailabale() == timestep)
		{

			BreakCooks.dequeue(the_cook);
			the_cook->setStatus(AVAILABLE);
			switch (the_cook->GetType())
			{
			case TYPE_NRM:
				Ncooks.enqueue(the_cook, the_cook->getspeed());
				break;
			case TYPE_VGAN:
				Gcooks.enqueue(the_cook, the_cook->getspeed());
				break;
			case TYPE_VIP:
				Vcooks.enqueue(the_cook, the_cook->getspeed());
				break;
			}
		}
		else
		{
			flag = false;
		}
	}



	/////////////////////////////////Injured Cooks //////////////////////////////////////


	flag = true;
	while (!(InjuredCooks.isEmpty()) && flag)
	{
		InjuredCooks.peekFront(the_cook);
		if (the_cook->getTimesteptobeavailabale() == timestep)
		{
			InjuredCooks.dequeue(the_cook);
			the_cook->setSpeed(the_cook->getOriginalSpeed());
			the_cook->setStatus(AVAILABLE);
			switch (the_cook->GetType())
			{
			case TYPE_NRM:
				Ncooks.enqueue(the_cook, the_cook->getspeed());
				break;
			case TYPE_VGAN:
				Gcooks.enqueue(the_cook, the_cook->getspeed());
				break;
			case TYPE_VIP:
				Vcooks.enqueue(the_cook, the_cook->getspeed());
				break;
			}
		}
		else
		{
			flag = false;
		}
	}






	////////////////////Generate Injured Cook ///////////////////////////
	flag = true;
	float curr_prop = ((float)rand() / RAND_MAX);
	if (curr_prop <= Cook::getInjProp())
	{
		Queue<Cook*> temp;
		Queue<Order*>temp2;
		Order* the_order;

		while (!(BusyCooks.isEmpty()) && flag && !(prepare_Order.isEmpty()))
		{// the size is equal but every thing may happen
			BusyCooks.dequeue(the_cook);
			
			prepare_Order.dequeue(the_order);
			if (the_cook->GetStatus() != INJURD)
			{
				
				


				if (the_order != the_cook->getServedOrder())
				{
					///////////This case shouldn't happen so i will make it an error to be obvious to correct in the coming cases
					while (true)
					{
						cout << "in valid";
					}
					/////// The first cook in BusyCooks should have the first order in prepare_Order
				}
				int remaining_time_for_serving = the_cook->getTimesteptobeavailabale() - timestep;

				///////////////////this should be equal to : the_order->getFinishtime() - timestep;

				the_cook->setStatus(INJURD);
				if ( (the_cook->getspeed() / 2 ) >= 1)
				{
					the_cook->setSpeed(the_cook->getspeed() / 2);
				}
				Cook::increase_injury(); // for out file 
				the_cook->setTimesteptobeavailabale(the_cook->getTimesteptobeavailabale() + remaining_time_for_serving);
				the_order->setservicetime(the_order->getservicetime() + remaining_time_for_serving);
				the_order->CalFinish();
				prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
				the_cook->CalUnavailabalePriority();
				BusyCooks.enqueue(the_cook, the_cook->getUnavailabalePriority());
				flag = false;

			}
			else
			{
				temp.enqueue(the_cook);
				temp2.enqueue(the_order);
			}
		}

		while (!temp.isEmpty() && !temp2.isEmpty())
		{//the size of temp=temp2 but i do it for every thing may happen
			temp2.dequeue(the_order);
			temp.dequeue(the_cook);
			BusyCooks.enqueue(the_cook, the_cook->getUnavailabalePriority());
			prepare_Order.enqueue(the_order,1000- the_order->getFinshtime());
		}


	}

}




void Restaurant::loadfile()
{
	int Ncook, Gcook, Vcook, NspeedMin, NspeedMax, GspeedMin, GspeedMax, VspeedMin, VspeedMax, OrdersToBreak, NBreakMin, NBreakMax, GbreakMin, GbreakMax, VbreakMin, VbreakMax, AutoP, EventsNum;
	char EventType;
	char OTYP;
	int TS, ID, SIZE;
	ORD_TYPE TYP;
	double MONY;
	int  RstPrd, VIP_WT;
	float InjProp;
	ifstream myfile;

	pGUI->PrintMessage("Enter file input name");
	string file_name = pGUI->GetString();

	myfile.open(file_name + ".txt");

	myfile >> Ncook >> Gcook >> Vcook;
	myfile >> NspeedMin >> NspeedMax >> GspeedMin >> GspeedMax >> VspeedMin >> VspeedMax;
	myfile >> OrdersToBreak >> NBreakMin >> NBreakMax >> GbreakMin >> GbreakMax >> VbreakMin >> VbreakMax;
	myfile >> InjProp >> RstPrd;
	myfile >> AutoP >> VIP_WT;
	myfile >> EventsNum;


	//creating Ncooks
	Cook* newNcooks = new Cook[Ncook];

	//creating Gcooks
	Cook* newGcooks = new Cook[Gcook];
	//creating Vcooks
	Cook* newVcook = new Cook[Vcook];
	//setting number of cooks for output file 
	Cook::setVcount(Vcook);
	Cook::setNcount(Ncook);
	Cook::setGcount(Gcook);
	Cook::setAvailableVcount(Vcook);
	Cook::setAvailableNcount(Ncook);
	Cook::setAvailableGcount(Gcook);

	//setting static data type order to break 
	Cook::setordertobreak(OrdersToBreak);
	//setting static data type autopormotion
	Order::setautopormotion(AutoP);
	Order::SetVIP_WT(VIP_WT);
	//setting static
	Cook::SetRstPrd(RstPrd);
	//setting static
	Cook::SetInjProp(InjProp);
	//setting data for normal cooks and enqueue them
	for (int i = 0; i < Ncook; i++)
	{
		newNcooks[i].setSpeed(NspeedMin + rand() % NspeedMax);
		newNcooks[i].setOriginalSpeed(newNcooks[i].getspeed());
		newNcooks[i].setType(TYPE_NRM);
		newNcooks[i].setBreakduration(NBreakMin + rand() % NBreakMin);

		newNcooks[i].setID(i + 1);
		//we should ask for bonous that every cook has different periorty depend on different speed
		Ncooks.enqueue(&newNcooks[i], newNcooks[i].getspeed());
	}
	//setting data for vegan cooks and enqueue them
	for (int i = 0; i < Gcook; i++)
	{
		newGcooks[i].setSpeed(GspeedMin + rand() % GspeedMax);
		newGcooks[i].setOriginalSpeed(newGcooks[i].getspeed());
		newGcooks[i].setType(TYPE_VGAN);
		newGcooks[i].setBreakduration(GbreakMin + rand() % GbreakMax);

		newGcooks[i].setID(i + 1);
		//we should ask for bonous that every cook has different periorty depend on different speed
		Gcooks.enqueue(&newGcooks[i], newGcooks[i].getspeed());
	}
	//setting data for VIP cooks and enqueue them
	for (int i = 0; i < Vcook; i++)
	{
		newVcook[i].setSpeed(VspeedMin + rand() % VspeedMax);
		newVcook[i].setOriginalSpeed(newVcook[i].getspeed());
		newVcook[i].setType(TYPE_VIP);
		newVcook[i].setBreakduration(VbreakMin + rand() % VbreakMax);

		newVcook[i].setID(i + 1);
		//we should ask for bonous that every cook has different periorty depend on different speed
		Vcooks.enqueue(&newVcook[i], newVcook[i].getspeed());
	}
	//reading events lines and set its data the enqueue them in events queue

	//pointer to base class event 
	Event* newevent;
	Event** EventArray = new Event* [EventsNum];
	for (int i = 0; i < EventsNum; i++)
	{
		myfile >> EventType;

		switch (EventType)
		{
			//arrival event
		case 'R':
		{
			myfile >> OTYP >> TS >> ID >> SIZE >> MONY;
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
			myfile >> TS >> ID;
			newevent = new CancellationEvent(TS, ID);
			break;
		}
		//Promotion event
		case 'P':
		{
			myfile >> TS >> ID >> MONY;
			newevent = new PromotionEvent(TS, ID, MONY);
			break;
		}

		}
		EventArray[i] = newevent;
		
	
	}

	shellSortEvents(EventArray, EventsNum);
	for (int i = 0; i < EventsNum; i++)
	{
		EventsQueue.enqueue(EventArray[i]);

	}
	delete[] EventArray;
	myfile.close();

}



//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event* pE;
	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current timestep
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
	int  VOrdersCount, NOrdersCount, GOrdersCount;
	VOrdersCount = 0;
	NOrdersCount = 0;
	GOrdersCount = 0;

	int  VCooksCount, NCooksCount, GCooksCount, ServOrder, Doneorder;
	VCooksCount = 0;
	NCooksCount = 0;
	GCooksCount = 0;
	ServOrder = 0;
	Doneorder = 0;
	//This function should be implemented in phase1
	//It should add ALL orders and Cooks to the drawing list
	Order* orderptr;
	Cook* cookptr;
	//orders copy 
	Order** Vorderscopy = Vorders.toArray(VOrdersCount);
	Order** Norderscopy = Norders.toArray(NOrdersCount);
	Order** Gorderscopy = Gorders.toArray(GOrdersCount);
	Order::set_waiting_Gorder(GOrdersCount);
	Order::set_waiting_Norder(NOrdersCount);
	Order::set_waiting_Vorder(VOrdersCount);
	//cooks copy
	Cook** Vcookscopy = Vcooks.toArray(VCooksCount);
	Cook** Ncookscopy = Ncooks.toArray(NCooksCount);
	Cook** Gcookscopy = Gcooks.toArray(GCooksCount);
	// surved and done orders
	Order** Servordercopy = prepare_Order.toArray(ServOrder);
	Order** Doneordercopy = finished_order.toArray(Doneorder);
	///////////////Drawing VIP Orders ///////////////
	for (int i = 0; i < VOrdersCount; i++)
	{
		pGUI->AddToDrawingList(Vorderscopy[i]);
	}

	///////////////Drawing Normal Orders ///////////////
	for (int i = 0; i < NOrdersCount; i++)
	{
		pGUI->AddToDrawingList(Norderscopy[i]);
	}


	///////////////Drawing Vegan Orders ///////////////
	for (int i = 0; i < GOrdersCount; i++)
	{
		pGUI->AddToDrawingList(Gorderscopy[i]);
	}



	///////////////Drawing VIP Cooks ///////////////
	for (int i = 0; i < VCooksCount; i++)
	{
		pGUI->AddToDrawingList(Vcookscopy[i]);
	}


	///////////////Drawing Normal Cooks ///////////////
	for (int i = 0; i < NCooksCount; i++)
	{
		pGUI->AddToDrawingList(Ncookscopy[i]);
	}


	///////////////Drawing Vegan Cooks ///////////////
	for (int i = 0; i < GCooksCount; i++)
	{
		pGUI->AddToDrawingList(Gcookscopy[i]);
	}
	///////////////Drawing SerV Cooks ///////////////
	for (int i = 0; i < ServOrder; i++)
	{
		pGUI->AddToDrawingList(Servordercopy[i]);

	}
	///////////////Drawing Done Cooks ///////////////
	for (int i = 0; i < Doneorder; i++)
	{
		pGUI->AddToDrawingList(Doneordercopy[i]);

	}

	//update interface 
	pGUI->UpdateInterface();
	pGUI->ResetDrawingList();

}









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
		Vorders.enqueue(po, po->getPriority());
		//add order for Vip
	}

}
void Restaurant::Seacrh(int ID, Order*& frntEntry)
{
	Queue<Order*> qtemp;
	Order* Otemp;
	bool flag = true;
	//search about order
	while (Norders.dequeue(Otemp) && flag)
	{
		if (Otemp->GetID() != ID)
			qtemp.enqueue(Otemp);
		else
		{
			frntEntry = Otemp;
			flag = false;
		}

	}

	while (qtemp.dequeue(Otemp))
		Norders.enqueue(Otemp);
}

void Restaurant::outputfile()
{
	//sorting the finish queue 
	int count = 0;
	float AvgWT = 0.0;
	float AvgST = 0.0;
	Order** finisharr = finished_order.toArray(count);
	//heapSort(finisharr, count);
	//getting avg wait time and avg service time 
	getavgSTandWT(finisharr, count, AvgWT, AvgST);
	pGUI->PrintMessage("Enter file output name ");
	string file_name = pGUI->GetString();
	ofstream myfile;
	myfile.open(file_name + ".txt");
	//writing finish time and id and arrival time and wait time and service time of each order
	myfile << "FT" << " " << "ID" << " " << "AT" << " " << "WT" << " " << "ST" << "\n";
	for (int i = 0; i < count; i++)
	{
		myfile << finisharr[i]->getFinshtime() << "  " << finisharr[i]->GetID() << " ";
		myfile << finisharr[i]->getorderarrivaltime() << " " << finisharr[i]->getwaittime() << " ";
		myfile << finisharr[i]->getservicetime() << "\n";
	}
	//writing num of all orders and num of all types of orders
	myfile << "Orders: " << Order::getordercount() << " [Norm:" << Order::getNordercount();
	myfile << ", Veg:" << Order::getGordercount() << ", VIP:" << Order::getVordercount() << "]\n";
	//writing num of all cooks and num of all types of cooks
	myfile << "cooks: " << Cook::Getcookscount() << " [Norm:" << Cook::GetNcount() << ", Veg:" << Cook::GetGcount() << ", VIP" << Cook::GetVcount();
	myfile << ", injured:" << Cook::get_num_of_injury() << "]" << "\n";
	//getting avg of 
	myfile << "Avg Wait = " << AvgWT << ",  Avg Serv = " << AvgST << "\n";
	myfile << " Urgent orders : " << Order::get_Urgent_num() << ", Auto - promoted :" << Order::Get_num_of_order_auto_P() << "\n";
}

void Restaurant::getavgSTandWT(Order** arr, int count, float& avgWT, float& avgST)
{
	float WTsum = 0;
	float STsum = 0;
	for (int i = 0; i < count; i++)
	{
		WTsum += arr[i]->getwaittime();
		STsum += arr[i]->getservicetime();
	}
	avgWT = (WTsum) / float(count);
	avgST = (STsum) / float(count);
}


void Restaurant::shellSort(Order* arr[], int n)
{
	// Start with a big gap, then reduce the gap 
	for (int gap = n / 2; gap > 0; gap /= 2)
	{
		// Do a gapped insertion sort for this gap size. 
		// The first gap elements a[0..gap-1] are already in gapped order 
		// keep adding one more element until the entire array is 
		// gap sorted  
		for (int i = gap; i < n; i += 1)
		{
			// add a[i] to the elements that have been gap sorted 
			// save a[i] in temp and make a hole at position i 
			Order* temp = arr[i];

			// shift earlier gap-sorted elements up until the correct  
			// location for a[i] is found 
			int j;
			for (j = i; j >= gap && (arr[j - gap]->getservicetime() > temp->getservicetime()); j -= gap)
				arr[j] = arr[j - gap];

			//  put temp (the original a[i]) in its correct location 
			arr[j] = temp;
		}
	}
}


void Restaurant::shellSortEvents(Event* arr[], int n)
{
	// Start with a big gap, then reduce the gap 
	for (int gap = n / 2; gap > 0; gap /= 2)
	{
		// Do a gapped insertion sort for this gap size. 
		// The first gap elements a[0..gap-1] are already in gapped order 
		// keep adding one more element until the entire array is 
		// gap sorted  
		for (int i = gap; i < n; i += 1)
		{
			// add a[i] to the elements that have been gap sorted 
			// save a[i] in temp and make a hole at position i 
			Event* temp = arr[i];

			// shift earlier gap-sorted elements up until the correct  
			// location for a[i] is found 
			int j;
			for (j = i; j >= gap && (arr[j - gap]->getEventTime() > temp->getEventTime() ); j -= gap)
				arr[j] = arr[j - gap];

			//  put temp (the original a[i]) in its correct location 
			arr[j] = temp;
		}
	}
}











/*
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





void Restaurant::reheapdown(Order**& arr, int n, int root)
{
	int largest = root; // root is the largest element
	int l = 2 * root + 1; // left = 2*root + 1
	int r = 2 * root + 2; // right = 2*root + 2

	// If left child is larger than root
	if (l < n && arr[l]->getFinshtime() > arr[largest]->getFinshtime())
		largest = l;

	//if left child is equal the largest so far
	if (l < n && arr[l]->getFinshtime() == arr[largest]->getFinshtime())
		if (arr[l]->getorderarrivaltime() > arr[largest]->getorderarrivaltime())
			largest = l;

	// If right child is larger than largest so far
	if (r < n && arr[r]->getFinshtime() > arr[largest]->getFinshtime())
		largest = r;

	//if right child is equal the largest so far
	if (r < n && arr[r]->getFinshtime() == arr[largest]->getFinshtime())
		if (arr[r]->getorderarrivaltime() > arr[largest]->getorderarrivaltime())
			largest = r;
	// If largest is not root
	if (largest != root)
	{
		//swap root and largest

		swap(arr[root], arr[largest]);

		// Recursively heapify the sub-tree
		reheapdown(arr, n, largest);
	}
}

void Restaurant::heapSort(Order**& arr, int n)
{
	// build heap
	for (int i = n / 2 - 1; i >= 0; i--)
		reheapdown(arr, n, i);

	// extracting elements from heap one by one
	for (int i = n - 1; i >= 0; i--)
	{
		// Move current root to end
		swap(arr[0], arr[i]);

		// again call max heapify on the reduced heap
		reheapdown(arr, i, 0);
	}
}





*/






