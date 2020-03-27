#pragma once

#include "..\Defs.h"

#pragma once
class Cook
{
	static int OrderstoBreak;
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	COOK_STATUS status;

public:
	Cook();
	virtual ~Cook();
	int GetID() const;
	ORD_TYPE GetType() const;
	COOK_STATUS GetStatus()const;
	void setID(int ID);
	void setType(ORD_TYPE type) ;
	void setStatus(COOK_STATUS st);
};
