#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "Inode.h"
#include "translator.h"
#include "FEntry.h"
#include "discManipulator.h"
#include "fileOpener.h"
#include "Procesor_Manager.h"
#include "ConsoleColor.h"

//Global variables of file system
char disc[1024];
INode inodeTable[32];
std::vector<std::pair<unsigned short int, unsigned short int>> openFiles; // first - iNodeNumber, second - how many processes opened it
std::vector<fileOpener> processOFT;
short int firstFreeBlock = -1;
short int drawLevel = 0;
short int processNumber = 0;

//File functions prototypes
//Shorter overload - unstable; Longer overload - stable;
bool create(std::string folder_name, std::string file_name);
bool create(std::string folder_name, std::string file_name, std::string sfolder_name);
bool unLink(std::string file_name);
bool unLink(std::string file_name, std::string &sfolder_name);
bool open(std::string file_name, bool open_type);
bool open(std::string file_name, bool open_type, std::string sfolder_name);
bool close(std::string file_name);
bool close(std::string file_name, std::string sfolder_name);
bool truncate(std::string file_name, unsigned short int final_bytes_amount);
bool truncate(std::string file_name, unsigned short int final_bytes_amount, std::string sfolder_name);
short int read(std::string file_name, char* &data, unsigned short int bytes_amount); //sequential
short int read(std::string file_name, char* &data, unsigned short int bytes_amount, std::string sfolder_name); //sequential
short int read(std::string file_name, char* &data, unsigned short int byte_number_in_file, unsigned short int bytes_amount); //index
short int read(std::string file_name, char* &data, unsigned short int byte_number_in_file, unsigned short int bytes_amount, std::string sfolder_name); //index
short int write(std::string file_name, std::vector<char> data); //sequential
short int write(std::string file_name, std::vector<char> data, std::string sfolder_name); //sequential
short int write(std::string file_name, std::vector<char> data, unsigned short int byte_number_in_file); //index
short int write(std::string file_name, std::vector<char> data, unsigned short int byte_number_in_file, std::string sfolder_name); //index
short int lseek(std::string file_name, short int move_by_amount_in_bytes, unsigned short int reference_point); // reference_point: 0 - actual position, 1 - begin, 2 - end
short int lseek(std::string file_name, short int move_by_amount_in_bytes, unsigned short int reference_point, std::string sfolder_name); // reference_point: 0 - actual position, 1 - begin, 2 - end
//Folder functions prototypes
//Shorter overload - unstable; Longer overload - stable;
bool fcreate(std::string folder_name, std::string folder_to_create_name);
bool fcreate(std::string folder_name, std::string folder_to_create_name, std::string sfolder_name);
bool fdelete(std::string folder_name);
bool fdelete(std::string folder_name, std::string sfolder_name);
bool fecreate(std::string folder_name, std::string file_name, unsigned short int inode_number);
bool fecreate(std::string folder_name, std::string file_name, unsigned short int inode_number, std::string sfolder_name);
bool fedelete(std::string file_name);
bool fedelete(std::string file_name, std::string sfolder_name);
bool frename(std::string file_name, std::string new_file_name);
bool frename(std::string file_name, std::string new_file_name, std::string sfolder_name);
std::vector<FEntry> fcontent(std::string folder_name);
std::vector<FEntry> fcontent(std::string folder_name, std::string sfolder_name);
short int fsearch(std::string file_name, unsigned short int folder_inode_number);
short int fsearch(std::string file_name, unsigned short int folder_inode_number, std::string sfolder_name);


//Utility Functions
//DO NOT use them in your modules
void allocateFreeMemory();
std::vector<short int> readIB(unsigned short int nr);
std::vector<unsigned short int> takeFreeMemory(unsigned short int numberOfBlocks);
std::vector<FEntry> extractFEntries(unsigned short int folder_inode_number);
short int acquireFreeBlock();
bool expandFile(short int INodeNumber);
bool addFEntry(short int folderInodeNumber, FEntry entry);
short int findFirstFreeInode();
void drawFolderContents(unsigned short int iNodeNumber);
bool releaseINode(unsigned short int iNodeNumber);
bool deleteFEntry(short int folderInodeNumber, std::string name);
short int whoHasThat(std::string name);
bool isAllowedToModify(std::string name, bool open_type);

//ShowOff Functions
//Those functions do not have any direct effect on file system and should be used in Shell Module
//Can also be used to debug cooperation between your module and the file system
void drawFolderContents(std::string folderName);
void drawBlock(unsigned short int block); //UNSTABLE! If there is an ENDLINE char, the whole thing rolls over!
void drawBlockD(unsigned short int block);
void drawDisc(); //UNSTABLE! If there is an ENDLINE char, the whole thing rolls over!
void drawDiscD();
void drawInode(unsigned short int inode);


//The most important function that prepares everything to work
//It MUST be called in main function of the whole system!
bool initializeFileSystem()
{
	bool inodeReset = true;
	for (int i = 0; i < 1024; i++)
		disc[i] = (char)0;
	std::cout << blue << "[SZPIK]" << white << " Wyczyszczono dysk.\n";
	for (int i = 0; i < 32; i++)
	{
		if (inodeTable[i].setSize(0) == false || inodeTable[i].setdb1(-1) == false || inodeTable[i].setdb2(-1) == false || inodeTable[i].setib(-1) == false)
			inodeReset = false;
	}
	if (inodeReset == true)
		std::cout << blue << "[SZPIK]" << white << " Poprawnie wyczyszczono I-wezly.\n";
	else
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawnie wyczyszczono I-wezly.\n" << white;
		return false;
	}
	openFiles.resize(0);
	if (openFiles.size() == 0)
		std::cout << blue << "[SZPIK]" << white << " Poprawnie wyczyszczono Systemowa Tablice Otwartych Plikow.\n";
	else
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawnie wyczyszczono Systemowa Tablice Otwartych Plikow.\n" << white;
		return false;
	}
	processOFT.resize(0);
	if (processOFT.size() == 0)
		std::cout << blue << "[SZPIK]" << white << " Poprawnie wyczyszczono Procesowa Tablice Otwartych Plikow.\n";
	else
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawnie wyczyszczono Procesowa Tablice Otwartych Plikow.\n" << white;
		return false;
	}
	inodeTable[0] = INode(true);
	inodeTable[0].setdb1(0);
	inodeTable[0].incrementLinkCounter();
	std::vector<char> rootFolderData = { (char)0, (char)46 };
	std::vector<unsigned short int> blocks = { 0 };
	dWrite(rootFolderData, blocks, 0);
	rootFolderData = { (char)32 };
	dWrite(rootFolderData, blocks, 8);
	dWrite(rootFolderData, blocks, 16);
	dWrite(rootFolderData, blocks, 24);
	inodeTable[0].setSize(8);
	allocateFreeMemory();
	if (firstFreeBlock > 0)
		std::cout << blue << "[SZPIK]" << white << " Utworzono folder nadrzedny.\n";
	std::cout << blue << "[SZPIK]" << white << " Pierwszy wolny blok dyskowy: " << firstFreeBlock << std::endl;
	return true;
}

//Functions Definitions
void allocateFreeMemory()
{
	bool freeBlocks[32];
	for (int i = 0; i < 32; i++)
		freeBlocks[i] = true;
	for (int i = 0; i < 32; i++)
		if (inodeTable[i].getLinkCounter() > 0)
		{
			if (inodeTable[i].getdb1() > -1)
				freeBlocks[inodeTable[i].getdb1()] = false;
			if (inodeTable[i].getdb2() > -1)
				freeBlocks[inodeTable[i].getdb2()] = false;
			if (inodeTable[i].getib() > -1)
			{
				freeBlocks[inodeTable[i].getib()] = false;
				std::vector<short int> ibContent = readIB(inodeTable[i].getib());
				for (int j = 0; j < ibContent.size(); j++)
					freeBlocks[ibContent[j]] = false;
			}
		}
	std::vector<unsigned short int> FreeB;
	for (int i = 0; i < 32; i++)
		if (freeBlocks[i] == true)
			FreeB.push_back(i);
	if (FreeB.size() == 0)
	{
		firstFreeBlock = -1;
		return;
	}
	else
	{
		std::sort(FreeB.begin(), FreeB.end());
		firstFreeBlock = FreeB[0];
		std::vector<char>FreeIBcontents;
		for (int i = 1; i < FreeB.size(); i++)
		{
			if (FreeB[i] != 0 && FreeB[i] < 32)
				FreeIBcontents.push_back((char)FreeB[i]);
		}
		for (int i = FreeB.size(); i < 33; i++)
		{
			FreeIBcontents.push_back((char)32);
		}
		for (int i = FreeB.size(); i < 31; i++)
			FreeIBcontents[i] = (char)32;
		std::vector<unsigned short> buff = { (unsigned short)firstFreeBlock };
		dWrite(FreeIBcontents, buff, 0);
	}
}

std::vector<short int> readIB(unsigned short int nr)
{
	std::vector<unsigned short int> blockToRead = { nr };
	char* blockContents = new char[32];
	dRead(blockContents, blockToRead, 0, 32);
	std::vector<short int> blockContentsToReturn;
	for (int i = 0; i < 32; i++)
		if ((int)blockContents[i] >= 0 && (int)blockContents[i] < 32)
			blockContentsToReturn.push_back((int)blockContents[i]);
	return blockContentsToReturn;
}

std::vector<unsigned short int> takeFreeMemory(unsigned short int numberOfBlocks)
{
	std::vector<unsigned short int> blocksToTake;
	if (firstFreeBlock == -1)
		return blocksToTake;
	else
	{
		std::vector<short int> freeBlocks = readIB(firstFreeBlock);
		if (freeBlocks[0] == 32)
		{
			blocksToTake.push_back(firstFreeBlock);
			firstFreeBlock = -1;
			return blocksToTake;
		}
		for (int i = 0; i < numberOfBlocks; i++)
			for (int j = 0; j < 32; j++)
			{
				if (j == 31 && freeBlocks[j] != 32)
				{
					blocksToTake.push_back(freeBlocks[j]);
					break;
				}
				else
				{
					if (freeBlocks[j + 1] == 32)
					{
						blocksToTake.push_back(freeBlocks[j]);
						break;
					}
				}
			}
	}
}

std::vector<FEntry> extractFEntries(unsigned short int folder_inode_number)
{
	std::vector<FEntry> extractedFEntries;
	if (inodeTable[folder_inode_number].getdb1() != -1)
	{
		for (int i = 0; i < 4; i++)
		{
			if ((int)disc[(inodeTable[folder_inode_number].getdb1() * 32) + (i * 8)] >= 0 && (int)disc[(inodeTable[folder_inode_number].getdb1() * 32) + (i * 8)] < 32)
			{
				char number = disc[(inodeTable[folder_inode_number].getdb1() * 32) + (i * 8)];
				std::vector<char> name;
				for (int j = 1; j < 8; j++)
				{
					if ((int)disc[(inodeTable[folder_inode_number].getdb1() * 32) + (i * 8) + j] > 0)
					{
						name.push_back(disc[(inodeTable[folder_inode_number].getdb1() * 32) + (i * 8) + j]);
					}
					else
						break;
				}
				if (name.size() > 0)
				{
					char* buff = new char[7];
					for (int i = 0; i < name.size(); i++)
					{
						buff[i] = name[i];
					}
					for (int i = name.size(); i < 8; i++)
						buff[i] = (char)0;
					FEntry buff2((int)number, buff);
					extractedFEntries.push_back(buff2);
				}
			}
		}
	}
	if (inodeTable[folder_inode_number].getdb2() != -1)
	{
		for (int i = 0; i < 4; i++)
		{
			if ((int)disc[(inodeTable[folder_inode_number].getdb2() * 32) + (i * 8)] >= 0 && (int)disc[(inodeTable[folder_inode_number].getdb2() * 32) + (i * 8)] < 32)
			{
				char number = disc[(inodeTable[folder_inode_number].getdb2() * 32) + (i * 8)];
				std::vector<char> name;
				for (int j = 1; j < 8; j++)
				{
					if ((int)disc[(inodeTable[folder_inode_number].getdb2() * 32) + (i * 8) + j] > 0)
					{
						name.push_back(disc[(inodeTable[folder_inode_number].getdb2() * 32) + (i * 8) + j]);
					}
					else
						break;
				}
				if (name.size() > 0)
				{
					char* buff = new char[7];
					for (int i = 0; i < name.size(); i++)
					{
						buff[i] = name[i];
					}
					for (int i = name.size(); i < 8; i++)
						buff[i] = (char)0;
					FEntry buff2((int)number, buff);
					extractedFEntries.push_back(buff2);
				}
			}
		}
	}
	if (inodeTable[folder_inode_number].getib() != -1)
	{
		std::vector<short int> readBlocks = readIB(inodeTable[folder_inode_number].getib());
		for (int i = 0; i < readBlocks.size(); i++)
		{
			if (readBlocks[i] < 0 || readBlocks[i] > 31)
				readBlocks.erase(readBlocks.begin() + i);
		}
		for (int k = 0; k < readBlocks.size(); k++)
		{
			for (int i = 0; i < 4; i++)
			{
				if ((int)disc[(readBlocks[k] * 32) + (i * 8)] >= 0 && (int)disc[(readBlocks[k] * 32) + (i * 8)] < 32)
				{
					char number = disc[(readBlocks[k] * 32) + (i * 8)];
					std::vector<char> name;
					for (int j = 1; j < 8; j++)
					{
						if ((int)disc[(readBlocks[k] * 32) + (i * 8) + j] > 0)
						{
							name.push_back(disc[(readBlocks[k] * 32) + (i * 8) + j]);
						}
						else
							break;
					}
					if (name.size() > 0)
					{
						char* buff = new char[7];
						for (int i = 0; i < name.size(); i++)
						{
							buff[i] = name[i];
						}
						for (int i = name.size(); i < 8; i++)
							buff[i] = (char)0;
						FEntry buff2((int)number, buff);
						extractedFEntries.push_back(buff2);
					}
				}
			}
		}
	}
	return extractedFEntries;
}

