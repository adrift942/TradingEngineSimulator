#pragma once
#include "Common.h"
#include <string>
#include <iostream>

struct Ack
{
	bool success;
	std::string message;
	OrderId orderId;

	friend std::ostream& operator<<(std::ostream& os, const Ack& ack)
	{
		std::string res = ack.success ? "Success: " : "Error: ";
		os << res << ack.message;
		return os;
	}
};

