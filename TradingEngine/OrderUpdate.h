#pragma once
#include "Common.h"

enum OrderUpdateType { filled, partially_filled, amended, canceled };

struct OrderUpdate
{
	ClientId clientId;
	OrderUpdateType orderUpdateType;
	Order order;
};