short int fsearch(std::string file_name, unsigned short int folder_inode_number)
{
	if (file_name == "ROOT" || file_name == "root")
		return 0;
	if (file_name == "." || file_name == "..")
	{
		return -1;
	}
	else
	{
		std::vector<FEntry> extractedFEntries = extractFEntries(folder_inode_number);
		for (int i = 0; i < extractedFEntries.size(); i++)
		{
			if (extractedFEntries[i].getName() == file_name)
				return extractedFEntries[i].getNumber();
		}
		for (int i = 0; i < extractedFEntries.size(); i++)
		{
			if (inodeTable[extractedFEntries[i].getNumber()].getType() == true)
			{
				if (extractedFEntries[i].getName()[0] == '.')
				{

				}
				else
				{
					short int result = fsearch(file_name, extractedFEntries[i].getNumber());
					if (result != -1)
						return result;
				}
			}
		}
		return -1;
	}
}

short int fsearch(std::string file_name, unsigned short int folder_inode_number, std::string sfolder_name)
{
	if (file_name == "ROOT" || file_name == "root")
		return 0;
	if (file_name == "." || file_name == "..")
	{
		return -1;
	}
	else
	{
		short int sfolder = fsearch(sfolder_name, folder_inode_number);
		if (sfolder < 0 || sfolder > 31)
			return -1;
		else
		{
			return fsearch(file_name, sfolder);
		}
	}
}

short int acquireFreeBlock()
{
	allocateFreeMemory();
	if (firstFreeBlock < 0)
		return -1;
	else
	{
		std::vector<short int> contents = readIB(firstFreeBlock);
		if (contents.size() < 1)
		{
			short int buff = firstFreeBlock;
			firstFreeBlock = -1;
			return buff;
		}
		else
		{
			short int buff = contents[contents.size() - 1];
			std::vector<char> data = { (char)32 };
			std::vector<unsigned short> blocks = { (unsigned short)firstFreeBlock };
			dWrite(data, blocks, 32 - contents.size());
			return buff;
		}
	}
}

bool expandFile(short int INodeNumber)
{

	if (inodeTable[INodeNumber].getdb1() == -1)
	{
		short int buff = acquireFreeBlock();
		if (buff < 0 || buff > 31)
			return false;
		else
		{
			inodeTable[INodeNumber].setdb1(buff);
			std::vector<char> data;
			std::vector<unsigned short> blocks = { (unsigned short)buff };
			for (int i = 0; i < 32; i++)
				data.push_back((char)0);
			dWrite(data, blocks, 0);
			allocateFreeMemory();
			return true;
		}
	}
	else
	{
		if (inodeTable[INodeNumber].getdb2() == -1)
		{
			short int buff = acquireFreeBlock();
			if (buff < 0 || buff > 31)
				return false;
			else
			{
				inodeTable[INodeNumber].setdb2(buff);
				std::vector<char> data;
				std::vector<unsigned short> blocks = { (unsigned short)buff };
				for (int i = 0; i < 32; i++)
					data.push_back((char)0);
				dWrite(data, blocks, 0);
				allocateFreeMemory();
				return true;
			}
		}
		else
		{
			if (inodeTable[INodeNumber].getib() == -1)
			{
				short int buff = acquireFreeBlock();
				if (buff < 0 || buff > 31)
					return false;
				else
				{
					inodeTable[INodeNumber].setib(buff);
					std::vector<char> data;
					std::vector<unsigned short> blocks = { (unsigned short)buff };
					for (int i = 0; i < 32; i++)
						data.push_back((char)32);
					dWrite(data, blocks, 0);
					allocateFreeMemory();
					return true;
				}
			}
			else
			{
				std::vector<short int> contents = readIB(inodeTable[INodeNumber].getib());
				if (contents.size() == 32)
					return false;
				else
				{
					short int buff = acquireFreeBlock();
					if (buff < 0 || buff > 31)
						return false;
					else
					{
						std::vector<char> data = { (char)buff };
						std::vector<unsigned short int> blocks = { (unsigned short int)inodeTable[INodeNumber].getib() };
						dWrite(data, blocks, contents.size());
						blocks = { (unsigned short)buff };
						data.resize(0);
						for (int i = 0; i < 32; i++)
							data.push_back((char)0);
						dWrite(data, blocks, 0);
						allocateFreeMemory();
						return true;
					}
				}
			}
		}
	}
}

bool addFEntry(short int folderInodeNumber, FEntry entry)
{
	std::vector<FEntry> FEntries = extractFEntries(folderInodeNumber);
	for (int i = 0; i < FEntries.size(); i++)
		if (FEntries[i].getName() == entry.getName())
			return false;
	if (inodeTable[folderInodeNumber].getSize() % 32 == 0)
	{
		if (expandFile(folderInodeNumber))
		{
			float HowToStart = (float)inodeTable[folderInodeNumber].getSize() / 32.0;
			if (HowToStart <= 2)
			{
				if (HowToStart <= 1)
				{
					std::vector<unsigned short> blocks;
					if (inodeTable[folderInodeNumber].getdb1() > -1)
						blocks = { (unsigned short)inodeTable[folderInodeNumber].getdb1() };
					if (inodeTable[folderInodeNumber].getdb2() > -1)
						blocks = { (unsigned short)inodeTable[folderInodeNumber].getdb2() };
					if (inodeTable[folderInodeNumber].getib() > -1)
					{
						std::vector<short> IB = readIB(inodeTable[folderInodeNumber].getib());
						blocks = { (unsigned short)IB[IB.size() - 1] };
					}
					short int HowToStart2 = (inodeTable[folderInodeNumber].getSize() % 32);
					if (HowToStart2 < 8)
					{
						std::string buff = entry.toData();
						std::vector<char> data;
						for (int i = 0; i < buff.size(); i++)
							data.push_back(buff[i]);
						dWrite(data, blocks, 0);
						inodeTable[folderInodeNumber].increaseSize(8);
						allocateFreeMemory();
						return true;
					}
					if (HowToStart2 >= 8 && HowToStart2 < 16)
					{
						std::string buff = entry.toData();
						std::vector<char> data;
						for (int i = 0; i < buff.size(); i++)
							data.push_back(buff[i]);
						dWrite(data, blocks, 8);
						inodeTable[folderInodeNumber].increaseSize(8);
						allocateFreeMemory();
						return true;
					}
					if (HowToStart2 >= 16 && HowToStart2 < 24)
					{
						std::string buff = entry.toData();
						std::vector<char> data;
						for (int i = 0; i < buff.size(); i++)
							data.push_back(buff[i]);
						dWrite(data, blocks, 16);
						inodeTable[folderInodeNumber].increaseSize(8);
						allocateFreeMemory();
						return true;
					}
					if (HowToStart2 >= 24)
					{
						std::string buff = entry.toData();
						std::vector<char> data;
						for (int i = 0; i < buff.size(); i++)
							data.push_back(buff[i]);
						dWrite(data, blocks, 24);
						inodeTable[folderInodeNumber].increaseSize(8);
						allocateFreeMemory();
						return true;
					}
				}
				else
				{
					std::vector<unsigned short> blocks;
					if (inodeTable[folderInodeNumber].getdb1() > -1)
						blocks = { (unsigned short)inodeTable[folderInodeNumber].getdb1() };
					if (inodeTable[folderInodeNumber].getdb2() > -1)
						blocks = { (unsigned short)inodeTable[folderInodeNumber].getdb2() };
					if (inodeTable[folderInodeNumber].getib() > -1)
					{
						std::vector<short> IB = readIB(inodeTable[folderInodeNumber].getib());
						blocks = { (unsigned short)IB[IB.size() - 1] };
					}
					short int HowToStart2 = (inodeTable[folderInodeNumber].getSize() % 32);
					if (HowToStart2 < 8)
					{
						std::string buff = entry.toData();
						std::vector<char> data;
						for (int i = 0; i < buff.size(); i++)
							data.push_back(buff[i]);
						dWrite(data, blocks, 0);
						inodeTable[folderInodeNumber].increaseSize(8);
						allocateFreeMemory();
						return true;
					}
					if (HowToStart2 >= 8 && HowToStart2 < 16)
					{
						std::string buff = entry.toData();
						std::vector<char> data;
						for (int i = 0; i < buff.size(); i++)
							data.push_back(buff[i]);
						dWrite(data, blocks, 8);
						inodeTable[folderInodeNumber].increaseSize(8);
						allocateFreeMemory();
						return true;
					}
					if (HowToStart2 >= 16 && HowToStart2 < 24)
					{
						std::string buff = entry.toData();
						std::vector<char> data;
						for (int i = 0; i < buff.size(); i++)
							data.push_back(buff[i]);
						dWrite(data, blocks, 16);
						inodeTable[folderInodeNumber].increaseSize(8);
						allocateFreeMemory();
						return true;
					}
					if (HowToStart2 >= 24)
					{
						std::string buff = entry.toData();
						std::vector<char> data;
						for (int i = 0; i < buff.size(); i++)
							data.push_back(buff[i]);
						dWrite(data, blocks, 24);
						inodeTable[folderInodeNumber].increaseSize(8);
						allocateFreeMemory();
						return true;
					}
				}
			}
			else
			{
				std::vector<short int> contents = readIB(inodeTable[folderInodeNumber].getib());
				if (contents.size() < 1)
				{
					if (expandFile(folderInodeNumber))
					{
						contents = readIB(inodeTable[folderInodeNumber].getib());
						std::vector<unsigned short> blocks = { (unsigned short)contents[contents.size() - 1] };
						std::string buff = entry.toData();
						std::vector<char> data;
						for (int i = 0; i < buff.size(); i++)
							data.push_back(buff[i]);
						dWrite(data, blocks, 24);
						inodeTable[folderInodeNumber].increaseSize(8);
						allocateFreeMemory();
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					std::vector<unsigned short> blocks = { (unsigned short)contents[contents.size() - 1] };
					std::string buff = entry.toData();
					std::vector<char> data;
					for (int i = 0; i < buff.size(); i++)
						data.push_back(buff[i]);
					dWrite(data, blocks, 24);
					inodeTable[folderInodeNumber].increaseSize(8);
					allocateFreeMemory();
					return true;
				}
			}
		}
		else
		{
			std::cout << blue << "[SZPIK]" << red << " Problem z poszerzaniem folderu." << white << std::endl;
			return false;
		}
	}
	else
	{
		float HowToStart = (float)inodeTable[folderInodeNumber].getSize() / 32.0;
		if (HowToStart <= 2)
		{
			if (HowToStart <= 1)
			{
				std::vector<unsigned short> blocks;
				if (inodeTable[folderInodeNumber].getdb1() > -1)
					blocks = { (unsigned short)inodeTable[folderInodeNumber].getdb1() };
				if (inodeTable[folderInodeNumber].getdb2() > -1)
					blocks = { (unsigned short)inodeTable[folderInodeNumber].getdb2() };
				if (inodeTable[folderInodeNumber].getib() > -1)
				{
					std::vector<short> IB = readIB(inodeTable[folderInodeNumber].getib());
					blocks = { (unsigned short)IB[IB.size() - 1] };
				}
				short int HowToStart2 = (inodeTable[folderInodeNumber].getSize() % 32);
				if (HowToStart2 < 8)
				{
					std::string buff = entry.toData();
					std::vector<char> data;
					for (int i = 0; i < buff.size(); i++)
						data.push_back(buff[i]);
					dWrite(data, blocks, 0);
					inodeTable[folderInodeNumber].increaseSize(8);
					allocateFreeMemory();
					return true;
				}
				if (HowToStart2 >= 8 && HowToStart2 < 16)
				{
					std::string buff = entry.toData();
					std::vector<char> data;
					for (int i = 0; i < buff.size(); i++)
						data.push_back(buff[i]);
					dWrite(data, blocks, 8);
					inodeTable[folderInodeNumber].increaseSize(8);
					allocateFreeMemory();
					return true;
				}
				if (HowToStart2 >= 16 && HowToStart2 < 24)
				{
					std::string buff = entry.toData();
					std::vector<char> data;
					for (int i = 0; i < buff.size(); i++)
						data.push_back(buff[i]);
					dWrite(data, blocks, 16);
					inodeTable[folderInodeNumber].increaseSize(8);
					allocateFreeMemory();
					return true;
				}
				if (HowToStart2 >= 24)
				{
					std::string buff = entry.toData();
					std::vector<char> data;
					for (int i = 0; i < buff.size(); i++)
						data.push_back(buff[i]);
					dWrite(data, blocks, 24);
					inodeTable[folderInodeNumber].increaseSize(8);
					allocateFreeMemory();
					return true;
				}
			}
			else
			{
				std::vector<unsigned short> blocks;
				if (inodeTable[folderInodeNumber].getdb1() > -1)
					blocks = { (unsigned short)inodeTable[folderInodeNumber].getdb1() };
				if (inodeTable[folderInodeNumber].getdb2() > -1)
					blocks = { (unsigned short)inodeTable[folderInodeNumber].getdb2() };
				if (inodeTable[folderInodeNumber].getib() > -1)
				{
					std::vector<short> IB = readIB(inodeTable[folderInodeNumber].getib());
					blocks = { (unsigned short)IB[IB.size() - 1] };
				}
				short int HowToStart2 = (inodeTable[folderInodeNumber].getSize() % 32);
				if (HowToStart2 < 8)
				{
					std::string buff = entry.toData();
					std::vector<char> data;
					for (int i = 0; i < buff.size(); i++)
						data.push_back(buff[i]);
					dWrite(data, blocks, 0);
					inodeTable[folderInodeNumber].increaseSize(8);
					allocateFreeMemory();
					return true;
				}
				if (HowToStart2 >= 8 && HowToStart2 < 16)
				{
					std::string buff = entry.toData();
					std::vector<char> data;
					for (int i = 0; i < buff.size(); i++)
						data.push_back(buff[i]);
					dWrite(data, blocks, 8);
					inodeTable[folderInodeNumber].increaseSize(8);
					allocateFreeMemory();
					return true;
				}
				if (HowToStart2 >= 16 && HowToStart2 < 24)
				{
					std::string buff = entry.toData();
					std::vector<char> data;
					for (int i = 0; i < buff.size(); i++)
						data.push_back(buff[i]);
					dWrite(data, blocks, 16);
					inodeTable[folderInodeNumber].increaseSize(8);
					allocateFreeMemory();
					return true;
				}
				if (HowToStart2 >= 24)
				{
					std::string buff = entry.toData();
					std::vector<char> data;
					for (int i = 0; i < buff.size(); i++)
						data.push_back(buff[i]);
					dWrite(data, blocks, 24);
					inodeTable[folderInodeNumber].increaseSize(8);
					allocateFreeMemory();
					return true;
				}
			}
		}
		else
		{
			std::vector<short int> contents = readIB(inodeTable[folderInodeNumber].getib());
			if (contents.size() < 1)
			{
				if (expandFile(folderInodeNumber))
				{
					contents = readIB(inodeTable[folderInodeNumber].getib());
					std::vector<unsigned short> blocks = { (unsigned short)contents[contents.size() - 1] };
					std::string buff = entry.toData();
					std::vector<char> data;
					for (int i = 0; i < buff.size(); i++)
						data.push_back(buff[i]);
					dWrite(data, blocks, 24);
					inodeTable[folderInodeNumber].increaseSize(8);
					allocateFreeMemory();
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				std::vector<unsigned short> blocks = { (unsigned short)contents[contents.size() - 1] };
				std::string buff = entry.toData();
				std::vector<char> data;
				for (int i = 0; i < buff.size(); i++)
					data.push_back(buff[i]);
				dWrite(data, blocks, 24);
				inodeTable[folderInodeNumber].increaseSize(8);
				allocateFreeMemory();
				return true;
			}
		}
	}
}

short int findFirstFreeInode()
{
	for (int i = 0; i < 32; i++)
	{
		if (inodeTable[i].getLinkCounter() <= 0)
			return i;
	}
	return -1;
}

bool fcreate(std::string folder_name, std::string folder_to_create_name)
{
	allocateFreeMemory();
	if (folder_name.size() < 1 || folder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu." << white << std::endl;
		return false;
	}
	if (folder_to_create_name.size() < 1 || folder_to_create_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu nadrzednego." << white << std::endl;
		return false;
	}
	short int sfolder = fsearch(folder_name, 0);
	if (sfolder < 0)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie znaleziono folderu." << white << std::endl;
		return false;
	}
	short int freeInode = findFirstFreeInode();
	if (freeInode == -1)
	{
		std::cout << blue << "[SZPIK]" << red << " Brak wolnych iwezlow dla folderu." << white << std::endl;
		return false;
	}
	else
	{
		char* buff = (char*)folder_to_create_name.c_str();
		FEntry entry = FEntry((char)freeInode, buff);
		if (addFEntry(sfolder, entry))
		{
			FEntry entry2 = FEntry((char)freeInode, ".");
			if (addFEntry(freeInode, entry2))
			{
				entry2 = FEntry((char)sfolder, "..");
				if (addFEntry(freeInode, entry2))
				{
					std::vector<char>data = { 32 };
					std::vector<unsigned short>blocks = { (unsigned short)inodeTable[freeInode].getdb1() };
					dWrite(data, blocks, 16);
					dWrite(data, blocks, 24);
					inodeTable[freeInode].setType(true);
					inodeTable[freeInode].incrementLinkCounter();
					allocateFreeMemory();
					return true;
				}
			}
		}
		else
		{
			std::cout << blue << "[SZPIK]" << red << " Blad tworzenia wpisu." << white << std::endl;
			return false;
		}
	}
}

bool fcreate(std::string folder_name, std::string folder_to_create_name, std::string sfolder_name)
{
	allocateFreeMemory();
	if (folder_name.size() < 1 || folder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu." << white << std::endl;
		return false;
	}
	if (folder_to_create_name.size() < 1 || folder_to_create_name.size() > 7 || sfolder_name.size() < 1 || sfolder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu nadrzednego." << white << std::endl;
		return false;
	}
	short int sfolder = fsearch(folder_name, 0, sfolder_name);
	if (sfolder < 0)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie znaleziono folderu." << white << std::endl;
		return false;
	}
	short int freeInode = findFirstFreeInode();
	if (freeInode == -1)
	{
		std::cout << blue << "[SZPIK]" << red << " Brak wolnych iwezlow dla folderu." << white << std::endl;
		return false;
	}
	else
	{
		char* buff = (char*)folder_to_create_name.c_str();
		FEntry entry = FEntry((char)freeInode, buff);
		if (addFEntry(sfolder, entry))
		{
			FEntry entry2 = FEntry((char)freeInode, ".");
			if (addFEntry(freeInode, entry2))
			{
				entry2 = FEntry((char)sfolder, "..");
				if (addFEntry(freeInode, entry2))
				{
					std::vector<char>data = { 32 };
					std::vector<unsigned short>blocks = { (unsigned short)inodeTable[freeInode].getdb1() };
					dWrite(data, blocks, 16);
					dWrite(data, blocks, 24);
					inodeTable[freeInode].setType(true);
					inodeTable[freeInode].incrementLinkCounter();
					allocateFreeMemory();
					return true;
				}
			}
		}
		else
		{
			std::cout << blue << "[SZPIK]" << red << " Blad tworzenia wpisu." << white << std::endl;
			return false;
		}
	}
}

