#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	static int num_of_orderautopormoted;	//this is only for Normal order 
	                
	double totalMoney;	//Total order money

	int ArrTime, ServTime, FinishTime;	//arrival, service start, and finish times
	static int VIP_WT;
	static int orderpromted;
	//
	// TODO: Add More Data Members As Needed

	int WaitTime;  // The time the order waits before bieng assigned to the cook
	int OrderSize; // How many Dishes in the order
	int Priority;  // The Priorits of servicing the order (used only for VIP orders)
	//for outputfile 
	static int Ordercount;
	static int Vordercount;
	static int Nordercount;
	static int Gordercount;

public:
	Order(int ID, ORD_TYPE r_Type);
	
	
	Order(int ID, ORD_TYPE r_Type, double money, int size, int timestep);
	void increase_promotion();
	int GetAUto();
	int GetIncrese();
	virtual ~Order();

	int GetID();

	ORD_TYPE GetType() const;

	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;
	static void SetVIP_WT(int vip);
	static int getVIP_WT();
	//
	// TODO: Add More Member Functions As Needed

	void CalPriority();
	int getPriority();

	void SetArr(int timestep);

	void IncSer();
	int IncWait();
	void CalFinish();
	void SetSize(int size);
	void AddMoney(double extra);
	static void setautopormotion(int autop);
	void SetType(ORD_TYPE type);
	void setOrderSize(int size);
	int getOrderSize();
	void setwaittime(int time);
	void setservicetime(int time);
	int getFinshtime();
	int getorderarrivaltime();
	int getwaittime();
	int getservicetime();
	static int getordercount();
	static int getVordercount();
	static int getNordercount();
	static int getGordercount();
};

#endif
