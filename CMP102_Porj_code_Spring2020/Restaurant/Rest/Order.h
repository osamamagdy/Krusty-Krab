#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	static int orderautopormotion;	//this is only for Normal order 
	                
	double totalMoney;	//Total order money

	int ArrTime, ServTime, FinishTime;	//arrival, service start, and finish times
	
	
	//
	// TODO: Add More Data Members As Needed

	int WaitTime;  // The time the order waits before bieng assigned to the cook
	int OrderSize; // How many Dishes in the order
	int Priority;  // The Priorits of servicing the order (used only for VIP orders)
	//

public:
	Order(int ID, ORD_TYPE r_Type);
	
	
	Order(int ID, ORD_TYPE r_Type, double money, int size, int timestep);


	virtual ~Order();

	int GetID();

	ORD_TYPE GetType() const;

	void SetDistance(int d);
	int GetDistance() const;

	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;
	
	//
	// TODO: Add More Member Functions As Needed

	void CalPriority();
	int getPriority();
	void SetArr(int timestep);
	void IncSer();
	void IncWait();
	void CalFinish();
	void SetSize(int size);
	void AddMoney(double extra);
	static void setautopormotion(int autop);
	
	
	
	//

};

#endif