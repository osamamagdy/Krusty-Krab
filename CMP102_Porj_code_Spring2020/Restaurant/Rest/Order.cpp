#include "Order.h"

int Order::num_of_orderautopormoted = 0;
int Order::Ordercount =0 ;
int Order::Vordercount = 0;
int Order::Nordercount = 0;
int Order::Gordercount = 0;
int Order::count_Urgent = 0;

int Order::FinishedOrdersCount = 0;

int Order::orderpromted = 0;
int Order::VIP_WT = 0;


Order::Order(int id, ORD_TYPE r_Type)
{

	Priority = 0;
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = r_Type;
	status = WAIT;

	Order::Ordercount = Order::Ordercount + 1;

	switch (type)
	{
	case TYPE_NRM :
		Order::Nordercount = Order::Nordercount + 1;
		break;

	case TYPE_VGAN :
		Order::Gordercount = Order::Gordercount + 1;
		break;
	case TYPE_VIP:
		Order::Vordercount = Order::Vordercount + 1;
	}


}

Order::Order(int id, ORD_TYPE r_Type, double money, int size, int timestep)
{
	Priority = 0;
	totalMoney = money;
	OrderSize = size;
	ArrTime = timestep;
	ID = (id > 0 && id < 1000) ? id : 0;	//1<ID<999
	type = r_Type;
	status = WAIT;

	Order::Ordercount = Order::Ordercount + 1;

	switch (type)
	{
	case TYPE_NRM:
		Order::Nordercount = Order::Nordercount + 1;
		break;

	case TYPE_VGAN:
		Order::Gordercount = Order::Gordercount + 1;
		break;
	case TYPE_VIP:
		Order::Vordercount = Order::Vordercount + 1;
	}

	
}
void Order::SetType(ORD_TYPE type)
{
	this->type = type;
}
void Order::increase_promotion()
{
	num_of_orderautopormoted++;
}
int Order::GetAUto()
{
	return orderpromted;
}
int Order::Get_num_of_order_auto_P()
{
	return num_of_orderautopormoted;
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
	Priority = 100 * ArrTime + (2 * OrderSize) + (int(totalMoney) / 100);
	 
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
int Order ::IncWait()
{
	WaitTime++;
	return WaitTime -1;
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

void Order::SetVIP_WT(int vip)
{
	VIP_WT = vip > 0 ? vip : 0;
}
int Order::getVIP_WT()
{
	return VIP_WT;
}

void Order::setOrderSize(int size)
{
	OrderSize = size;
}
int Order::getOrderSize()
{
	return OrderSize;
}

void Order::setwaittime(int time)
{
	WaitTime = time;
}

void Order::setservicetime(int time)
{
	ServTime = time;
}


int Order::getFinshtime()
{
	
	return FinishTime;
}

int Order::getorderarrivaltime()
{
	return ArrTime;
}

int Order::getwaittime()
{
	return WaitTime;
}

int Order::getservicetime()
{
	return ServTime;
}

int Order::getordercount()
{
	return Ordercount;
}

int Order::getVordercount()
{
	return Vordercount;
}

int Order::getNordercount()
{
	return Nordercount;
}

int Order::getGordercount()
{
	return Gordercount;
}

void Order::increase_urgent()
{
	count_Urgent++;
}

int Order::get_Urgent_num()
{
	return count_Urgent;
}


void Order::setOrderscount(int num)
{
	Order::Ordercount = num;
}

void Order::setNOrderscount(int num)
{
	Order::Nordercount = num;
}


void Order::setVOrderscount(int num)
{
	Order::Vordercount = num;
}


void Order::setFinishedOrdersCount(int num)
{
	Order::FinishedOrdersCount = num;
}


int Order::getFinishedOrdersCount()
{
	return Order::FinishedOrdersCount;
}