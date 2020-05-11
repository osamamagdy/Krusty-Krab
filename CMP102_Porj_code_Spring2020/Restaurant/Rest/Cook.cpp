#include "Cook.h"

int Cook::OrderstoBreak = 0;
Cook::Cook()
{
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


void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}

void Cook::setMinSpeed(int s)
{
	Minspeed = (s >= 0) ? s : 0;
}
void Cook::setMaxSpeed(int s)
{
	Maxspeed = (s >= 0) ? s : 0;
}

void Cook::setMinBreakduration(int B)
{
	BreakdurationMin = (B >= 0) ? B : 0;
}
void Cook::setMaxBreakduration(int B)
{
	BreakdurationMax = (B >= 0) ? B : 0;
}
int Cook::getMinBreakduration()
{
	return BreakdurationMin;
}
int Cook::getMaxBreakduration()
{
	return BreakdurationMax;
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

int Cook::getMinspeed()
{
	return Minspeed;
}
int Cook::getMaxspeed()
{
	return Maxspeed;
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