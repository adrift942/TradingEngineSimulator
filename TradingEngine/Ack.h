#pragma once
#include <string>
#include <iostream>

struct Ack
{
	bool success;
	std::string message;

	friend std::ostream& operator<<(std::ostream& os, const Ack& ack)
	{
		if (!ack.success)
			os << "Error: ";
		else
			os << "Success: ";

		os << ack.message;
		return os;
	}
};

