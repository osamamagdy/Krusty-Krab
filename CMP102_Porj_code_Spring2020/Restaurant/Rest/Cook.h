#pragma once

#include "..\Defs.h"
#include "Order.h"
#pragma once
class Cook
{
	static int OrderstoBreak;
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	int OriginalSpeed;
	int Breakduration;
	COOK_STATUS status;
	static float InjProp ;
	static int RstPrd;
	int Unavailabalepriority, availabalepriority, Injuredpriority, N_orders_Finished, Timesteptobeavailabale;
	
	Order* ServedOrder; 
	
	//data members for output file 
	static int Vcount;
	static int Ncount;
	static int Gcount;
public:
	Cook();
	virtual ~Cook();
	int GetID() const;
	ORD_TYPE GetType() const;
	//void setID(int);
	//void setType(ORD_TYPE) ;

	void setSpeed(int s);
	void setBreakduration(int B);
	static void setordertobreak(int ordtobreak);
	static int get_order_to_break();
	COOK_STATUS GetStatus()const;
	void setID(int ID);
	void setType(ORD_TYPE type) ;
	void setStatus(COOK_STATUS st);
	int getspeed();
	static void SetRstPrd(int s);
	static int getRstPrd();
	static void  SetInjProp(float i);
	static float getInjProp();
	int getBreakduration();
	void CalUnavailabalePriority(/*int RealTimeStep*/);
	int getUnavailabalePriority();
	void CalavailabalePriority();
	int getavailabalePriority();
	int GetN_orderFinshed();
	void setTimesteptobeavailabale(int time);
	int getTimesteptobeavailabale();
	void setN_orders_Finished(int num);
	int getN_orders_Finished();
	void CalInjuredPriority();
	int getInjuredPriority();

	Order* getServedOrder();

	void setServedOrder(Order* Served);

	void setOriginalSpeed(int sp);
	int getOriginalSpeed();

	//output file member func
	static void setVcount(int count);
	static void setNcount(int count);
	static void setGcount(int count);
	static int GetVcount();
	static int GetNcount();
	static int GetGcount();
	static int Getcookscount();
	

};
