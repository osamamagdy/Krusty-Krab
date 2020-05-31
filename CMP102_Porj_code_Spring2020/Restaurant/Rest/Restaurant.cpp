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
	if (!loadfile())
	{
		pGUI->PrintMessage("Couldn't find this file");
		Sleep(2000);
		return;
	}
	if (Vcooks.isEmpty() && Gcooks.isEmpty() && Ncooks.isEmpty())
	{
		pGUI->PrintMessage("You don't have any Cooks, The restaurant is closed");

		PlaySound(TEXT("good_bye.wav"), NULL, SND_SYNC);

		return;
	}

	PROG_MODE	mode = pGUI->getGUIMode();

	switch (mode)	//switch to choose our mode
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
	if (!Gorders.isEmpty())
	{
		pGUI->PrintMessage("You don't have vegan Cooks, So the restaurant didn't prepera any vegan orders");
		Sleep(1000);
	}
	if (!Norders.isEmpty())
	{
		pGUI->PrintMessage("You don't have VIP and Normal Cooks, So the restaurant didn't prepera any Normal orders");
		Sleep(1000);
	}
	PlaySound(TEXT("good_bye.wav"), NULL, SND_SYNC);
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
	while (
		!EventsQueue.isEmpty() ||
		( !Vorders.isEmpty() && ( !Vcooks.isEmpty() || !Ncooks.isEmpty() || !Gcooks.isEmpty() || !InjuredCooks.isEmpty() || !BreakCooks.isEmpty() ) ) ||
		( !Norders.isEmpty() && ( !Vcooks.isEmpty() || !Ncooks.isEmpty() || !InjuredCooks.isEmpty() || !BreakCooks.isEmpty() ) ) ||
		(!Gorders.isEmpty() && ( !Gcooks.isEmpty() || !InjuredCooks.isEmpty() ||!BreakCooks.isEmpty() ) )||
		!prepare_Order.isEmpty() || 
		!BusyCooks.isEmpty() 
		)
	{

		string masg4 = "";
		bool Vflag = true;
		bool Gflag = true;
		bool Nflag = true;
		bool Urgentflag = true;
		//Excute events in each timestep
		ExecuteEvents(timestep);
		//assign prepared order to finish
		assignOrdertofinish(timestep);
		//check for unavailble cooks 
		checkunavailblecooks(timestep);
		//check if there is an urgent vip-order
		CheckurgentForVIP(timestep);
		//checking if there is an auto pormoted normal order in this time step
		autopormotedForNormal(timestep);
		//start to assign orders according to Orders Service Criteria 
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
		//applaying different mode 
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

		//increase our current time step
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
		if (waitTime > prmotedTime)
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
			the_order->setservicetime(int(ceil(float(the_order->getOrderSize()) / the_cook->getspeed())));

			the_order->CalFinish();
			prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
			//setting cook information 
			the_cook->setStatus(BUSY);
			the_cook->setServedOrder(the_order);
			the_cook->setTimesteptobeavailabale(the_order->getservicetime() + timestep);
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
			the_order->setservicetime(int(ceil(float(the_order->getOrderSize()) / the_cook->getspeed())));

			the_order->CalFinish();
			prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
			//setting cook information 
			the_cook->setStatus(BUSY);
			the_cook->setServedOrder(the_order);
			the_cook->setTimesteptobeavailabale(the_order->getservicetime() + timestep);
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

			the_order->setservicetime(int(ceil(float(the_order->getOrderSize()) / the_cook->getspeed())));
			the_order->CalFinish();
			prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
			//setting cook information 
			the_cook->setStatus(BUSY);
			the_cook->setServedOrder(the_order);
			the_cook->setTimesteptobeavailabale(the_order->getservicetime() + timestep);
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

		the_order->setservicetime(int(ceil(float(the_order->getOrderSize()) / the_cook->getspeed())));
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cook information 
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getservicetime() + timestep);
		the_cook->CalUnavailabalePriority();
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

		the_order->setservicetime(int(ceil(float(the_order->getOrderSize()) / the_cook->getspeed())));
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cook information 
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getservicetime() + timestep);
		the_cook->CalUnavailabalePriority();
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

		the_order->setservicetime(int(ceil(float(the_order->getOrderSize()) / the_cook->getspeed())));
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cook information 
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getservicetime() + timestep);
		the_cook->CalUnavailabalePriority();
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

		the_order->setservicetime(int(ceil(float(the_order->getOrderSize()) / the_cook->getspeed())));
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cooks informations
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getservicetime() + timestep);
		the_cook->CalUnavailabalePriority();
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

		the_order->setservicetime(int(ceil(float(the_order->getOrderSize()) / the_cook->getspeed())));
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cook informations 
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getservicetime() + timestep);
		the_cook->CalUnavailabalePriority();
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

		the_order->setservicetime(int(ceil(float(the_order->getOrderSize()) / the_cook->getspeed())));
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cook informations 
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getservicetime() + timestep);
		the_cook->CalUnavailabalePriority();
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

		the_order->setservicetime(int(ceil(float(the_order->getOrderSize()) / the_cook->getspeed())));
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cook information 
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getservicetime() + timestep);
		the_cook->CalUnavailabalePriority();
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

		the_order->setservicetime(int(ceil(float(the_order->getOrderSize()) / the_cook->getspeed())));
		the_order->CalFinish();
		prepare_Order.enqueue(the_order, 1000 - the_order->getFinshtime());
		//setting cook information 
		the_cook->setStatus(BUSY);
		the_cook->setServedOrder(the_order);
		the_cook->setTimesteptobeavailabale(the_order->getservicetime() + timestep);
		the_cook->CalUnavailabalePriority();
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
		if (the_order->getFinshtime() <= timestep)
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
		// get the peek of busy cooks queue
		BusyCooks.peekFront(the_cook);
		//check if he is available now 
		if (the_cook->getTimesteptobeavailabale() == timestep)
		{
			
			BusyCooks.dequeue(the_cook);
			the_cook->setN_orders_Finished(the_cook->getN_orders_Finished() + 1);
			//check if he is injury
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
			//check if the cook need break
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
				
				

				/*
				if (the_order != the_cook->getServedOrder())
				{
					///////////This case shouldn't happen so i will make it an error to be obvious to correct in the coming cases
					while (true)
					{
						cout << "in valid";
					}
					/////// The first cook in BusyCooks should have the first order in prepare_Order
				}
				*/





				int remaining_time_for_serving = the_cook->getTimesteptobeavailabale() - timestep;

				///////////////////this should be equal to : the_order->getFinishtime() - timestep;

				the_cook->setStatus(INJURD);
				if ( (the_cook->getspeed() / 2 ) >= 1)
				{
					the_cook->setSpeed(the_cook->getspeed() / 2);
				}
				else
				{
					the_cook->setSpeed(1);
				}

				if (! the_cook->get_isinjured())
				{
					the_cook->set_isinjured(true);
					Cook::increase_injury(); // for out file
				} 
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
bool Restaurant::loadfile()
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
	int temp;
	//getting file name from user
	pGUI->PrintMessage("Enter file input name");
	string file_name = pGUI->GetString();

	myfile.open(file_name + ".txt");
	if (myfile.fail())
	{
		pGUI->PrintMessage("Invalid input file Name");
		return false;
	}
	myfile >> Ncook >> Gcook >> Vcook;
	myfile >> NspeedMin >> NspeedMax >> GspeedMin >> GspeedMax >> VspeedMin >> VspeedMax;
	myfile >> OrdersToBreak >> NBreakMin >> NBreakMax >> GbreakMin >> GbreakMax >> VbreakMin >> VbreakMax;
	myfile >> InjProp >> RstPrd;
	myfile >> AutoP >> VIP_WT;
	myfile >> EventsNum;
	//handling user errors in input file
	if (NspeedMax<NspeedMin)
	{
		temp = NspeedMax;
		NspeedMax = NspeedMin;
		NspeedMin = temp;
	}

	if (VspeedMax < VspeedMin)
	{
		temp = VspeedMax;
		VspeedMax = VspeedMin;
		VspeedMin = temp;
	}

	if (GspeedMax < GspeedMin)
	{
		temp = GspeedMax;
		GspeedMax = GspeedMin;
		GspeedMin = temp;
	}

	if (NBreakMax < NBreakMin)
	{
		temp = NBreakMax;
		NBreakMax = NBreakMin;
		NBreakMin = temp;
	}

	if (VbreakMax < VbreakMin)
	{
		temp = VbreakMax;
		VbreakMax = VbreakMin;
		VbreakMin = temp;
	}

	if (GbreakMax < GbreakMin)
	{
		temp = GbreakMax;
		GbreakMax = GbreakMin;
		GbreakMin = temp;
	}

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
		if (NspeedMax==NspeedMin)
		{
			newNcooks[i].setSpeed(NspeedMin);
		}
		else
		{
			newNcooks[i].setSpeed(NspeedMin + rand() % (NspeedMax - NspeedMin));

		}
		newNcooks[i].setOriginalSpeed(newNcooks[i].getspeed());
		newNcooks[i].setType(TYPE_NRM);
		
		if (NBreakMax==NBreakMin)
		{
			newNcooks[i].setBreakduration(NBreakMin);
		}
		else
		{
			newNcooks[i].setBreakduration(NBreakMin + rand() % (NBreakMax - NBreakMin));
		}
		newNcooks[i].setID(i + 1);
		//we should ask for bonous that every cook has different periorty depend on different speed
		Ncooks.enqueue(&newNcooks[i], newNcooks[i].getspeed());
	}
	//setting data for vegan cooks and enqueue them
	for (int i = 0; i < Gcook; i++)
	{
		if (GspeedMax == GspeedMin)
		{
			newGcooks[i].setSpeed(GspeedMin);
		}
		else
		{
			newGcooks[i].setSpeed(GspeedMin + rand() % (GspeedMax - GspeedMin));

		}
		newGcooks[i].setOriginalSpeed(newGcooks[i].getspeed());
		newGcooks[i].setType(TYPE_VGAN);
		

		if (GbreakMax == GbreakMin)
		{
			newGcooks[i].setBreakduration(GbreakMin);
		}
		else
		{
			newGcooks[i].setBreakduration(GbreakMin + rand() % (GbreakMax - GbreakMin));
		}
		newGcooks[i].setID(i + 1);
		//we should ask for bonous that every cook has different periorty depend on different speed
		Gcooks.enqueue(&newGcooks[i], newGcooks[i].getspeed());
	}
	//setting data for VIP cooks and enqueue them
	for (int i = 0; i < Vcook; i++)
	{

		if (VspeedMax == VspeedMin)
		{
			newVcook[i].setSpeed(VspeedMin);
		}
		else
		{
			newVcook[i].setSpeed(VspeedMin + rand() % (VspeedMax - VspeedMin));
		}
		newVcook[i].setOriginalSpeed(newVcook[i].getspeed());
		newVcook[i].setType(TYPE_VIP);

		if (VbreakMax == VbreakMin)
		{
			newVcook[i].setBreakduration(VbreakMin);
		}
		else
		{
			newVcook[i].setBreakduration(VbreakMin + rand() % (VbreakMax - VbreakMin));
		}
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
	return true;
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
	int  VOrdersCount, NOrdersCount, GOrdersCount, UrgentOrdersCount;
	VOrdersCount = 0;
	NOrdersCount = 0;
	GOrdersCount = 0;
	UrgentOrdersCount = 0;
	int  VCooksCount, NCooksCount, GCooksCount, ServOrder, Doneorder;
	VCooksCount = 0;
	NCooksCount = 0;
	GCooksCount = 0;
	ServOrder = 0;
	Doneorder = 0;
	
	Order* orderptr;
	Cook* cookptr;
	//orders copy 
	Order** Vorderscopy = Vorders.toArray(VOrdersCount);
	Order** Norderscopy = Norders.toArray(NOrdersCount);
	Order** Gorderscopy = Gorders.toArray(GOrdersCount);
	Order** Urgentorderscopy = UrgentOrder.toArray(UrgentOrdersCount);
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

	///////////////Drawing VIP Urgent Orders ///////////////
	for (int i = 0; i < UrgentOrdersCount; i++)
	{
		pGUI->AddToDrawingList(Urgentorderscopy[i]);
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
	
	//search about order
	while (!Norders.isEmpty())
	{
		Norders.dequeue(Otemp);
		if (Otemp->GetID() != ID)
			qtemp.enqueue(Otemp);
		else
		{
			frntEntry = Otemp;
			
		}

	}

	while (!qtemp.isEmpty())
	{
		qtemp.dequeue(Otemp);
			Norders.enqueue(Otemp);
	}
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
	myfile << "cooks: " << Cook::Getcookscount() << " [Norm:" << Cook::GetNcount() << ", Veg:" << Cook::GetGcount() << ", VIP:" << Cook::GetVcount();
	myfile << ", injured:" << Cook::get_num_of_injury() << "]" << "\n";
	//getting avg of 
	myfile << "Avg Wait = " << AvgWT << ",  Avg Serv = " << AvgST << "\n";
	myfile << "Urgent orders : " << Order::get_Urgent_num() << ", Auto - promoted :" << Order::Get_num_of_order_auto_P() << "\n";
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