void drawFolderContents(std::string folderName)
{
	short int inodeNumber = fsearch(folderName, 0);
	if (inodeTable[inodeNumber].getType() == false)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na pliki!" << white << std::endl;
		return;
	}
	std::vector<FEntry> entries = extractFEntries(fsearch(folderName, 0));
	if (entries.size() == 0)
		return;
	else
	{
		std::setw(7);
		std::cout << folderName;
		std::cout << " \t" << "FOLDER " << "\t";
		std::setw(3);
		std::cout << inodeTable[fsearch(folderName, 0)].getSize() << "B";
		std::cout << std::endl;
		drawLevel++;
		for (int i = 0; i < entries.size(); i++)
		{
			for (int j = 0; j < drawLevel; j++)
				std::cout << "\t";
			if (i != entries.size() - 1)
				std::cout << (char)204;
			else
				std::cout << (char)200;
			if (inodeTable[entries[i].getNumber()].getType() == true)
			{
				if (entries[i].getName()[0] == '.')
				{
					std::setw(7);
					std::cout << entries[i].getName();
					std::cout << "\t" << "FOLDER ";
					std::setw(3);
					if (entries[i].getName()[1] == '.')
						std::cout << inodeTable[entries[i].getNumber()].getSize() << "B";
					else
						std::cout << inodeTable[entries[i].getNumber()].getSize() << "B";
					std::cout << std::endl;
				}
				else
				{
					std::setw(7);
					std::cout << entries[i].getName();
					drawFolderContents(entries[i].getNumber());
				}
			}
			else
			{
				std::setw(7);
				std::cout << entries[i].getName();
				std::cout << "\t" << "FILE \t";
				std::setw(3);
				std::cout << inodeTable[entries[i].getNumber()].getSize() << "B";
				std::cout << std::endl;
			}
		}
		drawLevel--;
	}
}

void drawFolderContents(unsigned short int iNodeNumber)
{
	if (inodeTable[iNodeNumber].getType() == false)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na pliki!" << white << std::endl;
		return;
	}
	std::vector<FEntry> entries = extractFEntries(iNodeNumber);
	if (entries.size() == 0)
		return;
	else
	{
		std::cout << " \t" << "FOLDER " << "\t";
		std::setw(3);
		std::cout << inodeTable[iNodeNumber].getSize() << "B";
		std::cout << std::endl;
		drawLevel++;
		for (int i = 0; i < entries.size(); i++)
		{
			for (int j = 0; j < drawLevel; j++)
				std::cout << "\t";
			if (i != entries.size() - 1)
				std::cout << (char)204;
			else
				std::cout << (char)200;
			if (inodeTable[entries[i].getNumber()].getType() == true)
			{
				if (entries[i].getName()[0] == '.')
				{
					std::setw(7);
					std::cout << entries[i].getName();
					std::cout << "\t" << "FOLDER \t";
					std::setw(3);
					if (entries[i].getName()[1] == '.')
						std::cout << inodeTable[entries[i].getNumber()].getSize() << "B";
					else
						std::cout << inodeTable[entries[i].getNumber()].getSize() << "B";
					std::cout << std::endl;
				}
				else
				{
					std::setw(7);
					std::cout << entries[i].getName();
					drawFolderContents(entries[i].getNumber());
				}
			}
			else
			{
				std::setw(7);
				std::cout << entries[i].getName();
				std::cout << "\t" << "FILE \t";
				std::setw(3);
				std::cout << inodeTable[entries[i].getNumber()].getSize() << "B";
				std::cout << std::endl;
			}
		}
		drawLevel--;
	}
}

bool releaseINode(unsigned short int iNodeNumber)
{
	inodeTable[iNodeNumber] = INode();
	if (inodeTable[iNodeNumber].getdb1() == -1 && inodeTable[iNodeNumber].getdb2() == -1 && inodeTable[iNodeNumber].getib() == -1 && inodeTable[iNodeNumber].getSize() == 0)
	{
		allocateFreeMemory();
		return true;
	}
	else
		return false;
}

bool deleteFEntry(short int folderInodeNumber, std::string name)
{
	std::vector<FEntry> entries = extractFEntries(folderInodeNumber);
	int which;
	for (int i = 1; i < entries.size(); i++)
		if (entries[i].getName() == name)
		{
			which = i;
			break;
		}
	if (which < 1)
		return false;
	inodeTable[folderInodeNumber].setSize(8 * which);
	if (which < 4)
	{
		inodeTable[folderInodeNumber].setdb2(-1);
		inodeTable[folderInodeNumber].setib(-1);
		allocateFreeMemory();
		for (int i = which + 1; i < entries.size(); i++)
		{
			if (i <= entries.size())
				addFEntry(folderInodeNumber, entries[i]);
			else
				break;
		}
		addFEntry(folderInodeNumber, FEntry(0, ""));
		inodeTable[folderInodeNumber].setSize(inodeTable[folderInodeNumber].getSize() - 8);
		return true;
	}
	else
	{
		if (which < 8)
		{
			inodeTable[folderInodeNumber].setib(-1);
			allocateFreeMemory();
			for (int i = which + 1; i < entries.size(); i++)
			{
				if (i <= entries.size())
					addFEntry(folderInodeNumber, entries[i]);
				else
					break;
			}
			addFEntry(folderInodeNumber, FEntry(0, ""));
			inodeTable[folderInodeNumber].setSize(inodeTable[folderInodeNumber].getSize() - 8);
			return true;
		}
		else
		{
			std::vector<short> IBentries = readIB(inodeTable[folderInodeNumber].getib());
			which -= 8;
			int whichDB = which / 4;
			if (whichDB > IBentries.size())
				return false;
			else
			{
				for (int i = whichDB + 1; i < 32; i++)
				{
					std::vector<unsigned short> blocks = { (unsigned short)inodeTable[folderInodeNumber].getib() };
					std::vector<char> data = { (char)32 };
					dWrite(data, blocks, i);
				}
				which = which % 4;
				for (int i = which; i < 4; i++)
				{
					std::vector<unsigned short> blocks = { (unsigned short)IBentries[whichDB] };
					std::vector<char> data = { (char)32, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0 };
					dWrite(data, blocks, i * 8);
				}
				which += 8;
				for (int i = which - 1; i < entries.size(); i++)
					addFEntry(folderInodeNumber, entries[i]);
				std::vector<short> IBentries2 = readIB(inodeTable[folderInodeNumber].getib());
				if (IBentries2.size() + 1 == IBentries.size())
					return true;
				else
					return false;
			}
		}
	}
}

bool fdelete(std::string folder_name)
{
	if (folder_name[0] == '.' || folder_name == "ROOT")
	{
		std::cout << blue << "[SZPIK]" << red << " Nie mozna tego usunac!" << white << std::endl;
		return false;
	}
	if (folder_name.size() < 1 || folder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy!" << white << std::endl;
		return false;
	}
	short int inode = fsearch(folder_name, 0);
	if (inode < 0 || inode > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono " << folder_name << " na dysku." << white << std::endl;
		return false;
	}
	if (inodeTable[inode].getType() == false)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na pliki!" << white << std::endl;
		return false;
	}
	else
	{
		std::vector<FEntry> entries = extractFEntries(inode);
		int buff = entries[1].getNumber();
		for (int i = 2; i < entries.size(); i++)
			if (inodeTable[entries[i].getNumber()].getType() == true)
				fdelete(entries[i].getName());
		//else
			//unLink(entries[i].getName());
		if (deleteFEntry(buff, folder_name))
		{
			if (releaseINode(inode))
				return true;
			else
			{
				std::cout << blue << "[SZPIK]" << red << " Blad czyszczenia iwezla. Pliki podrzedne zostaly usuniete." << white << std::endl;
				return false;
			}
		}
		else
		{
			std::cout << blue << "[SZPIK]" << red << " Blad usuwania wpisu w folderze nadrzednym. Pliki podrzedne zostaly usuniete." << white << std::endl;
			return false;
		}

	}
}

