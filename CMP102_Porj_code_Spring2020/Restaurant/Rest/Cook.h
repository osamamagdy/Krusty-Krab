#pragma once

#include "..\Defs.h"

#pragma once
class Cook
{
	static int OrderstoBreak;
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int Minspeed, Maxspeed;		//dishes it can prepare in one clock tick (in one timestep)
	int BreakdurationMin, BreakdurationMax;
	COOK_STATUS status;
	float InjProp ;
	int RstPrd;
public:
	Cook();
	virtual ~Cook();
	int GetID() const;
	ORD_TYPE GetType() const;
	//void setID(int);
	//void setType(ORD_TYPE) ;
	void setMaxSpeed(int s);
	void setMinSpeed(int s);
	void setMinBreakduration(int B);
	void setMaxBreakduration(int B);
	static void setordertobreak(int ordtobreak);
	COOK_STATUS GetStatus()const;
	void setID(int ID);
	void setType(ORD_TYPE type) ;
	void setStatus(COOK_STATUS st);
	int getMinspeed();
	int getMaxspeed();
	void SetRstPrd(int s);
	int getRstPrd();
	void  SetInjProp(float i);
	float getInjProp();
	int getMinBreakduration();
	int getMaxBreakduration();
};
