#pragma once
#include <string>
#include <iostream>

struct Ack
{
	bool success;
	std::string message;

	friend std::ostream& operator<<(std::ostream& os, const Ack& ack)
	{
		std::string res = ack.success ? "Success: " : "Error: ";
		os << res << ack.message;
		return os;
	}
};