bool fdelete(std::string folder_name, std::string sfolder_name)
{
	if (folder_name[0] == '.' || folder_name == "ROOT")
	{
		std::cout << blue << "[SZPIK]" << red << " Nie mozna tego usunac!" << white << std::endl;
		return false;
	}
	if (folder_name.size() < 1 || folder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy!" << white << std::endl;
		return false;
	}
	short int inode = fsearch(folder_name, 0, sfolder_name);
	if (inode < 0 || inode > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono " << folder_name << " na dysku." << white << std::endl;
		return false;
	}
	if (inodeTable[inode].getType() == false)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na pliki!" << white << std::endl;
		return false;
	}
	else
	{
		std::vector<FEntry> entries = extractFEntries(inode);
		int buff = entries[1].getNumber();
		for (int i = 2; i < entries.size(); i++)
		{
			if (inodeTable[entries[i].getNumber()].getType() == true)
				fdelete(entries[i].getName());
			else
				unLink(entries[i].getName());
		}
		if (deleteFEntry(buff, folder_name))
		{
			if (releaseINode(inode))
				return true;
			else
			{
				std::cout << blue << "[SZPIK]" << red << " Blad czyszczenia iwezla. Pliki podrzedne zostaly usuniete." << white << std::endl;
				return false;
			}
		}
		else
		{
			std::cout << blue << "[SZPIK]" << red << " Blad usuwania wpisu w folderze nadrzednym. Pliki podrzedne zostaly usuniete." << white << std::endl;
			return false;
		}

	}
}

bool fecreate(std::string folder_name, std::string file_name, unsigned short int inode_number)
{
	if (folder_name.size() < 1 || folder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu." << white << std::endl;
		return false;
	}
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return false;
	}
	short int folder = fsearch(folder_name, 0);
	if (folder < 0)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono folderu." << white << std::endl;
		return false;
	}
	if (inodeTable[folder].getType() == false)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na pliki!" << white << std::endl;
		return false;
	}
	if (inodeTable[inode_number].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie mozna tworzyc dodatkowych dowiazan folderom!" << white << std::endl;
		return false;
	}
	else
	{
		FEntry entry = FEntry(inode_number, file_name);
		if (addFEntry(folder, entry))
			return true;
		else
		{
			std::cout << blue << "[SZPIK]" << red << " Blad dodawania wpisu." << white << std::endl;
			return false;
		}
	}
}

bool fecreate(std::string folder_name, std::string file_name, unsigned short int inode_number, std::string sfolder_name)
{
	if (folder_name.size() < 1 || folder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu." << white << std::endl;
		return false;
	}
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return false;
	}
	if (sfolder_name.size() < 1 || sfolder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu nadrzednego." << white << std::endl;
		return false;
	}
	short int folder = fsearch(folder_name, 0, sfolder_name);
	if (folder < 0)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono folderu." << white << std::endl;
		return false;
	}
	if (inodeTable[folder].getType() == false)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na pliki!" << white << std::endl;
		return false;
	}
	if (inodeTable[inode_number].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie mozna tworzyc dodatkowych dowiazan folderom!" << white << std::endl;
		return false;
	}
	else
	{
		FEntry entry = FEntry(inode_number, file_name);
		if (addFEntry(folder, entry))
			return true;
		else
		{
			std::cout << blue << "[SZPIK]" << red << " Blad dodawania wpisu." << white << std::endl;
			return false;
		}
	}
}

short int whoHasThat(std::string name)
{
	std::vector<FEntry> entries;
	for (int i = 0; i < 32; i++)
		if (inodeTable[i].getType() == true && inodeTable[i].getLinkCounter() != 0)
		{
			entries = extractFEntries(i);
			for (int j = 0; j < entries.size(); j++)
				if (entries[j].getName() == name)
					return i;
		}
	return -1;
}

std::vector<short int> whoHasThatE(std::string name)
{
	std::vector<FEntry> entries;
	std::vector<short int> found;
	for (int i = 0; i < 32; i++)
		if (inodeTable[i].getType() == true && inodeTable[i].getLinkCounter() != 0)
		{
			entries = extractFEntries(i);
			for (int j = 0; j < entries.size(); j++)
				if (entries[j].getName() == name)
					found.push_back((short int)entries[j].getNumber());
		}
	return found;
}

bool fedelete(std::string file_name)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return false;
	}
	else
	{
		short int inode = fsearch(file_name, 0);
		if (inode < 0 || inode > 31)
		{
			std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono " << file_name << " na dysku." << white << std::endl;
			return false;
		}
		else
		{
			if (inodeTable[inode].getType() == true)
			{
				if (fdelete(file_name))
					return false;
				else
				{
					std::cout << blue << "[SZPIK]" << red << " Blad usuwania wpisu." << white << std::endl;
					return false;
				}
			}
			else
			{
				short int folder = whoHasThat(file_name);
				if (folder < 0 || folder > 31)
				{
					std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono folderu zawierajacego wpis o tej nazwie." << white << std::endl;
					return false;
				}
				else
				{
					if (deleteFEntry(folder, file_name))
					{
						inodeTable[inode].decrementLinkCounter();
						if (inodeTable[inode].getLinkCounter() == 0)
							releaseINode(inode);
						return true;
					}
					else
					{
						std::cout << blue << "[SZPIK]" << red << " Blad usuwania wpisu." << white << std::endl;
						return false;
					}
				}
			}
		}
	}
}

bool fedelete(std::string file_name, std::string sfolder_name)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return false;
	}
	if (sfolder_name.size() < 1 || sfolder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu nadrzednego." << white << std::endl;
		return false;
	}
	else
	{
		short int inode = fsearch(file_name, 0, sfolder_name);
		if (inode < 0 || inode > 31)
		{
			std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono " << file_name << " na dysku." << white << std::endl;
			return false;
		}
		else
		{
			if (inodeTable[inode].getType() == true)
			{
				if (fdelete(file_name))
					return false;
				else
				{
					std::cout << blue << "[SZPIK]" << red << " Blad usuwania wpisu." << white << std::endl;
					return false;
				}
			}
			else
			{
				short int folder = fsearch(sfolder_name, 0);
				if (folder < 0 || folder > 31)
				{
					std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono folderu zawierajacego wpis o tej nazwie." << white << std::endl;
					return false;
				}
				else
				{
					if (deleteFEntry(folder, file_name))
					{
						inodeTable[inode].decrementLinkCounter();
						if (inodeTable[inode].getLinkCounter() == 0)
							releaseINode(inode);
						return true;
					}
					else
					{
						std::cout << blue << "[SZPIK]" << red << " Blad usuwania wpisu." << white << std::endl;
						return false;
					}
				}
			}
		}
	}
}

std::vector<FEntry> fcontent(std::string folder_name)
{
	short int inode = fsearch(folder_name, 0);
	if (inodeTable[inode].getType() == false)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na pliki!" << white << std::endl;
		return { FEntry((char)32, "ERROR") };
	}
	if (fsearch(folder_name, 0) == -1)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono folderu o nazwie " << folder_name << "."  << white << std::endl;
		return { FEntry((char)32, "ERROR") };
	}
	std::vector<FEntry> entries = extractFEntries(fsearch(folder_name, 0));
	if (entries.size() < 1)
	{
		std::cout << blue << "[SZPIK]" << red << " Folder o nazwie " << folder_name << " nie posiada zadnych wpisow." << white << std::endl;
		return { FEntry((char)32, "ERROR") };
	}
	else
		return entries;
}

std::vector<FEntry> fcontent(std::string folder_name, std::string sfolder_name)
{
	short int inode = fsearch(folder_name, 0);
	if (inodeTable[inode].getType() == false)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na pliki!" << white << std::endl;
		return { FEntry((char)32, "ERROR") };
	}
	if (fsearch(folder_name, 0, sfolder_name) == -1)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono folderu o nazwie " << folder_name << "."  << white << std::endl;
		return { FEntry((char)32, "ERROR") };
	}
	std::vector<FEntry> entries = extractFEntries(fsearch(folder_name, 0, sfolder_name));
	if (entries.size() < 1)
	{
		std::cout << blue << "[SZPIK]" << red << " Folder o nazwie " << folder_name << " nie posiada zadnych wpisow." << white << std::endl;
		return { FEntry((char)32, "ERROR") };
	}
	else
		return entries;
}

bool create(std::string folder_name, std::string file_name)
{
	allocateFreeMemory();
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return false;
	}
	if (folder_name.size() < 1 || folder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu." << white << std::endl;
		return false;
	}
	short int sfolder = fsearch(folder_name, 0);
	if (sfolder < 0)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie znaleziono folderu." << white << std::endl;
		return false;
	}
	short int freeInode = findFirstFreeInode();
	if (freeInode == -1)
	{
		std::cout << blue << "[SZPIK]" << red << " Brak wolnych iwezlow dla pliku." << white << std::endl;
		return false;
	}
	else
	{
		char* buff = (char*)file_name.c_str();
		FEntry entry = FEntry((char)freeInode, buff);
		if (addFEntry(sfolder, entry))
		{
			inodeTable[freeInode].incrementLinkCounter();
			allocateFreeMemory();
			return true;
		}
		else
		{
			std::cout << blue << "[SZPIK]" << red << " Blad tworzenia wpisu." << white << std::endl;
			return false;
		}
	}
}

bool create(std::string folder_name, std::string file_name, std::string sfolder_name)
{
	allocateFreeMemory();
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return false;
	}
	if (folder_name.size() < 1 || folder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu." << white << std::endl;
		return false;
	}
	if (sfolder_name.size() < 1 || sfolder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu nadrzednego."  << white << std::endl;
		return false;
	}
	short int sfolder = fsearch(folder_name, 0, sfolder_name);
	if (sfolder < 0)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie znaleziono folderu." << white << std::endl;
		return false;
	}
	if (inodeTable[sfolder].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return false;
	}
	short int freeInode = findFirstFreeInode();
	if (freeInode == -1)
	{
		std::cout << blue << "[SZPIK]" << red << " Brak wolnych iwezlow dla pliku." << white << std::endl;
		return false;
	}
	else
	{
		char* buff = (char*)file_name.c_str();
		FEntry entry = FEntry((char)freeInode, buff);
		if (addFEntry(sfolder, entry))
		{
			inodeTable[freeInode].incrementLinkCounter();
			allocateFreeMemory();
			return true;
		}
		else
		{
			std::cout << blue << "[SZPIK]" << red << " Blad tworzenia wpisu." << white << std::endl;
			return false;
		}
	}
}

bool unLink(std::string file_name)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return false;
	}
	else
	{
		short int inode = fsearch(file_name, 0);
		if (inode < 0 || inode > 31)
		{
			std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono " << file_name << " na dysku." << white <<  std::endl;
			return false;
		}
		else
		{
			if (inodeTable[inode].getType() == true)
			{
				std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie moze dzialac na folderach!" << white << std::endl;
				return false;
			}
			else
			{
				short int folder = whoHasThat(file_name);
				if (folder < 0 || folder > 31)
				{
					std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono folderu zawierajacego wpis o tej nazwie." << white << std::endl;
					return false;
				}
				else
				{
					if (deleteFEntry(folder, file_name))
					{
						inodeTable[inode].decrementLinkCounter();
						if (inodeTable[inode].getLinkCounter() == 0)
							releaseINode(inode);
						return true;
					}
					else
					{
						std::cout << blue << "[SZPIK]" << red << " Blad usuwania wpisu." << white <<  std::endl;
						return false;
					}
				}
			}
		}
	}
}

bool unLink(std::string file_name, std::string &sfolder_name)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return false;
	}
	if (sfolder_name.size() < 1 || sfolder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu nadrzednego."  << white << std::endl;
		return false;
	}
	else
	{
		short int inode = fsearch(file_name, 0, sfolder_name);
		if (inode < 0 || inode > 31)
		{
			std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono " << file_name << " na dysku." << white <<  std::endl;
			return false;
		}
		else
		{
			if (inodeTable[inode].getType() == true)
			{
				std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie moze dzialac na folderach!" << white << std::endl;
				return false;
			}
			else
			{
				short int folder = fsearch(sfolder_name, 0);
				if (folder < 0 || folder > 31)
				{
					std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono folderu zawierajacego wpis o tej nazwie." << white << std::endl;
					return false;
				}
				else
				{
					if (deleteFEntry(folder, file_name))
					{
						inodeTable[inode].decrementLinkCounter();
						if (inodeTable[inode].getLinkCounter() == 0)
							releaseINode(inode);
						return true;
					}
					else
					{
						std::cout << blue << "[SZPIK]" << red << " Blad usuwania wpisu." << white <<  std::endl;
						return false;
					}
				}
			}
		}
	}
}

