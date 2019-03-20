#pragma once

class INode
{
protected:
	bool type;
	unsigned short int linkCounter = 0;
	unsigned short int size = 0;
	short int db1 = -1;
	short int db2 = -1;
	short int ib = -1;
public:

	INode(bool type)
	{
		this->type = type;
	}

	INode(){}

	bool getType()
	{
		return type;
	}
	void incrementLinkCounter()
	{
		linkCounter++;
	}
	bool decrementLinkCounter()
	{
		if (linkCounter == 0)
			return false;
		else
		{
			linkCounter--;
			return true;
		}
	}
	unsigned short int getLinkCounter()
	{
		return linkCounter;
	}
	bool setSize(unsigned short int s)
	{
		if (s > 1024)
			return false;
		else
		{
			this->size = s;
			return true;
		}
	}
	unsigned short int getSize()
	{
		return size;
	}
	bool setdb1(short int i)
	{
		if (i < -1 || i>31)
			return false;
		else
		{
			db1 = i;
			return true;
		}
	}
	short int getdb1()
	{
		return db1;
	}
	bool setdb2(short int i)
	{
		if (i < -1 || i>31)
			return false;
		else
		{
			db2 = i;
			return true;
		}
	}
	short int getdb2()
	{
		return db2;
	}
	bool setib(short int i)
	{
		if (i < -1 || i>31)
			return false;
		else
		{
			ib = i;
			return true;
		}
	}
	short int getib()
	{
		return ib;
	}
	bool increaseSize(unsigned short int howMuch)
	{
		size += howMuch;
		return true;
	}

	bool setType(bool type)
	{
		this->type = type;
		return true;
	}

};

short int findFreeINode(INode t[32])
{
	for (int i = 0; i < 32; i++)
		if (t[i].getLinkCounter() == 0)
			return i;
	return -1;
}