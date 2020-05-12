#pragma once

#include "..\Defs.h"

#pragma once
class Cook
{
	static int OrderstoBreak;
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	int Breakduration;
	COOK_STATUS status;
	static float InjProp ;
	static int RstPrd;
	int Unavailabalepriority, availabalepriority, N_orders_Finished, Timesteptobeavailabale;
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
	void CalUnavailabalePriority(int RealTimeStep);
	int getUnavailabalePriority();
	void CalavailabalePriority();
	int getavailabalePriority();
	int GetN_orderFinshed();
};