bool frename(std::string file_name, std::string new_file_name)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc starej nazwy pliku." << white << std::endl;
		return false;
	}
	if (new_file_name.size() < 1 || new_file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nowej nazwy pliku." << white << std::endl;
		return false;
	}
	int folderInodeNumber = whoHasThat(file_name);
	if (folderInodeNumber < 0)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono folderu zawierajacego wpis o tej nazwie." << white << std::endl;
		return false;
	}
	if (inodeTable[folderInodeNumber].getType() == false)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na pliki!" << white << std::endl;
		return false;
	}
	else
	{
		std::vector<FEntry> entries = extractFEntries(folderInodeNumber);
		int which;
		for (int i = 1; i < entries.size(); i++)
			if (entries[i].getName() == file_name)
			{
				which = i;
				break;
			}
		if (which < 1)
			return false;
		if (which < 4)
		{
			std::vector<unsigned short> blocks = { (unsigned short)inodeTable[folderInodeNumber].getdb1() };
			std::vector<char> data;
			for (int i = 0; i < new_file_name.size(); i++)
				data.push_back(new_file_name[i]);
			dWrite({ (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0 }, blocks, (which * 8) + 1);
			dWrite(data, blocks, (which * 8) + 1);
			return true;
		}
		else
		{
			if (which < 8)
			{
				std::vector<unsigned short> blocks = { (unsigned short)inodeTable[folderInodeNumber].getdb2() };
				std::vector<char> data;
				for (int i = 0; i < new_file_name.size(); i++)
					data.push_back(new_file_name[i]);
				dWrite({ (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0 }, blocks, (which * 8) + 1);
				dWrite(data, blocks, (which * 8) + 1);
				return true;
			}
			else
			{
				std::vector<short> IBentries = readIB(inodeTable[folderInodeNumber].getib());
				which -= 8;
				int whichDB = which / 4;
				if (whichDB > IBentries.size())
					return false;
				else
				{
					which = which % 4;
					std::vector<unsigned short> blocks = { (unsigned short)IBentries[whichDB] };
					std::vector<char> data;
					for (int i = 0; i < new_file_name.size(); i++)
						data.push_back(new_file_name[i]);
					dWrite({ (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0 }, blocks, (which * 8) + 1);
					dWrite(data, blocks, (which * 8) + 1);
					return true;
				}
			}
		}
	}
}

bool frename(std::string file_name, std::string new_file_name, std::string sfolder_name)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc starej nazwy pliku." << white << std::endl;
		return false;
	}
	if (new_file_name.size() < 1 || new_file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nowej nazwy pliku." << white << std::endl;
		return false;
	}
	if (sfolder_name.size() < 1 || sfolder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc folderu nadrzednego." << white << std::endl;
		return false;
	}
	std::vector<short int> folderInodeNumberT = whoHasThatE(file_name);
	short int folderInodeNumber = -1;
	if (folderInodeNumberT.size() < 0)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono folderu zawierajacego wpis o tej nazwie." << white << std::endl;
		return false;
	}
	else
	{
		for (int i = 0; i < folderInodeNumberT.size(); i++)
		{
			if (fsearch(file_name, folderInodeNumberT[i]) == fsearch(file_name, fsearch(sfolder_name, 0)))
			{
				folderInodeNumber = folderInodeNumberT[i];
				break;
			}
		}
		if (folderInodeNumber == -1)
		{
			std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono folderu nadrzednego zawierajacego folder posiadajacy wpis o tej nazwie." << white << std::endl;
			return false;
		}
		if (inodeTable[folderInodeNumber].getType() == false)
		{
			std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na pliki!" << white << std::endl;
			return false;
		}
		std::vector<FEntry> entries = extractFEntries(folderInodeNumber);
		int which;
		for (int i = 1; i < entries.size(); i++)
			if (entries[i].getName() == file_name)
			{
				which = i;
				break;
			}
		if (which < 1)
			return false;
		if (which < 4)
		{
			std::vector<unsigned short> blocks = { (unsigned short)inodeTable[folderInodeNumber].getdb1() };
			std::vector<char> data;
			for (int i = 0; i < new_file_name.size(); i++)
				data.push_back(new_file_name[i]);
			dWrite({ (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0 }, blocks, (which * 8) + 1);
			dWrite(data, blocks, (which * 8) + 1);
			return true;
		}
		else
		{
			if (which < 8)
			{
				std::vector<unsigned short> blocks = { (unsigned short)inodeTable[folderInodeNumber].getdb2() };
				std::vector<char> data;
				for (int i = 0; i < new_file_name.size(); i++)
					data.push_back(new_file_name[i]);
				dWrite({ (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0 }, blocks, (which * 8) + 1);
				dWrite(data, blocks, (which * 8) + 1);
				return true;
			}
			else
			{
				std::vector<short> IBentries = readIB(inodeTable[folderInodeNumber].getib());
				which -= 8;
				int whichDB = which / 4;
				if (whichDB > IBentries.size())
					return false;
				else
				{
					which = which % 4;
					std::vector<unsigned short> blocks = { (unsigned short)IBentries[whichDB] };
					std::vector<char> data;
					for (int i = 0; i < new_file_name.size(); i++)
						data.push_back(new_file_name[i]);
					dWrite({ (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0 }, blocks, (which * 8) + 1);
					dWrite(data, blocks, (which * 8) + 1);
					return true;
				}
			}
		}
	}
}

bool open(std::string file_name, bool open_type)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return false;
	}
	short int inodeNumber = fsearch(file_name, 0);
	if (inodeNumber < 0 || inodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return false;
	}
	if (inodeTable[inodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return false;
	}
	else
	{
		processNumber = Procesor::getRunning()->PID;
		bool is = false;
		for (int i = 0; i < processOFT.size(); i++)
		{
			if (processOFT[i].processID == processNumber)
			{
				for (int j = 0; j < processOFT[i].files.size(); j++)
					if (processOFT[i].files[j].first == inodeNumber)
					{
						std::cout << blue << "[SZPIK]" << red << " Ten proces juz otworzyl ten plik! Jezeli chcesz zmienic typ otwarcia, zamknij ten plik i ponownie go otworz." << white <<  std::endl;
						return false;
					}
			}
		}
		int which = -1;
		for (int i = 0; i < openFiles.size(); i++)
		{
			if (openFiles[i].first == inodeNumber)
			{
				is = true;
				int which = i;
				break;
			}
		}
		if (is)
		{
			if (open_type == true)
			{
				for (int i = 0; i < processOFT.size(); i++)
				{
					for (int j = 0; j < processOFT[i].files.size(); j++)
					{
						if (processOFT[i].files[j].second == true)
						{
							std::cout << blue << "[SZPIK]" << red << " Proces o numerze " << processOFT[i].processID << " otworzyl juz ten plik do zapisu!" << white <<  std::endl;
							return false;
						}
					}
				}
				for (int i = 0; i < processOFT.size(); i++)
				{
					if (processOFT[i].processID == processNumber)
					{
						openFiles[which].second++;
						processOFT[i].files.push_back(std::make_pair(inodeNumber, open_type));
						processOFT[i].positions.push_back(0);
						return true;
					}
				}
				std::cout << blue << "[SZPIK]" << red << " Nastapil nieoczekiwany blad, nie udalo sie otworzyc pliku." << white <<  std::endl;
				return false;
			}
			else
			{
				for (int i = 0; i < processOFT.size(); i++)
				{
					if (processOFT[i].processID == processNumber)
					{
						openFiles[which].second++;
						processOFT[i].files.push_back(std::make_pair(inodeNumber, open_type));
						processOFT[i].positions.push_back(0);
						return true;
					}
				}
				std::cout << blue << "[SZPIK]" << red << " Nastapil nieoczekiwany blad, nie udalo sie otworzyc pliku." << white <<  std::endl;
				return false;
			}
		}
		else
		{
			is = false;
			for (int i = 0; i < processOFT.size(); i++)
			{
				if (processOFT[i].processID == processNumber)
				{
					is = true;
					which = i;
					break;
				}
			}
			if (is)
			{
				openFiles.push_back(std::make_pair(inodeNumber, 1));
				processOFT[which].files.push_back(std::make_pair(inodeNumber, open_type));
				processOFT[which].positions.push_back(0);
				return true;
			}
			else
			{
				openFiles.push_back(std::make_pair(inodeNumber, 1));
				processOFT.push_back(fileOpener(processNumber, { std::make_pair(inodeNumber, true) }));
				return true;
			}
		}
	}
}

bool open(std::string file_name, bool open_type, std::string sfolder_name)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return false;
	}
	if (sfolder_name.size() < 1 || sfolder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc folderu nadrzednego." << white << std::endl;
		return false;
	}
	short int inodeNumber = fsearch(file_name, 0, sfolder_name);
	if (inodeNumber < 0 || inodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return false;
	}
	if (inodeTable[inodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return false;
	}
	else
	{
		processNumber = Procesor::getRunning()->PID;
		bool is = false;
		for (int i = 0; i < processOFT.size(); i++)
		{
			if (processOFT[i].processID == processNumber)
			{
				for (int j = 0; j < processOFT[i].files.size(); j++)
					if (processOFT[i].files[j].first == inodeNumber)
					{
						std::cout << blue << "[SZPIK]" << red << " Ten proces juz otworzyl ten plik! Jezeli chcesz zmienic typ otwarcia, zamknij ten plik i ponownie go otworz." << white <<  std::endl;
						return false;
					}
			}
		}
		int which = -1;
		for (int i = 0; i < openFiles.size(); i++)
		{
			if (openFiles[i].first == inodeNumber)
			{
				is = true;
				int which = i;
				break;
			}
		}
		if (is)
		{
			if (open_type == true)
			{
				for (int i = 0; i < processOFT.size(); i++)
				{
					for (int j = 0; j < processOFT[i].files.size(); j++)
					{
						if (processOFT[i].files[j].second == true)
						{
							std::cout << blue << "[SZPIK]" << red << " Proces o numerze " << processOFT[i].processID << " otworzyl juz ten plik do zapisu!" << white <<  std::endl;
							return false;
						}
					}
				}
				for (int i = 0; i < processOFT.size(); i++)
				{
					if (processOFT[i].processID == processNumber)
					{
						openFiles[which].second++;
						processOFT[i].files.push_back(std::make_pair(inodeNumber, open_type));
						return true;
					}
				}
				std::cout << blue << "[SZPIK]" << red << " Nastapil nieoczekiwany blad, nie udalo sie otworzyc pliku." << white <<  std::endl;
				return false;
			}
			else
			{
				for (int i = 0; i < processOFT.size(); i++)
				{
					if (processOFT[i].processID == processNumber)
					{
						openFiles[which].second++;
						processOFT[i].files.push_back(std::make_pair(inodeNumber, open_type));
						return true;
					}
				}
				std::cout << blue << "[SZPIK]" << red << " Nastapil nieoczekiwany blad, nie udalo sie otworzyc pliku." << white <<  std::endl;
				return false;
			}
		}
		else
		{
			is = false;
			for (int i = 0; i < processOFT.size(); i++)
			{
				if (processOFT[i].processID == processNumber)
				{
					is = true;
					which = i;
					break;
				}
			}
			if (is)
			{
				openFiles.push_back(std::make_pair(inodeNumber, 1));
				processOFT[which].files.push_back(std::make_pair(inodeNumber, open_type));
				return true;
			}
			else
			{
				openFiles.push_back(std::make_pair(inodeNumber, 1));
				processOFT.push_back(fileOpener(processNumber, { std::make_pair(inodeNumber, true) }));
				return true;
			}
		}
	}
}

bool close(std::string file_name)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return false;
	}
	short int iNodeNumber = fsearch(file_name, 0);
	if (iNodeNumber < 0 || iNodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return false;
	}
	if (inodeTable[iNodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return false;
	}
	else
	{
		processNumber = Procesor::getRunning()->PID;
		bool is = false;
		int which = -1;
		int which2 = -1;
		int which3 = -1;
		for (int i = 0; i < openFiles.size(); i++) //szukanie pliku w tablicy otwartych plików
		{
			if (openFiles[i].first == iNodeNumber)
			{
				is = true;
				which2 = i;
				break;
			}
		}
		if (is)
		{
			is = false;
			for (int i = 0; i < processOFT.size(); i++) // szukanie procesu w procesOFT
			{
				if (processOFT[i].processID == processNumber)
				{
					is = true;
					which = i;
					break;
				}
			}
			if (is)
			{
				is = false;
				for (int i = 0; i < processOFT[which].files.size(); i++)
				{
					if (processOFT[which].files[i].first == iNodeNumber)
					{
						is = true;
						which3 = i;
						break;
					}
				}
				if (is)
				{
					processOFT[which].files.erase(processOFT[which].files.begin() + which3);
					processOFT[which].positions.erase(processOFT[which].positions.begin() + which3);
					openFiles[which2].second--;
					if (openFiles[which2].second < 1)
					{
						openFiles.erase(openFiles.begin() + which2);
					}
					return true;
				}
				else
				{
					std::cout << blue << "[SZPIK]" << red << " Blad Uprawnien: Ten proces nie otworzyl tego pliku." << white <<  std::endl;
					return false;
				}
			}
			else
			{
				std::cout << blue << "[SZPIK]" << red << " Blad Uprawnien: Ten proces nie otworzyl zadnego pliku." << white <<  std::endl;
				return false;
			}
		}
		else
		{
			std::cout << blue << "[SZPIK]" << red << " Ten plik nie jest otwarty!" << white << std::endl;
			return false;
		}
	}
}

