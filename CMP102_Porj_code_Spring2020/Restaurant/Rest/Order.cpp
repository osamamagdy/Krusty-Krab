#include "Order.h"

int Order::orderautopormotion = 0;

Order::Order(int id, ORD_TYPE r_Type)
{
	Priority = 0;
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = r_Type;
	status = WAIT;
}

Order::Order(int id, ORD_TYPE r_Type, double money, int size, int timestep,int VIpwait)
{
	Priority = 0;
	totalMoney = money;
	OrderSize = size;
	ArrTime = timestep;
	ID = (id > 0 && id < 1000) ? id : 0;	//1<ID<999
	type = r_Type;
	status = WAIT;
	if (type == TYPE_VIP)
	{
		VIP_WT = VIpwait > 0 ? VIpwait : 0;
	}
}
void Order::SetType(ORD_TYPE type)
{
	this->type = type;
}
void Order::increase_promotion()
{
	orderautopormotion++;
}
int Order::GetAUto()
{
	return orderpromted;
}
int Order::GetIncrese()
{
	return orderautopormotion;
}
Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}


ORD_TYPE Order::GetType() const
{
	return type;
}

void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}


void Order::CalPriority()
{
	Priority = (2 * OrderSize) + (int(totalMoney) / 100) - ArrTime;
}

int Order:: getPriority()
{
	return Priority ;
}


void Order :: SetArr(int timestep)
{
	ArrTime = timestep;
}
void Order::IncSer()
{
	ServTime++;
}
void Order ::IncWait()
{
	WaitTime++;
}
void Order:: CalFinish()
{
	FinishTime = ArrTime + WaitTime + ServTime;
}
void Order::SetSize(int size)
{
	OrderSize = size;
}

void Order :: AddMoney(double extra)
{
	totalMoney = totalMoney + extra;
}

void Order::setautopormotion(int autop)
{
	orderpromted = autop;
}


int Order::orderpromted = 0;




