#pragma once
#include <string>
#include <iostream>
#include <vector>

class FEntry
{
protected:

	char number;
	char* name;

public:

	FEntry() {}

	FEntry(int number, char* name)
	{
		this->number = number;
		this->name = new char[8];
		for (int i = 0; i < 8; i++)
			this->name[i] = name[i];
	}

	FEntry(int number, std::string name2)
	{
		this->number = (char)number;
		char* name3 = (char*)name2.c_str();
		this->name = name3;
	}

	FEntry(int number, std::vector<char> name)
	{
		this->number = number;
		this->number = number;
		for (int i = 0; i < 8; i++)
			this->name[i] = name[i];
	}

	char getNumber()
	{
		return number;
	}

	char* getName()
	{
		return name;
	}

	std::string toData()
	{
		char polaczone[8] = { number, name[0], name[1] , name[2] , name[3] , name[4]  ,name[5]  ,name[6] };
		std::string buff = "........";
		for (int i = 0; i < 8; i++)
		{
			buff[i] = polaczone[i];
		}
		return buff;
	}

};

std::ostream& operator << (std::ostream& stream, FEntry& entry)
{
	return stream << entry.getName() << ": " << (int)entry.getNumber() << std::endl;
}