bool close(std::string file_name, std::string sfolder_name)
{
	processNumber = Procesor::getRunning()->PID;
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return false;
	}
	if (sfolder_name.size() < 1 || sfolder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu nadrzednego."  << white << std::endl;
		return false;
	}
	short int iNodeNumber = fsearch(file_name, 0, sfolder_name);
	if (iNodeNumber < 0 || iNodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return false;
	}
	if (inodeTable[iNodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return false;
	}
	else
	{
		bool is = false;
		int which = -1;
		int which2 = -1;
		int which3 = -1;
		for (int i = 0; i < openFiles.size(); i++)
		{
			if (openFiles[i].first == iNodeNumber)
			{
				is = true;
				which2 = i;
				break;
			}
		}
		if (is)
		{
			is = false;
			for (int i = 0; i < processOFT.size(); i++)
			{
				if (processOFT[i].processID == processNumber)
				{
					is = true;
					which = i;
					break;
				}
			}
			if (is)
			{
				is = false;
				for (int i = 0; i < processOFT[which].files.size(); i++)
				{
					if (processOFT[which].files[i].first == iNodeNumber)
					{
						is = true;
						which3 = i;
						break;
					}
				}
				if (is)
				{
					processOFT[which].files.erase(processOFT[which].files.begin() + which3);
					processOFT[which].positions.erase(processOFT[which].positions.begin() + which3);
					openFiles[which2].second--;
					if (openFiles[which2].second < 1)
					{
						openFiles.erase(openFiles.begin() + which2);
					}
					return true;
				}
				else
				{
					std::cout << blue << "[SZPIK]" << red << " Blad Uprawnien: Ten proces nie otworzyl tego pliku." << white <<  std::endl;
					return false;
				}
			}
			else
			{
				std::cout << blue << "[SZPIK]" << red << " Blad Uprawnien: Ten proces nie otworzyl zadnego pliku." << white <<  std::endl;
				return false;
			}
		}
		else
		{
			std::cout << blue << "[SZPIK]" << red << " Ten plik nie jest otwarty!" << white << std::endl;
			return false;
		}
	}
}

bool truncate(std::string file_name, unsigned short int final_bytes_amount)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return false;
	}
	short int inodeNumber = fsearch(file_name, 0);
	if (inodeNumber < 0 || inodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return false;
	}
	if (inodeTable[inodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return false;
	}
	if (final_bytes_amount < 0 || final_bytes_amount > inodeTable[inodeNumber].getSize())
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc docelowa." << white << std::endl;
		return false;
	}
	else
	{
		inodeTable[inodeNumber].setSize(final_bytes_amount);
		if (inodeTable[inodeNumber].getib() != -1)
		{
			std::vector<char> data;
			for (int i = 0; i < 32; i++)
				data.push_back((char)32);
			std::vector<short int> blocks = readIB(inodeTable[inodeNumber].getib());
			for (int i = 0; i < blocks.size(); i++)
			{
				if (final_bytes_amount <= (64 + ((i + 1) * 32)))
					data[i] = (char)32;
			}
			std::vector<unsigned short int> block = { (unsigned short)inodeTable[inodeNumber].getib() };
			dWrite(data, block, 0);
		}
		if (final_bytes_amount <= 64)
		{
			inodeTable[inodeNumber].setib(-1);
		}
		if (final_bytes_amount <= 32)
		{
			inodeTable[inodeNumber].setdb2(-1);
		}
		if (final_bytes_amount <= 0)
		{
			inodeTable[inodeNumber].setdb1(-1);
		}
		allocateFreeMemory();
		return true;
	}
}

bool truncate(std::string file_name, unsigned short int final_bytes_amount, std::string sfolder_name)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return false;
	}
	if (sfolder_name.size() < 1 || sfolder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu nadrzednego."  << white << std::endl;
		return false;
	}
	short int inodeNumber = fsearch(file_name, 0, sfolder_name);
	if (inodeNumber < 0 || inodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return false;
	}
	if (inodeTable[inodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return false;
	}
	if (final_bytes_amount < 0 || final_bytes_amount > inodeTable[inodeNumber].getSize())
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc docelowa." << white << std::endl;
		return false;
	}
	else
	{
		inodeTable[inodeNumber].setSize(final_bytes_amount);
		if (inodeTable[inodeNumber].getib() != -1)
		{
			std::vector<char> data;
			for (int i = 0; i < 32; i++)
				data.push_back((char)32);
			std::vector<short int> blocks = readIB(inodeTable[inodeNumber].getib());
			for (int i = 0; i < blocks.size(); i++)
			{
				if (final_bytes_amount <= (64 + ((i + 1) * 32)))
					data[i] = (char)32;
			}
			std::vector<unsigned short int> block = { (unsigned short)inodeTable[inodeNumber].getib() };
			dWrite(data, block, 0);
		}
		if (final_bytes_amount <= 64)
		{
			inodeTable[inodeNumber].setib(-1);
		}
		if (final_bytes_amount <= 32)
		{
			inodeTable[inodeNumber].setdb2(-1);
		}
		if (final_bytes_amount <= 0)
		{
			inodeTable[inodeNumber].setdb1(-1);
		}
		allocateFreeMemory();
		return true;
	}
}

bool isAllowedToModify(std::string name, bool open_type)
{
	short int iNode = fsearch(name, 0);
	if (iNode < 0 || iNode > 31)
		return false;
	else
	{
		processNumber = Procesor::getRunning()->PID;
		bool is = false;
		int which = -1;
		int which2 = -1;
		int which3 = -1;
		for (int i = 0; i < openFiles.size(); i++)
		{
			if (openFiles[i].first == iNode)
			{
				is = true;
				which2 = i;
				break;
			}
		}
		if (is)
		{
			is = false;
			for (int i = 0; i < processOFT.size(); i++)
			{
				if (processOFT[i].processID == processNumber)
				{
					is = true;
					which = i;
					break;
				}
			}
			if (is)
			{
				is = false;
				for (int i = 0; i < processOFT[which].files.size(); i++)
				{
					if (processOFT[which].files[i].first == iNode)
					{
						is = true;
						which3 = i;
						break;
					}
				}
				if (is)
				{
					if (open_type == true)
					{
						if (processOFT[which].files[which3].second)
							return true;
						else
						{
							return false;
						}
					}
					else
					{
						return true;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

short int read(std::string file_name, char* &data, unsigned short int bytes_amount)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return 0;
	}
	short int inodeNumber = fsearch(file_name, 0);
	if (inodeNumber < 0 || inodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return 0;
	}
	if (inodeTable[inodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return 0;
	}
	else
	{
		processNumber = Procesor::getRunning()->PID;
		unsigned short int toRead = bytes_amount;
		bool is = false;
		for (int i = 0; i < processOFT.size(); i++)
		{
			if (processOFT[i].processID == processNumber)
			{
				for (int j = 0; j < processOFT[i].files.size(); j++)
					if (processOFT[i].files[j].first == inodeNumber)
					{
						if (isAllowedToModify(file_name, false))
						{
							if (bytes_amount > inodeTable[inodeNumber].getSize() - processOFT[i].positions[j])
								toRead = inodeTable[inodeNumber].getSize() - processOFT[i].positions[j];
							std::vector<unsigned short int>blocks;
							if (toRead >= 0)
								blocks.push_back(inodeTable[inodeNumber].getdb1());
							if (toRead > 32)
								blocks.push_back(inodeTable[inodeNumber].getdb2());
							std::vector<short int> ibBlocks = readIB(inodeTable[inodeNumber].getib());
							for (int i = 0; i < ibBlocks.size(); i++)
							{
								if (toRead > (64 + (i * 32)))
									blocks.push_back(ibBlocks[i]);
							}
							unsigned short int read = dRead(data, blocks, processOFT[i].positions[j] % 32, toRead);							
							processOFT[i].increasePosition(read, j);
							return read;
						}
						else
						{
							std::cout << blue << "[SZPIK]" << red << " Blad Uprawnien: Proces nie ma uprawnien do dzialania na tym pliku." << white << std::endl;
							return 0;
						}
					}
				std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl tego pliku." << white << std::endl;
				return 0;
			}
		}
		std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl zadnego pliku." << white << std::endl;
		return 0;
	}
}

short int read(std::string file_name, char* &data, unsigned short int bytes_amount, std::string sfolder_name)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return 0;
	}
	if (sfolder_name.size() < 1 || sfolder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu nadrzednego."  << white << std::endl;
		return 0;
	}
	short int inodeNumber = fsearch(file_name, 0, sfolder_name);
	if (inodeNumber < 0 || inodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return 0;
	}
	if (inodeTable[inodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return 0;
	}
	else
	{
		processNumber = Procesor::getRunning()->PID;
		unsigned short int toRead = bytes_amount;
		bool is = false;
		for (int i = 0; i < processOFT.size(); i++)
		{
			if (processOFT[i].processID == processNumber)
			{
				for (int j = 0; j < processOFT[i].files.size(); j++)
					if (processOFT[i].files[j].first == inodeNumber)
					{
						if (isAllowedToModify(file_name, false))
						{
							if (bytes_amount > inodeTable[inodeNumber].getSize() - processOFT[i].positions[j])
								toRead = inodeTable[inodeNumber].getSize() - processOFT[i].positions[j];
							std::vector<unsigned short int>blocks;
							if (toRead >= 0)
								blocks.push_back(inodeTable[inodeNumber].getdb1());
							if (toRead > 32)
								blocks.push_back(inodeTable[inodeNumber].getdb2());
							std::vector<short int> ibBlocks = readIB(inodeTable[inodeNumber].getib());
							for (int i = 0; i < ibBlocks.size(); i++)
							{
								if (toRead > (64 + (i * 32)))
									blocks.push_back(ibBlocks[i]);
							}
							unsigned short int read = dRead(data, blocks, toRead, processOFT[i].positions[j] % 32);
							processOFT[i].increasePosition(read, j);
							return read;
						}
						else
						{
							std::cout << blue << "[SZPIK]" << red << " Blad Uprawnien: Proces nie ma uprawnien do dzialania na tym pliku." << white << std::endl;
							return 0;
						}
					}
				std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl tego pliku." << white << std::endl;
				return 0;
			}
		}
		std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl zadnego pliku." << white << std::endl;
		return 0;
	}
}

short int read(std::string file_name, char* &data, unsigned short int byte_number_in_file, unsigned short int bytes_amount)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return 0;
	}
	short int inodeNumber = fsearch(file_name, 0);
	if (inodeNumber < 0 || inodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return 0;
	}
	if (inodeTable[inodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return 0;
	}
	else
	{
		processNumber = Procesor::getRunning()->PID;
		if (byte_number_in_file > inodeTable[inodeNumber].getSize() - 1)
		{
			std::cout << blue << "[SZPIK]" << red << " Punkt startu wykracza poza obszar pliku!" << white << std::endl;
			return 0;
		}
		unsigned short int toRead = bytes_amount;
		bool is = false;
		for (int i = 0; i < processOFT.size(); i++)
		{
			if (processOFT[i].processID == processNumber)
			{
				for (int j = 0; j < processOFT[i].files.size(); j++)
					if (processOFT[i].files[j].first == inodeNumber)
					{
						if (isAllowedToModify(file_name, false))
						{
							if (bytes_amount > inodeTable[inodeNumber].getSize() - processOFT[i].positions[j])
								toRead = inodeTable[inodeNumber].getSize() - processOFT[i].positions[j];
							std::vector<unsigned short int>blocks;
							if (toRead >= 0)
								blocks.push_back(inodeTable[inodeNumber].getdb1());
							if (toRead > 32)
								blocks.push_back(inodeTable[inodeNumber].getdb2());
							std::vector<short int> ibBlocks = readIB(inodeTable[inodeNumber].getib());
							for (int i = 0; i < ibBlocks.size(); i++)
							{
								if (toRead > (64 + (i * 32)))
									blocks.push_back(ibBlocks[i]);
							}
							unsigned short int read = dRead(data, blocks, toRead, byte_number_in_file % 32);
							return read;
						}
						else
						{
							std::cout << blue << "[SZPIK]" << red << " Blad Uprawnien: Proces nie ma uprawnien do dzialania na tym pliku." << white << std::endl;
							return 0;
						}
					}
				std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl tego pliku." << white << std::endl;
				return 0;
			}
		}
		std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl zadnego pliku." << white << std::endl;
		return 0;
	}
}

short int read(std::string file_name, char* &data, unsigned short int byte_number_in_file, unsigned short int bytes_amount, std::string sfolder_name)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return 0;
	}
	if (sfolder_name.size() < 1 || sfolder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu nadrzednego."  << white << std::endl;
		return false;
	}
	short int inodeNumber = fsearch(file_name, 0, sfolder_name);
	if (inodeNumber < 0 || inodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return 0;
	}
	if (inodeTable[inodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return 0;
	}
	else
	{
		processNumber = Procesor::getRunning()->PID;
		if (byte_number_in_file > inodeTable[inodeNumber].getSize() - 1)
		{
			std::cout << blue << "[SZPIK]" << red << " Punkt startu wykracza poza obszar pliku!" << white << std::endl;
			return 0;
		}
		unsigned short int toRead = bytes_amount;
		bool is = false;
		for (int i = 0; i < processOFT.size(); i++)
		{
			if (processOFT[i].processID == processNumber)
			{
				for (int j = 0; j < processOFT[i].files.size(); j++)
					if (processOFT[i].files[j].first == inodeNumber)
					{
						if (isAllowedToModify(file_name, false))
						{
							if (bytes_amount > inodeTable[inodeNumber].getSize() - processOFT[i].positions[j])
								toRead = inodeTable[inodeNumber].getSize() - processOFT[i].positions[j];
							std::vector<unsigned short int>blocks;
							if (toRead <= 32 && toRead >= 0)
								blocks.push_back(inodeTable[inodeNumber].getdb1());
							if (toRead <= 64 && toRead > 32)
								blocks.push_back(inodeTable[inodeNumber].getdb2());
							std::vector<short int> ibBlocks = readIB(inodeTable[inodeNumber].getib());
							for (int i = 0; i < ibBlocks.size(); i++)
							{
								if (toRead < 64 + ((i + 1) * 32) && toRead >(64 + (i * 32)))
									blocks.push_back(ibBlocks[i]);
							}
							unsigned short int read = dRead(data, blocks, toRead, byte_number_in_file % 32);
							return read;
						}
						else
						{
							std::cout << blue << "[SZPIK]" << red << " Blad Uprawnien: Proces nie ma uprawnien do dzialania na tym pliku." << white << std::endl;
							return 0;
						}
					}
				std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl tego pliku." << white << std::endl;
				return 0;
			}
		}
		std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl zadnego pliku." << white << std::endl;
		return 0;
	}
}

short int lseek(std::string file_name, short int move_by_amount_in_bytes, unsigned short int reference_point)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return -1;
	}
	short int inodeNumber = fsearch(file_name, 0);
	if (inodeNumber < 0 || inodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return -1;
	}
	if (inodeTable[inodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return -1;
	}
	else
	{
		processNumber = Procesor::getRunning()->PID;
		bool is = false;
		for (int i = 0; i < processOFT.size(); i++)
		{
			if (processOFT[i].processID == processNumber)
			{
				for (int j = 0; j < processOFT[i].files.size(); j++)
					if (processOFT[i].files[j].first == inodeNumber)
					{
						if (isAllowedToModify(file_name, processOFT[i].files[j].second))
						{
							switch (reference_point)
							{
							case 0:
							{
								if (processOFT[i].positions[j] + move_by_amount_in_bytes < 0 || processOFT[i].positions[j] + move_by_amount_in_bytes > inodeTable[inodeNumber].getSize() - 1)
								{
									std::cout << blue << "[SZPIK]" << red << " Proba wykroczenia poza granice pliku." << white << std::endl;
									return processOFT[i].positions[j];
								}
								else
								{
									processOFT[i].positions[j] += move_by_amount_in_bytes;
									return processOFT[i].positions[j];
								}
								break;
							}
							case 1:
							{
								if (move_by_amount_in_bytes < 0 || move_by_amount_in_bytes > inodeTable[inodeNumber].getSize() - 1)
								{
									std::cout << blue << "[SZPIK]" << red << " Proba wykroczenia poza granice pliku." << white << std::endl;
									return processOFT[i].positions[j];
								}
								else
								{
									processOFT[i].positions[j] = move_by_amount_in_bytes;
									return processOFT[i].positions[j];
								}
								break;
							}
							case 2:
							{
								if (move_by_amount_in_bytes > 0 || inodeTable[inodeNumber].getSize() - 1 + move_by_amount_in_bytes < 0)
								{
									std::cout << blue << "[SZPIK]" << red << " Proba wykroczenia poza granice pliku." << white << std::endl;
									return processOFT[i].positions[j];
								}
								else
								{
									processOFT[i].positions[j] = inodeTable[inodeNumber].getSize() - 1 + move_by_amount_in_bytes;
									return processOFT[i].positions[j];
								}
								break;
							}
							default:
							{
								std::cout << blue << "[SZPIK]" << red << " Niepoprawny punkt odniesienia." << white << std::endl;
								return processOFT[i].positions[j];
							}
							}
						}
						else
						{
							std::cout << blue << "[SZPIK]" << red << " Blad Uprawnien: Proces nie ma uprawnien do dzialania na tym pliku." << white << std::endl;
							return -1;
						}
					}
				std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl tego pliku." << white << std::endl;
				return -1;
			}
		}
		std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl zadnego pliku." << white << std::endl;
		return -1;
	}
}

short int lseek(std::string file_name, short int move_by_amount_in_bytes, unsigned short int reference_point, std::string sfolder_name)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return -1;
	}
	if (sfolder_name.size() < 1 || sfolder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu nadrzednego."  << white << std::endl;
		return false;
	}
	short int inodeNumber = fsearch(file_name, 0, sfolder_name);
	if (inodeNumber < 0 || inodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return -1;
	}
	if (inodeTable[inodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return -1;
	}
	else
	{
		processNumber = Procesor::getRunning()->PID;
		bool is = false;
		for (int i = 0; i < processOFT.size(); i++)
		{
			if (processOFT[i].processID == processNumber)
			{
				for (int j = 0; j < processOFT[i].files.size(); j++)
					if (processOFT[i].files[j].first == inodeNumber)
					{
						if (isAllowedToModify(file_name, processOFT[i].files[j].second))
						{
							switch (reference_point)
							{
							case 0:
							{
								if (processOFT[i].positions[j] + move_by_amount_in_bytes < 0 || processOFT[i].positions[j] + move_by_amount_in_bytes > inodeTable[inodeNumber].getSize() - 1)
								{
									std::cout << blue << "[SZPIK]" << red << " Proba wykroczenia poza granice pliku." << white << std::endl;
									return processOFT[i].positions[j];
								}
								else
								{
									processOFT[i].positions[j] += move_by_amount_in_bytes;
									return processOFT[i].positions[j];
								}
								break;
							}
							case 1:
							{
								if (move_by_amount_in_bytes < 0 || move_by_amount_in_bytes > inodeTable[inodeNumber].getSize() - 1)
								{
									std::cout << blue << "[SZPIK]" << red << " Proba wykroczenia poza granice pliku." << white << std::endl;
									return processOFT[i].positions[j];
								}
								else
								{
									processOFT[i].positions[j] = move_by_amount_in_bytes;
									return processOFT[i].positions[j];
								}
								break;
							}
							case 2:
							{
								if (move_by_amount_in_bytes > 0 || inodeTable[inodeNumber].getSize() - 1 + move_by_amount_in_bytes < 0)
								{
									std::cout << blue << "[SZPIK]" << red << " Proba wykroczenia poza granice pliku." << white << std::endl;
									return processOFT[i].positions[j];
								}
								else
								{
									processOFT[i].positions[j] = inodeTable[inodeNumber].getSize() - 1 + move_by_amount_in_bytes;
									return processOFT[i].positions[j];
								}
								break;
							}
							default:
							{
								std::cout << blue << "[SZPIK]" << red << " Niepoprawny punkt odniesienia." << white << std::endl;
								return processOFT[i].positions[j];
							}
							}
						}
						else
						{
							std::cout << blue << "[SZPIK]" << red << " Blad Uprawnien: Proces nie ma uprawnien do dzialania na tym pliku." << white << std::endl;
							return -1;
						}
					}
				std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl tego pliku." << white << std::endl;
				return -1;
			}
		}
		std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl zadnego pliku." << white << std::endl;
		return -1;
	}
}

