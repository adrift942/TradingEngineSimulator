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
		std::string type;
		switch (orderUpdate.orderUpdateType)
		{
		case OrderUpdateType::Open:
			type = "open";
			break;
		case OrderUpdateType::Filled:
			type = "filled";
			break;
		case OrderUpdateType::PartiallyFilled:
			type = "partially filled";
			break;
		case OrderUpdateType::Amended:
			type = "amended";
			break;
		case OrderUpdateType::Canceled:
			type = "canceled";
			break;
		}

		os << type << " " << orderUpdate.order;
		return os;
	}
};