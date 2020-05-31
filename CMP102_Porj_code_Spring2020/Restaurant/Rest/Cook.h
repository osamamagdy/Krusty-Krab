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
	bool IsInjured;
	
	Order* ServedOrder; 
	
	//data members for output file 
	static int Vcount;
	static int Ncount;
	static int Gcount;
	static int num_injure_cook;
	static int Available_Vcount;
	static int Available_Ncount;
	static int Available_Gcount;
public:
	Cook();
	virtual ~Cook();
	int GetID() const;
	ORD_TYPE GetType() const;
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
	void CalUnavailabalePriority();
	int getUnavailabalePriority();

	void setTimesteptobeavailabale(int time);
	int getTimesteptobeavailabale();
	void setN_orders_Finished(int num);
	int getN_orders_Finished();

	Order* getServedOrder();

	void setServedOrder(Order* Served);

	void setOriginalSpeed(int sp);
	int getOriginalSpeed();

	void set_isinjured(bool val);
	bool get_isinjured();


	//output file member func
	static void setVcount(int count);
	static void setNcount(int count);
	static void setGcount(int count);
	static int GetVcount();
	static int GetNcount();
	static int GetGcount();
	static void setAvailableVcount(int count);
	static void setAvailableNcount(int count);
	static void setAvailableGcount(int count);
	static int GetAvailableVcount();
	static int GetAvailableNcount();
	static int GetAvailableGcount();
	static int Getcookscount();
	static void increase_injury();
	static int get_num_of_injury();
	

};
