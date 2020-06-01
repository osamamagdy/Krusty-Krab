#include "Cook.h"

int Cook::OrderstoBreak = 0;
float Cook::InjProp = 0;
int Cook::RstPrd = 0;
int Cook::Vcount = 0;
int Cook::Gcount = 0;
int Cook::Ncount = 0;
int Cook::Available_Vcount = 0;
int Cook::Available_Gcount = 0;
int Cook::Available_Ncount = 0;
int Cook::num_injure_cook = 0;


Cook::Cook()
{
	IsInjured = false;
	N_orders_Finished = 0;
}

int Cook::getOriginalSpeed()
{
	return OriginalSpeed;
}


void Cook::setOriginalSpeed(int sp)
{
	OriginalSpeed = sp;
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
	OrderstoBreak = ordtobreak > 0 ? ordtobreak : 0;
}

int Cook::get_order_to_break()
{
	return OrderstoBreak;
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
void Cook::CalUnavailabalePriority()
{
	Unavailabalepriority = 1000 - Timesteptobeavailabale;
		
		
}

int Cook::getUnavailabalePriority()
{
	return Unavailabalepriority;
}

void Cook::setVcount(int count)
{
	if (count > 0)
		Vcount = count;
	else
		Vcount = 0;
}

void Cook::setNcount(int count)
{
	Ncount = count > 0 ? count : 0;

}

void Cook::setGcount(int count)
{
	Gcount = count > 0 ? count : 0;
}

int Cook::GetVcount()
{
	return Vcount;
}

int Cook::GetNcount()
{
	return Ncount;
}

int Cook::GetGcount()
{
	return Gcount;
}


void Cook::setAvailableVcount(int count)
{
	Available_Vcount = count > 0 ? count : 0;
}

void Cook::setAvailableNcount(int count)
{
	Available_Ncount = count > 0 ? count : 0;
}

void Cook::setAvailableGcount(int count)
{
	Available_Gcount = count > 0 ? count : 0;
}

int Cook::GetAvailableVcount()
{
	return Available_Vcount;
}

int Cook::GetAvailableNcount()
{
	return Available_Ncount;
}

int Cook::GetAvailableGcount()
{
	return Available_Gcount;
}


int Cook::Getcookscount()
{
	return( Vcount+Gcount+Ncount);
}

void Cook::increase_injury()
{
	num_injure_cook++;
}

int Cook::get_num_of_injury()
{
	return num_injure_cook;
}



void Cook::setTimesteptobeavailabale(int time)
{
	Timesteptobeavailabale = time;
}
int Cook::getTimesteptobeavailabale()
{
	return Timesteptobeavailabale;
}
void Cook::setN_orders_Finished(int num)
{
	N_orders_Finished = num;
}
int Cook::getN_orders_Finished()
{
	return N_orders_Finished;
}


Order* Cook::getServedOrder()
{
	return ServedOrder;
}


void Cook::setServedOrder(Order* Served)
{
	ServedOrder = Served;
}


void Cook:: set_isinjured(bool val)
{
	IsInjured = val;
}

bool Cook :: get_isinjured()
{
	return IsInjured;
}