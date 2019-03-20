#pragma once

#include "fafms.h"
#include "Inode.h"
#include "translator.h"
#include <vector>

extern char disc[1024];

unsigned short int dWrite(std::vector<char> data, std::vector<unsigned short int> &blocks, short int startpoint)
{
	if (data.size() > ((blocks.size() * 32) - startpoint) || blocks.size() < 1 || startpoint < 0 || startpoint > 31)
	{
		return 0;
	}
	else
	{
		unsigned short int executed = 0;
		short int antistartpoint = 31 - startpoint; 
		short int howmany = data.size(); 
		for (int i = 0; i < antistartpoint; i++)
		{
			if (howmany <= 0)
			{
				break;
			}
			disc[blockToPhysical(blocks[0], 31 + i - antistartpoint)] = data[data.size() - howmany];
			howmany--;
			executed++;
		}
		for (int i = 1; i < blocks.size(); i++)
		{
			if (howmany <= 0)
				break;
			for (int j = 0; j < 32; j++)
			{
				if (howmany <= 0)
					break;
				disc[blockToPhysical(blocks[i], j)] = data[data.size() - howmany];
				howmany--;
				executed++;
			}
		}
		return executed;
	}
}

unsigned short int dRead(char* &data, std::vector<unsigned short int> &blocks, short int startpoint, unsigned short int howmany)
{
	if (blocks.size() > 32 || startpoint > 31 || startpoint < 0 || howmany > 1024)
		return 0;
	else
	{
		howmany += 1;
		std::vector<char> readBytes;
		short int antistartpoint = 31 - startpoint;
		if (howmany > (blocks.size() * 32) - startpoint)
			howmany = (blocks.size() * 32) - startpoint;
		for (int i = 0; i < antistartpoint; i++)
		{
			if (howmany <= 0)
				break;
			readBytes.push_back(disc[blockToPhysical(blocks[0], 31 + i - antistartpoint)]);
			howmany--;
		}
		for (int i = 1; i < blocks.size(); i++)
		{
			if (howmany <= 0)
				break;
			for (int j = 0; j < 32; j++)
			{
				if (howmany <= 0)
					break;
				readBytes.push_back(disc[blockToPhysical(blocks[i], j)]);
				howmany--;
			}
		}
		data = new char[readBytes.size()];
		for (int i = 0; i < readBytes.size(); i++)
		{
			data[i] = readBytes[i];
		}
		return readBytes.size();
	}
}
