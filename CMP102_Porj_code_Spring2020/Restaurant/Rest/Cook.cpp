#include "Cook.h"

int Cook::OrderstoBreak = 0;
float Cook::InjProp = 0;
int Cook::RstPrd = 0;
Cook::Cook()
{
	N_orders_Finished = 0;
}


Cook::~Cook()
{
}


int Cook::GetID() const
{
	return ID;
}


ORD_TYPE Cook::GetType() const
{
	return type;
}

int  Cook::GetN_orderFinshed()
{
	return N_orders_Finished;
}
void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}

void Cook::setSpeed(int s)
{
	speed = (s >= 0) ? s : 0;
}

void Cook::setBreakduration(int B)
{
	Breakduration = (B >= 0) ? B : 0;
}

int Cook::getBreakduration()
{
	return Breakduration;
}

void Cook::setordertobreak(int ordtobreak)
{
	OrderstoBreak = ordtobreak;
}


COOK_STATUS Cook::GetStatus()const
{
	return status;
}

void Cook::setStatus(COOK_STATUS st)
{
	status = st;
}

int Cook::getspeed()
{
	return speed;
}

void Cook::SetRstPrd(int s)
{
	RstPrd = s > 0 ? s : 0;
}
int Cook::getRstPrd()
{
	return RstPrd;
}
void  Cook::SetInjProp(float i)
{
	InjProp = i > 0 ? i : 0;
}
float Cook::getInjProp()
{
	return InjProp;
}
void Cook::CalUnavailabalePriority(int RealTimestep)
{
	Unavailabalepriority = Timesteptobeavailabale+(N_orders_Finished/OrderstoBreak)*Breakduration+(status/InjProp)*(RealTimestep -Timesteptobeavailabale);
}

int Cook::getUnavailabalePriority()
{
	return Unavailabalepriority;
}
void Cook::CalavailabalePriority()
{
	availabalepriority=speed + ID + type +  Breakduration+ 1 / (status + 1);
}


int Cook::getavailabalePriority()
{
	return availabalepriority;
}
