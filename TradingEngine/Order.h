#pragma once
#include "Common.h"
#include <iostream>


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

	friend std::ostream& operator<<(std::ostream& os, const Order& order)
	{
		os << "Order ID " << order.id;
		std::string side = order.isBuy ? "buy" : "sell";
		os << " side " << side << ", price " << order.price << ", amount " << order.amount;
		return os;
	}
};