short int write(std::string file_name, std::vector<char> data)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return 0;
	}
	if (data.size() < 1)
	{
		std::cout << blue << "[SZPIK]" << red << " Brak danych do zapisania." << white << std::endl;
		return 0;
	}
	short int inodeNumber = fsearch(file_name, 0);
	if (inodeNumber < 0 || inodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return 0;
	}
	if (inodeTable[inodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return 0;
	}
	else
	{
		processNumber = Procesor::getRunning()->PID;
		for (int i = 0; i < processOFT.size(); i++)
		{
			if (processOFT[i].processID == processNumber)
			{
				for (int j = 0; j < processOFT[i].files.size(); j++)
					if (processOFT[i].files[j].first == inodeNumber)
					{
						if (isAllowedToModify(file_name, true))
						{
							int executed = 0;
							if (data.size() <= inodeTable[inodeNumber].getSize() - processOFT[i].positions[j])
							{
								std::vector<unsigned short int> blocks;
								if (inodeTable[inodeNumber].getdb1() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb1());
								}
								if (inodeTable[inodeNumber].getdb2() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb2());
								}
								if (inodeTable[inodeNumber].getib() > -1)
								{
									std::vector<short> IBblocks = readIB(inodeTable[inodeNumber].getib());
									for (int i = 0; i < IBblocks.size(); i++)
										blocks.push_back(IBblocks[i]);
								}
								if (blocks.size() <= 0)
									return 0;
								else
								{
									executed = dWrite(data, blocks, processOFT[i].positions[j] % 32);
									processOFT[i].positions[j] += executed;
									return executed;
								}
							}
							else
							{
								short int howMuch = data.size() - (inodeTable[inodeNumber].getSize() - processOFT[i].positions[j]);
								short int howManyTimes = (howMuch / 32) + 1;
								if (howMuch % 32 > 0)
									howManyTimes++;
								for (int i = 0; i < howManyTimes; i++)
									expandFile(inodeNumber);
								std::vector<unsigned short int> blocks;
								if (inodeTable[inodeNumber].getdb1() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb1());
								}
								if (inodeTable[inodeNumber].getdb2() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb2());
								}
								if (inodeTable[inodeNumber].getib() > -1)
								{
									std::vector<short> IBblocks = readIB(inodeTable[inodeNumber].getib());
									for (int i = 0; i < IBblocks.size(); i++)
										blocks.push_back(IBblocks[i]);
								}
								if (blocks.size() <= 0)
									return 0;
								else
								{
									executed = dWrite(data, blocks, processOFT[i].positions[j] % 32);
									processOFT[i].positions[j] += executed;
									inodeTable[inodeNumber].setSize(processOFT[i].positions[j]);
									return executed;
								}
							}
						}
						else
						{
							std::cout << blue << "[SZPIK]" << red << " Blad Uprawnien: Proces nie ma uprawnien do dzialania na tym pliku." << white << std::endl;
							return 0;
						}
					}
				std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl tego pliku." << white << std::endl;
				return 0;
			}
		}
		std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl zadnego pliku." << white << std::endl;
		return 0;
	}
}

short int write(std::string file_name, std::vector<char> data, std::string sfolder_name)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return 0;
	}
	if (sfolder_name.size() < 1 || sfolder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu nadrzednego."  << white << std::endl;
		return 0;
	}
	if (data.size() < 1)
	{
		std::cout << blue << "[SZPIK]" << red << " Brak danych do zapisania." << white << std::endl;
		return 0;
	}
	short int inodeNumber = fsearch(file_name, 0, sfolder_name);
	if (inodeNumber < 0 || inodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return 0;
	}
	if (inodeTable[inodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return 0;
	}
	else
	{
		processNumber = Procesor::getRunning()->PID;
		for (int i = 0; i < processOFT.size(); i++)
		{
			if (processOFT[i].processID == processNumber)
			{
				for (int j = 0; j < processOFT[i].files.size(); j++)
					if (processOFT[i].files[j].first == inodeNumber)
					{
						if (isAllowedToModify(file_name, true))
						{
							int executed = 0;
							if (data.size() <= inodeTable[inodeNumber].getSize() - processOFT[i].positions[j])
							{
								std::vector<unsigned short int> blocks;
								if (inodeTable[inodeNumber].getdb1() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb1());
								}
								if (inodeTable[inodeNumber].getdb2() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb2());
								}
								if (inodeTable[inodeNumber].getib() > -1)
								{
									std::vector<short> IBblocks = readIB(inodeTable[inodeNumber].getib());
									for (int i = 0; i < IBblocks.size(); i++)
										if (data.size() < (i + 3) * 32)
											blocks.push_back(IBblocks[i]);
								}
								if (blocks.size() <= 0)
									return 0;
								else
								{
									executed = dWrite(data, blocks, processOFT[i].positions[j] % 32);
									processOFT[i].positions[j] += executed;
									return executed;
								}
							}
							else
							{
								short int howMuch = data.size() - (inodeTable[inodeNumber].getSize() - processOFT[i].positions[j]);
								short int howManyTimes = howMuch / 32;
								if (howMuch % 32 > 0)
									howManyTimes++;
								for (int i = 0; i < howManyTimes; i++)
									expandFile(inodeNumber);
								std::vector<unsigned short int> blocks;
								if (inodeTable[inodeNumber].getdb1() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb1());
								}
								if (inodeTable[inodeNumber].getdb2() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb2());
								}
								if (inodeTable[inodeNumber].getib() > -1)
								{
									std::vector<short> IBblocks = readIB(inodeTable[inodeNumber].getib());
									for (int i = 0; i < IBblocks.size(); i++)
										if (data.size() < (i + 3) * 32)
											blocks.push_back(IBblocks[i]);
								}
								if (blocks.size() <= 0)
									return 0;
								else
								{
									executed = dWrite(data, blocks, processOFT[i].positions[j] % 32);
									processOFT[i].positions[j] += executed;
									inodeTable[inodeNumber].setSize(processOFT[i].positions[j]);
									return executed;
								}
							}
						}
						else
						{
							std::cout << blue << "[SZPIK]" << red << " Blad Uprawnien: Proces nie ma uprawnien do dzialania na tym pliku." << white << std::endl;
							return 0;
						}
					}
				std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl tego pliku." << white << std::endl;
				return 0;
			}
		}
		std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl zadnego pliku." << white << std::endl;
		return 0;
	}
}

