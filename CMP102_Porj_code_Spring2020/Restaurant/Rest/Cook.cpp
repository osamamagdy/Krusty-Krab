#include "Cook.h"


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


COOK_STATUS Cook::GetStatus()const
{
	return status;
}

void Cook::setStatus(COOK_STATUS st)
{
	status = st;
}