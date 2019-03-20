#pragma once
#include <vector>

class fileOpener
{
public:
	unsigned short int processID;
	std::vector<unsigned short int> positions;
	std::vector<std::pair<unsigned short int, bool>> files;

	fileOpener(){}

	fileOpener(unsigned short int processID, std::vector<std::pair<unsigned short int, bool>> files)
	{
		this->processID = processID;
		this->files = files;
		for (int i = 0; i < files.size(); i++)
			positions.push_back(0);
	}

	fileOpener(unsigned short int processID, std::vector<std::pair<unsigned short int, bool>> files, std::vector<unsigned short int> positions)
	{
		this->processID = processID;
		this->files = files;
		this->positions = positions;
	}

	void increasePosition(unsigned int position, unsigned int number)
	{
		this->positions[number] += position;
	}

	void decreasePosition(unsigned int position, unsigned int number)
	{
		this->positions[number] -= position;
	}

	void setPosition(unsigned int position, unsigned int number)
	{
		this->positions[number] = position;
	}
};