short int write(std::string file_name, std::vector<char> data, unsigned short int byte_number_in_file)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return 0;
	}
	if (data.size() < 1)
	{
		std::cout << blue << "[SZPIK]" << red << " Brak danych do zapisania." << white << std::endl;
		return 0;
	}
	short int inodeNumber = fsearch(file_name, 0);
	if (inodeNumber < 0 || inodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return 0;
	}
	if (inodeTable[inodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return 0;
	}
	else
	{
		processNumber = Procesor::getRunning()->PID;
		if (byte_number_in_file > inodeTable[inodeNumber].getSize() - 1)
		{
			std::cout << blue << "[SZPIK]" << red << " Punkt startu wykracza poza obszar pliku!" << white << std::endl;
			return 0;
		}
		for (int i = 0; i < processOFT.size(); i++)
		{
			if (processOFT[i].processID == processNumber)
			{
				for (int j = 0; j < processOFT[i].files.size(); j++)
					if (processOFT[i].files[j].first == inodeNumber)
					{
						if (isAllowedToModify(file_name, true))
						{
							int executed = 0;
							if (data.size() <= inodeTable[inodeNumber].getSize() - byte_number_in_file)
							{
								std::vector<unsigned short int> blocks;
								if (inodeTable[inodeNumber].getdb1() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb1());
								}
								if (inodeTable[inodeNumber].getdb2() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb2());
								}
								if (inodeTable[inodeNumber].getib() > -1)
								{
									std::vector<short> IBblocks = readIB(inodeTable[inodeNumber].getib());
									for (int i = 0; i < IBblocks.size(); i++)
										if (data.size() < (i + 3) * 32)
											blocks.push_back(IBblocks[i]);
								}
								if (blocks.size() <= 0)
									return 0;
								else
								{
									executed = dWrite(data, blocks, byte_number_in_file % 32);
									return executed;
								}
							}
							else
							{
								short int howMuch = data.size() - (inodeTable[inodeNumber].getSize() - processOFT[i].positions[j]);
								short int howManyTimes = howMuch / 32;
								if (howMuch % 32 > 0)
									howManyTimes++;
								for (int i = 0; i < howManyTimes; i++)
									expandFile(inodeNumber);
								std::vector<unsigned short int> blocks;
								if (inodeTable[inodeNumber].getdb1() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb1());
								}
								if (inodeTable[inodeNumber].getdb2() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb2());
								}
								if (inodeTable[inodeNumber].getib() > -1)
								{
									std::vector<short> IBblocks = readIB(inodeTable[inodeNumber].getib());
									for (int i = 0; i < IBblocks.size(); i++)
										if (data.size() < (i + 3) * 32)
											blocks.push_back(IBblocks[i]);
								}
								if (blocks.size() <= 0)
									return 0;
								else
								{
									executed = dWrite(data, blocks, byte_number_in_file % 32);
									inodeTable[inodeNumber].setSize(processOFT[i].positions[j]);
									return executed;
								}
							}
						}
						else
						{
							std::cout << blue << "[SZPIK]" << red << " Blad Uprawnien: Proces nie ma uprawnien do dzialania na tym pliku." << white << std::endl;
							return 0;
						}
					}
				std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl tego pliku." << white << std::endl;
				return 0;
			}
		}
		std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl zadnego pliku." << white << std::endl;
		return 0;
	}
}

short int write(std::string file_name, std::vector<char> data, unsigned short int byte_number_in_file, std::string sfolder_name)
{
	if (file_name.size() < 1 || file_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy pliku." << white << std::endl;
		return 0;
	}
	if (sfolder_name.size() < 1 || sfolder_name.size() > 7)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawna dlugosc nazwy folderu nadrzednego."  << white << std::endl;
		return 0;
	}
	if (data.size() < 1)
	{
		std::cout << blue << "[SZPIK]" << red << " Brak danych do zapisania." << white << std::endl;
		return 0;
	}
	short int inodeNumber = fsearch(file_name, 0, sfolder_name);
	if (inodeNumber < 0 || inodeNumber > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Nie odnaleziono pliku." << white << std::endl;
		return 0;
	}
	if (inodeTable[inodeNumber].getType() == true)
	{
		std::cout << blue << "[SZPIK]" << red << " Ta funkcja nie dziala na foldery!" << white << std::endl;
		return 0;
	}
	else
	{
		processNumber = Procesor::getRunning()->PID;
		if (byte_number_in_file > inodeTable[inodeNumber].getSize() - 1)
		{
			std::cout << blue << "[SZPIK]" << red << " Punkt startu wykracza poza obszar pliku!" << white << std::endl;
			return 0;
		}
		for (int i = 0; i < processOFT.size(); i++)
		{
			if (processOFT[i].processID == processNumber)
			{
				for (int j = 0; j < processOFT[i].files.size(); j++)
					if (processOFT[i].files[j].first == inodeNumber)
					{
						if (isAllowedToModify(file_name, true))
						{
							int executed = 0;
							if (data.size() <= inodeTable[inodeNumber].getSize() - byte_number_in_file)
							{
								std::vector<unsigned short int> blocks;
								if (inodeTable[inodeNumber].getdb1() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb1());
								}
								if (inodeTable[inodeNumber].getdb2() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb2());
								}
								if (inodeTable[inodeNumber].getib() > -1)
								{
									std::vector<short> IBblocks = readIB(inodeTable[inodeNumber].getib());
									for (int i = 0; i < IBblocks.size(); i++)
										if (data.size() < (i + 3) * 32)
											blocks.push_back(IBblocks[i]);
								}
								if (blocks.size() <= 0)
									return 0;
								else
								{
									executed = dWrite(data, blocks, byte_number_in_file % 32);
									return executed;
								}
							}
							else
							{
								short int howMuch = data.size() - (inodeTable[inodeNumber].getSize() - processOFT[i].positions[j]);
								short int howManyTimes = howMuch / 32;
								if (howMuch % 32 > 0)
									howManyTimes++;
								for (int i = 0; i < howManyTimes; i++)
									expandFile(inodeNumber);
								std::vector<unsigned short int> blocks;
								if (inodeTable[inodeNumber].getdb1() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb1());
								}
								if (inodeTable[inodeNumber].getdb2() > -1)
								{
									blocks.push_back(inodeTable[inodeNumber].getdb2());
								}
								if (inodeTable[inodeNumber].getib() > -1)
								{
									std::vector<short> IBblocks = readIB(inodeTable[inodeNumber].getib());
									for (int i = 0; i < IBblocks.size(); i++)
										if (data.size() < (i + 3) * 32)
											blocks.push_back(IBblocks[i]);
								}
								if (blocks.size() <= 0)
									return 0;
								else
								{
									executed = dWrite(data, blocks, byte_number_in_file % 32);
									inodeTable[inodeNumber].setSize(processOFT[i].positions[j]);
									return executed;
								}
							}
						}
						else
						{
							std::cout << blue << "[SZPIK]" << red << " Blad Uprawnien: Proces nie ma uprawnien do dzialania na tym pliku." << white << std::endl;
							return 0;
						}
					}
				std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl tego pliku." << white << std::endl;
				return 0;
			}
		}
		std::cout << blue << "[SZPIK]" << red << " Ten proces nie otworzyl zadnego pliku." << white << std::endl;
		return 0;
	}
}

void drawBlock(unsigned short int block)
{
	allocateFreeMemory();
	if (block < 0 || block > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawny numer bloku dyskowego." << white << std::endl;
		return;
	}
	else
	{
		std::cout << (char)201;
		for (int i = 0; i < 4; i++)
			std::cout << (char)205;
		std::cout << "Blok " << std::setw(2) << block;
		for (int i = 0; i < 4; i++)
			std::cout << (char)205;
		std::cout << (char)187 << std::endl;
		char* data;
		std::vector<unsigned short int> blocks = { block };
		dRead(data, blocks, 0, 32);
		for (int i = 0; i < 4; i++)
		{
			std::cout << (char)186 << std::setw(1) << data[i * 8] << " " << std::setw(1) << data[(i * 8) + 1] << " " << std::setw(1) << data[(i * 8) + 2] << " " << std::setw(1) << data[(i * 8) + 3] << " " << std::setw(1) << data[(i * 8) + 4] << " " << std::setw(1) << data[(i * 8) + 5] << " " << std::setw(1) << data[(i * 8) + 6] << " " << std::setw(1) << data[(i * 8) + 7] << (char)186 << std::endl;
		}
		std::cout << (char)200;
		for (int i = 0; i < 15; i++)
			std::cout << (char)205;
		std::cout << (char)188 << std::endl;
	}
}

void drawBlockD(unsigned short int block)
{
	allocateFreeMemory();
	if (block < 0 || block > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawny numer bloku dyskowego." << white << std::endl;
		return;
	}
	else
	{
		std::cout << (char)201;
		for (int i = 0; i < 12; i++)
			std::cout << (char)205;
		std::cout << "Blok " << std::setw(2) << block;
		for (int i = 0; i < 12; i++)
			std::cout << (char)205;
		std::cout << (char)187 << std::endl;
		char* data;
		std::vector<unsigned short int> blocks = { block };
		dRead(data, blocks, 0, 32);
		for (int i = 0; i < 4; i++)
		{
			std::cout << (char)186 << std::setw(3) << (int)data[i * 8] << " " << std::setw(3) << (int)data[(i * 8) + 1] << " " << std::setw(3) << (int)data[(i * 8) + 2] << " " << std::setw(3) << (int)data[(i * 8) + 3] << " " << std::setw(3) << (int)data[(i * 8) + 4] << " " << std::setw(3) << (int)data[(i * 8) + 5] << " " << std::setw(3) << (int)data[(i * 8) + 6] << " " << std::setw(3) << (int)data[(i * 8) + 7] << (char)186 << std::endl;
		}
		std::cout << (char)200;
		for (int i = 0; i < 31; i++)
			std::cout << (char)205;
		std::cout << (char)188 << std::endl;
	}
}

void drawDisc()
{
	allocateFreeMemory();
	for (unsigned int i = 0; i < 8; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			std::cout << (char)201;
			for (int i = 0; i < 4; i++)
				std::cout << (char)205;
			std::cout << "Blok " << std::setw(2) << j + (i * 4);
			for (int i = 0; i < 4; i++)
				std::cout << (char)205;
			std::cout << (char)187;
		}
		std::cout << std::endl;
		for (unsigned int j = 0; j < 4; j++)
		{
			for (unsigned int k = 0; k < 4; k++)
			{
				char* data;
				unsigned short int buff = (i * 4) + k;
				std::vector<unsigned short int> blocks = { buff };
				buff = j * 8;
				dRead(data, blocks, buff, 8);
				std::cout << (char)186 << std::setw(1) << data[0] << " " << std::setw(1) << data[1] << " " << std::setw(1) << data[2] << " " << std::setw(1) << data[3] << " " << std::setw(1) << data[4] << " " << std::setw(1) << data[5] << " " << std::setw(1) << data[6] << " " << std::setw(1) << data[7] << (char)186;
			}
			std::cout << std::endl;
		}
		for (int j = 0; j < 4; j++)
		{
			std::cout << (char)200;
			for (int i = 0; i < 15; i++)
				std::cout << (char)205;
			std::cout << (char)188;
		}
		std::cout << std::endl;
	}
}

void drawDiscD()
{
	allocateFreeMemory();
	for (unsigned int i = 0; i < 16; i++)
	{
		for (unsigned int j = 0; j < 2; j++)
		{
			std::cout << (char)201;
			for (int i = 0; i < 12; i++)
				std::cout << (char)205;
			std::cout << "Blok " << std::setw(2) << j + (i * 2);
			for (int i = 0; i < 12; i++)
				std::cout << (char)205;
			std::cout << (char)187;
		}
		std::cout << std::endl;
		for (unsigned int j = 0; j < 4; j++)
		{
			for (unsigned int k = 0; k < 2; k++)
			{
				char* data;
				unsigned short int buff = (i * 4) + k;
				std::vector<unsigned short int> blocks = { buff };
				buff = j * 8;
				dRead(data, blocks, buff, 8);
				std::cout << (char)186 << std::setw(3) << (int)data[i * 8] << " " << std::setw(3) << (int)data[(i * 8) + 1] << " " << std::setw(3) << (int)data[(i * 8) + 2] << " " << std::setw(3) << (int)data[(i * 8) + 3] << " " << std::setw(3) << (int)data[(i * 8) + 4] << " " << std::setw(3) << (int)data[(i * 8) + 5] << " " << std::setw(3) << (int)data[(i * 8) + 6] << " " << std::setw(3) << (int)data[(i * 8) + 7] << (char)186;
			}
			std::cout << std::endl;
		}
		for (int j = 0; j < 2; j++)
		{
			std::cout << (char)200;
			for (int i = 0; i < 31; i++)
				std::cout << (char)205;
			std::cout << (char)188;
		}
		std::cout << std::endl;
	}
}

void drawInode(unsigned short int inode)
{
	if (inode < 0 || inode > 31)
	{
		std::cout << blue << "[SZPIK]" << red << " Niepoprawny numer I-Wezla." << white <<std::endl;
		return;
	}
	else
	{
		std::cout << std::left;
		std::cout << "I-Wezel nr " << inode << std::endl;
		std::cout << (char)204 << std::setw(18) << "Typ:" << " ";
		if (inodeTable[inode].getType())
			std::cout << "Katalog";
		else
			std::cout << "Plik";
		std::cout << std::endl;
		std::cout << (char)204 << std::setw(18) << "Ilosc Dowiazan:" << " " << inodeTable[inode].getLinkCounter() << std::endl;
		std::cout << (char)204 << std::setw(18) << "Rozmiar:" << " " << inodeTable[inode].getSize() << "B" << std::endl;
		std::cout << (char)204 << std::setw(18) << "Blok Dyskowy nr 1:" << " " << inodeTable[inode].getdb1() << std::endl;
		std::cout << (char)204 << std::setw(18) << "Blok Dyskowy nr 2:" << " " << inodeTable[inode].getdb2() << std::endl;
		std::cout << (char)200 << std::setw(18) << "Blok Indeksowy:" << " " << inodeTable[inode].getib() << std::endl;
	}
}