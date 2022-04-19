

#pragma once
#include <string>
class Customer
{
	std::string _id;
	double _balance;

public:
	Customer();
	Customer(std::string id, double balance);


	std::string GetID();
	double GetBalance();

	void setID(std::string id);
	void setBalance(double balance);

};

