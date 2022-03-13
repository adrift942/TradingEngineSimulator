#pragma once
#include "Common.h"

enum class OrderUpdateType { Open, Filled, PartiallyFilled, Amended, Canceled };

struct OrderUpdate
{
	ClientId clientId;
	OrderUpdateType orderUpdateType;
	Order order;

	friend std::ostream& operator<<(std::ostream& os, const OrderUpdate& orderUpdate)
	{
		std::string msg;
		switch (orderUpdate.orderUpdateType)
		{
		case OrderUpdateType::Filled:
			msg = "filled";
			break;
		case OrderUpdateType::PartiallyFilled:
			msg = "partially filled";
			break;
		case OrderUpdateType::Amended:
			msg = "amended";
			break;
		case OrderUpdateType::Canceled:
			msg = "canceled";
			break;
		}

		os << msg;
		return os;
	}
};