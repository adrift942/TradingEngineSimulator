#pragma once
#include "Common.h"


struct Order
{
	Order() {};
	Order(bool isBuy, float price, float amount) : isBuy{ isBuy }, price{ price }, amount{ amount }, unfilledAmount{ amount } {};

	OrderId id;
	bool isBuy;
	float price;
	float amount;
	float unfilledAmount;

	bool operator==(Order& other) const
	{
		return id == other.id && isBuy == other.isBuy && price == other.price &&
			amount == other.amount && unfilledAmount == other.unfilledAmount;
	}

	bool operator!=(Order& other) const
	{
		return !(*this == other);
	}
};

