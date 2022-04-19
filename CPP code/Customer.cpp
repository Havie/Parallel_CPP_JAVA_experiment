#include "Customer.h"


Customer::Customer()
{
	setID("");
	setBalance(0);
}

Customer::Customer(std::string id, double balance)
{
	setID(id);
	setBalance(balance);
}

std::string Customer::GetID()
{
	return _id;
}

double Customer::GetBalance()
{
	return _balance;
}

void Customer::setID(std::string id)
{
	_id = id;
}

void Customer::setBalance(double balance)
{
	_balance = balance;
}
