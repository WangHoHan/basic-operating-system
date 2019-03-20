#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include"pipes.h"
#include"Procesy.h"
#include "Memory.h"
#include "fafms.h"
#include "ConsoleColor.h"

extern class Procesor;
extern PCB_tree drzewko;

int AX;
int BX;
int CX;
int DX;
int CommandCounter;
int PID;

void SaveRegisters(); //zapisanie rejetrow po wykonanym rozkazie
void ShowRegisters();  //wyswietlenie wartosci rejestrow
void GetRegisters(); //pobranie rejestrow z RUNNING procesu
std::string GetCommand(); //pobranie rozkazu z pamieci RAM

void SaveRegisters()
{

	PCB* pcb = Procesor::getRunning();
	pcb->counter = CommandCounter;
	pcb->registers[0] = AX;
	pcb->registers[1] = BX;
	pcb->registers[2] = CX;
	pcb->registers[3] = DX;
	pcb->PID = PID;
}

void ShowRegisters()
{
	std::cout << yellow << "[ASSEMBLER]" << white << " Process PID: " << PID << std::endl;
	std::cout << yellow << "[ASSEMBLER]" << white << " AX: " << AX << " BX: " << BX << " CX: " << CX << " DX: " << DX << std::endl;
	std::cout << yellow << "[ASSEMBLER]" << white << " CommandCounter: " << CommandCounter << std::endl;
}

void GetRegisters()
{

	PCB *pcb = Procesor::getRunning();
	AX = pcb->registers[0];
	BX = pcb->registers[1];
	CX = pcb->registers[2];
	DX = pcb->registers[3];
	PID = pcb->PID;
	CommandCounter = pcb->counter;

}

std::string GetCommand()
{

	PCB *pcb = Procesor::getRunning();

	std::string command;
	command = getOrder(pcb, CommandCounter);
	CommandCounter += command.size() + 1;
	return command;
}
int signum(int x)
{
	if (x > 0)
	{
		return 1;
	}
	else if (x < 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

//wykonanie rozkazu, argumenty funckcji do dopisania
void Execute()
{
	PCB *pcb = Procesor::getRunning();
	std::string C1, C2;
	int temporary = 0;
	GetRegisters();
	std::string command;
	//command.clear(); 
	command = GetCommand();
	std::cout << yellow << "[ASSEMBLER]" << white << " Command is: " << command << " ";

	//DODAC ZABEZPIECZENIE PRZED ZEREM I INNE PIERDOLY
	//checking operation
	if (command == "AD")
	{
		C1 = GetCommand();
		C2 = GetCommand();
		if (C1.length() >= 10 || C2.length() >= 10)
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Wpisano zbyt duze liczby" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		
		//C1=DX
		if (C1 == "DX")
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nie mozna wykorzystywaC rejsetru DX do operacji" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		if (C2[0]>67)
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Nalezy wpisac liczbe" << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		std::cout << C1 << " " << C2 << std::endl;
		if (C2[1] != 'X')
		{
			temporary = stoi(C2);
		}

		//C1==AX
	
		if (C1 == "AX" && temporary != 0)
		{
		   AX + temporary;
						
		}
		else if (C1[0] == 'A' && C2[0] == 'B')
		{
			AX += BX;
		}
		else if (C1 == "AX" && C2 == "CX")
		{
			AX += CX;
		}
		else if (C1 == "AX" && C2 == "AX")
		{
			AX += AX;
		}
		else if (C1 == "AX" && C2 == "DX")
		{

			std::cout << yellow << "[ASSEMBLER]" << white << " Nieprawid這wy drugi argument" << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

		// C1=BX
		if (C1 == "BX" && temporary != 0)
		{
			BX += temporary;
		}
		else if (C1 == "BX" && C2 == "AX")
		{
			BX += AX;
		}
		else if (C1 == "BX" && C2 == "CX")
		{
			BX += CX;
		}
		else if (C1 == "BX" && C2 == "BX")
		{
			BX += BX;
		}
		else if (C1 == "BX" && C2 == "DX")
		{

			std::cout << yellow << "[ASSEMBLER]" << white << " Nieprawidlowy drugi argument" << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

		//C1=CX
		if (C1 == "CX" && temporary != 0)
		{
			CX += temporary;
		}
		else if (C1 == "CX" && C2 == "BX")
		{
			CX += BX;
		}
		else if (C1 == "CX" && C2 == "AX")
		{
			CX += AX;
		}
		else if (C1 == "CX" && C2 == "CX")
		{
			CX += CX;
		}
		else if (C1 == "CX" && C2 == "DX")
		{

			std::cout << yellow << "[ASSEMBLER]" << white << " Nieprawid這wy drugi argument" << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

		ShowRegisters();
		SaveRegisters();
		Procesor::incCOUNT();
	}

	//odejmowanie 
	//DX
	if (command == "SB")
	{

		C1 = GetCommand();
		C2 = GetCommand();
		if (C1.length() >= 10 || C2.length() >= 10)
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Wpisano zbyt duze liczby" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		if (C1 == "DX")
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nie mozna wykorzystywac rejsetru DX do operacji" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		if (C2[0] > 67)
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Nalezy wpisac liczbe" << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		std::cout << C1 << " " << C2 << std::endl;
		if (C2 != "AX" && C2 != "BX" && C2 != "CX" && C2 != "DX")
		{
			temporary = stoi(C2);
		}

		//C1==A

		if (C1 == "AX" && temporary != 0)
		{
			AX -= temporary;
		}
		else if (C1 == "AX" && C2 == "BX")
		{
			AX -= BX;
		}
		else if (C1 == "AX" && C2 == "CX")
		{
			AX -= CX;
		}
		else if (C1 == "AX" && C2 == "AX")
		{
			AX -= AX;
		}
		else if (C1 == "AX" && C2 == "DX")
		{

			std::cout << yellow << "[ASSEMBLER]" << white << " Nieprawidlowy drugi argument" << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

		// C1=BX
		if (C1 == "BX" && temporary != 0)
		{
			BX -= temporary;
		}
		else if (C1 == "BX" && C2 == "AX")
		{
			BX -= AX;
		}
		else if (C1 == "BX" && C2 == "CX")
		{
			BX -= CX;
		}
		else if (C1 == "BX" && C2 == "BX")
		{
			BX -= BX;
		}
		else if (C1 == "BX" && C2 == "DX")
		{

			std::cout << yellow << "[ASSEMBLER]" << white << " Nieprawidlowy drugi argument" << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

		//C1=CX
		if (C1 == "CX" && temporary != 0)
		{
			CX -= temporary;
		}
		else if (C1 == "CX" && C2 == "BX")
		{
			CX -= BX;
		}
		else if (C1 == "CX" && C2 == "AX")
		{
			CX -= AX;
		}
		else if (C1 == "CX" && C2 == "CX")
		{
			CX -= CX;
		}
		else if (C1 == "CX" && C2 == "DX")
		{

			std::cout << yellow << "[ASSEMBLER]" << white << " Nieprawidlowy drugi argument" << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

		ShowRegisters();
		SaveRegisters();
		Procesor::incCOUNT();
	}

	//DZIELENIE
	if (command == "DV")
	{
		C1 = GetCommand();
		C2 = GetCommand();
		
		if (C1.length() >= 10 || C2.length() >= 10)
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Wpisano zbyt duze liczby" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		if (C2[0] > 67)
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nalezy wpisac liczbe" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		if (stoi(C2) == 0)
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nie mozna dzielic przez 0!" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		if (C1 == "DX")
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nie mozna wykorzystywaC rejsetru DX do operacji" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		
		std::cout << C1 << " " << C2 << std::endl;
		if (C2 != "AX" && C2 != "BX" && C2 != "CX" && C2 != "DX")
		{
			temporary = stoi(C2);
		}

		//C1==A
		if (C1 == "AX" && temporary != 0)
		{
			AX /= temporary;
		}
		else if (C1 == "AX" && C2 == "BX")
		{
			AX /= BX;
		}
		else if (C1 == "AX" && C2 == "CX")
		{
			AX /= CX;
		}
		else if (C1 == "AX" && C2 == "AX")
		{
			AX /= AX;
		}
		else if (C1 == "AX" && C2 == "DX")
		{

			std::cout << yellow << "[ASSEMBLER]" << white << " Nieprawidlowy drugi argument" << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}


		// C1=BX
		if (C1 == "BX" && temporary != 0)
		{
			BX /= temporary;
		}
		else if (C1 == "BX" && C2 == "AX")
		{
			BX /= AX;
		}
		else if (C1 == "BX" && C2 == "CX")
		{
			BX /= CX;
		}
		else if (C1 == "BX" && C2 == "BX")
		{
			BX /= BX;
		}
		else if (C1 == "BX" && C2 == "DX")
		{

			std::cout << yellow << "[ASSEMBLER]" << white << " Nieprawidlowy drugi argument" << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

		//C1=CX
		if (C1 == "CX" && temporary != 0)
		{
			CX /= temporary;
		}
		else if (C1 == "CX" && C2 == "BX")
		{
			CX /= BX;
		}
		else if (C1 == "CX" && C2 == "AX")
		{
			CX /= AX;
		}
		else if (C1 == "CX" && C2 == "CX")
		{
			CX /= CX;
		}
		else if (C1 == "CX" && C2 == "DX")
		{

			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidlowy drugi argument" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		ShowRegisters();
		SaveRegisters();
		Procesor::incCOUNT();
	}

	//MNOZENIE
	if (command == "ML")
	{
		C1 = GetCommand();
		C2 = GetCommand();
		if (C1.length() >= 10 || C2.length() >= 10)
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Wpisano zbyt duze liczby" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		if (C1 == "DX")
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nie mozna wykorzystywac rejsetru DX do operacji" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		if (C2[0] > 67)
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nalezy wpisac liczbe" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		std::cout << C1 << " " << C2 << std::endl;
		if (C2 != "AX" && C2 != "BX" && C2 != "CX" && C2 != "DX")
		{
			temporary = stoi(C2);
		}

		//C1==AX
		if (C1 == "AX")
		{
			AX *= temporary;
		}
		else if (C1 == "AX" && C2 == "BX")
		{
			AX *= BX;
		}
		else if (C1 == "AX" && C2 == "CX")
		{
			AX *= CX;
		}
		else if (C1 == "AX" && C2 == "AX")
		{
			AX *= AX;
		}

		else if (C1 == "AX" && C2 == "DX")
		{

			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidlowy drugi argument" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

		// C1=BX
		if (C1 == "BX")
		{
			BX *= temporary;
		}
		else if (C1 == "BX" && C2 == "AX")
		{
			BX *= AX;
		}
		else if (C1 == "BX" && C2 == "CX")
		{
			BX *= CX;
		}
		else if (C1 == "BX" && C2 == "BX")
		{
			BX *= BX;
		}
		else if (C1 == "BX" && C2 == "DX")
		{

			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidlowy drugi argument" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

		//C1=CX
		if (C1 == "CX")
		{
			CX *= temporary;
		}
		else if (C1 == "CX" && C2 == "BX")
		{
			CX *= BX;
		}
		else if (C1 == "CX" && C2 == "AX")
		{
			CX *= AX;
		}
		else if (C1 == "CX" && C2 == "CX")
		{
			CX *= CX;
		}
		else if (C1 == "CX" && C2 == "DX")
		{

			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidlowy drugi argument" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

		ShowRegisters();
		SaveRegisters();
		Procesor::incCOUNT();
	}
	if (command == "CO")
	{
		int x = 0, y = 0;
		C1 = GetCommand();
		C2 = GetCommand();
		std::cout << " " << C1 << " " << C2 << std::endl;
		if (C1 == "DX")
		{
			std::cout <<yellow << "[ASSEMBLER]" << red << " Nie mozna wykorzystywac rejsetru DX do operacji" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		if (C1 == "AX")
		{
			x = AX;

		}
		else if (C1 == "BX")
		{
			x = BX;
		}
		else if (C1 == "CX")
		{
			x = CX;
		}
		else
		{
			x = stoi(C1);
		}

		if (C2 == "AX")
		{
			y = AX;

		}
		else if (C2 == "BX")
		{
			y = BX;
		}
		else if (C2 == "CX")
		{
			y = CX;
		}
		else if (C2 == "DX")
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nie mozna wykorzystywac rejsetru DX do operacji" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		else
		{
			y = stoi(C2);
		}

		DX = signum(x - y);
		ShowRegisters();
		SaveRegisters();
		Procesor::incCOUNT();

	}

	//INKREMENTACJA I DEKREMENTACJA REJETRU
	if (command[0] == 'D'&&command[1] == 'C')
	{
		C1 = GetCommand();
		if (C1 == "DX")
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nie mozna wykorzystywac rejsetru DX do operacji" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		
		std::cout << C1 << std::endl;
		if (C1 == "AX")
			AX--;
		if (C1 == "BX")
			BX--;
		if (C1 == "CX")
			CX--;

		ShowRegisters();
		SaveRegisters();
		Procesor::incCOUNT();
	}
	if (command == "IN")
	{
		C1 = GetCommand();
		if (C1 == "DX")
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nie mozna wykorzystywac rejsetru DX do operacji" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		std::cout << C1 << std::endl;
		if (C1 == "AX")
			AX++;
		if (C1 == "BX")
			BX++;
		if (C1 == "CX")
			CX++;
		ShowRegisters();
		SaveRegisters();
		Procesor::incCOUNT();
	}

	//ZAPIS DO REJESTRU
	if (command == "MO")
	{

		C1 = GetCommand();
		C2 = GetCommand();
		if (C1.length() >= 9 || C2.length() >= 9)
		{
			std::cout << yellow << "[ASSEMBLER]"<< red << " Wpisano zbyt duze liczby" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		else if (C1 == "DX")
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nie mozna wykorzystywac rejsetru DX do operacji" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		std::cout << C1 << " " << C2 << std::endl;

		if (C2 != "AX" && C2 != "BX" && C2 != "CX" && C2 != "DX")
		{
			temporary = stoi(C2);
		}
		//////////

		if (C1 == "AX" && temporary != 0)
		{
			AX = temporary;
		}
		else if (C1 == "AX" && C2 == "BX")
		{
			AX = BX;
		}
		else if (C1 == "AX" && C2 == "CX")
		{
			AX = CX;
		}
		///////
		if (C1 == "BX" && temporary != 0)
		{
			BX = temporary;
		}
		else if (C1 == "BX" && C2 == "AX")
		{
			BX = CX;
		}
		else if (C1 == "BX" && C2 == "CX")
		{
			BX = CX;
		}

		///////////////////
		if (C1 == "CX" && temporary != 0)
		{
			CX = temporary;
		}
		else if (C1 == "CX" && C2 == "BX")
		{
			CX = BX;
		}
		else if (C1 == "CX" && C2 == "AX")
		{
			CX = AX;
		}

		ShowRegisters();
		SaveRegisters();
		Procesor::incCOUNT();
	}

	////////SKOKI
	if (command == "JP")
	{
		C1 = GetCommand();
		if (C1[0]=='-')
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidlowy adres" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		if (C1[0] > 64)
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Nieprawidlowy adres" << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		std::cout << C1 << std::endl;
		CommandCounter = stoi(C1);

		ShowRegisters();
		SaveRegisters();
		Procesor::incCOUNT();

	}

	//SKOK POD WARUNKIEM

	if (command == "JZ")//skacze jak jest 0
	{
		C1 = GetCommand();
		std::cout << C1 << std::endl;
		if (C1[0] == '-')
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidlowy adres" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		if (C1[0] > 64)
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidlowy adres" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		int skok = 0;
		skok = stoi(C1);
		if (DX == 0)
		{

			CommandCounter = skok;
		}
		ShowRegisters();
		SaveRegisters();
		Procesor::incCOUNT();
	}
	if (command == "JN")//skacze jak nie jest 0
	{
		
		C1 = GetCommand();
		std::cout << C1 << std::endl;
		if (C1[0] == '-')
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidlowy adres" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		if (C1[0] > 64)
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidlowy adres" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		else
		{
			int skok = stoi(C1);
			if (DX != 0)
			{
				CommandCounter = skok;
			}
			ShowRegisters();
			SaveRegisters();
			Procesor::incCOUNT();
		}
	}

	if (command == "OT")//wyswietlenie rejestru
	{
		C1 = GetCommand();
		if (C1 == "AX")
		{
			std::cout << AX << std::endl;
		}
		else if (C1 == "BX")
		{
			std::cout << BX << std::endl;
		}
		else if (C1 == "CX")
		{
			std::cout << CX << std::endl;
		}
		else if (C1 == "DX")
		{
			std::cout << DX << std::endl;
		}
		ShowRegisters();
		SaveRegisters();
		Procesor::incCOUNT();
	}

	//DYSK
	if (command == "CF1")//create file 1
	{
		C1 = GetCommand();//folder
		C2 = GetCommand();//nazwa pliku
		SaveRegisters();
		Procesor::incCOUNT();
		std::cout << C1 << std::endl;
		std::cout << C2 << std::endl;
		if (create(C1, C2))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Utworzono plik" << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidlowe argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
	}
	if (command == "CF2")//create file 2
	{
		std::string C3;
		C1 = GetCommand();//folder
		C2 = GetCommand();//nazwa pliku
		C3 = GetCommand();//folder nadrzedny
		SaveRegisters();
		Procesor::incCOUNT();

		std::cout << C1 << std::endl;
		std::cout << C2 << std::endl;
		std::cout << C3 << std::endl;
		if (create(C1, C2, C3))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Utworzono plik" << std::endl;
		}
		else
		{

			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "UN1")//unlink 1
	{
		C1 = GetCommand();//nazwa pliku
		SaveRegisters();
		Procesor::incCOUNT();
		std::cout << C1 << std::endl;
		if (unLink(C1))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Usunieto dowiazanie" << std::endl;
		}
		else
		{

			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這wy argument" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}


	}
	if (command == "UN2")//unlink 2
	{
		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//folder nadrzedny
		SaveRegisters();
		Procesor::incCOUNT();
		std::cout << C1 << std::endl;
		if (unLink(C1, C2))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Usunieto dowiazanie" << std::endl;
		}
		else
		{

			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這wy argument" << white <<std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}


	}
	if (command == "OP1")//unlink 2
	{
		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//typ otwarcia
		SaveRegisters();
		Procesor::incCOUNT();
		int typ = stoi(C2);
		std::cout << C1 << std::endl;
		if (typ == 1)
		{
			if (open(C1, true))
			{
				std::cout << yellow << "[ASSEMBLER]" << white << " Otworzono plik" << std::endl;
			}
			else
			{
				std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
				Procesor::REMOVE(pcb);
				deleteFromMemory(pcb->PID);
				drzewko.kill(pcb->PID);
				Procesor::incCOUNT();
				return;
			}
		}
		else
		{
			if (open(C1, false))
			{
				std::cout << yellow << "[ASSEMBLER]" << white << " Otworzono plik" << std::endl;
			}
			else
			{
				std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
				Procesor::REMOVE(pcb);
				deleteFromMemory(pcb->PID);
				drzewko.kill(pcb->PID);
				Procesor::incCOUNT();
				return;
			}
		}


	}

	if (command == "OP2")//otwarcie 2
	{
		std::string C3;
		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//typ otwarcia
		C3 = GetCommand();//nazwa folderu
		SaveRegisters();
		Procesor::incCOUNT();
		int typ = stoi(C2);
		std::cout << C1 << std::endl;
		if (typ == 1)
		{
			if (open(C1, true, C3))
			{
				std::cout << yellow << "[ASSEMBLER]" << white << " Otworzono plik" << std::endl;
			}
			else
			{
				std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
				Procesor::REMOVE(pcb);
				deleteFromMemory(pcb->PID);
				drzewko.kill(pcb->PID);
				Procesor::incCOUNT();
				return;
			}
		}
		else
		{
			if (open(C1, false, C3))
			{
				std::cout << yellow << "[ASSEMBLER]" << white << " Otworzono plik" << std::endl;
			}
			else
			{
				std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這wy argument" << white << std::endl;
				Procesor::REMOVE(pcb);
				deleteFromMemory(pcb->PID);
				drzewko.kill(pcb->PID);
				Procesor::incCOUNT();
				return;
			}
		}


	}

	if (command == "CL1")//close 1
	{
		C1 = GetCommand();//nazwa pliku
		SaveRegisters();
		Procesor::incCOUNT();
		if (close(C1))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Zamknieto plik" << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這wy argument" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "CL2")//close 2
	{
		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//nazwa folderu
		SaveRegisters();
		Procesor::incCOUNT();
		if (close(C1, C2))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Zamknieto plik" << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "TR1")//truncate 1
	{
		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//rozmiar docelowy
		SaveRegisters();
		Procesor::incCOUNT();
		unsigned short zmienna = stoi(C2);
		if (truncate(C1, zmienna))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Zmieniono rozmiar pliku na " << zmienna << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumeny" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "TR2")//truncate 2
	{
		std::string C3;
		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//rozmiar docelowy
		C3 = GetCommand();//nazwa folderu
		SaveRegisters();
		Procesor::incCOUNT();
		unsigned short zmienna = stoi(C2);
		if (truncate(C1, zmienna, C3))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Zmieniono rozmiar pliku na " << zmienna << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumeny" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "RD1")//Read
	{
		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//rozmiar docelowy
		SaveRegisters();
		Procesor::incCOUNT();
		unsigned short zmienna = stoi(C2);
		char* buffer = new char[1];
		short int zmienna2 = read(C1, buffer, zmienna);
		if (zmienna2 != 0)
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Odczytano " << zmienna2 << " bajtow" << std::endl;
			for (int i = 0; i < zmienna2; i++)
			{
				std::cout << buffer[i];
			}
			std::cout << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nie odczytano" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "RD2")//Read 2
	{
		std::string C3;
		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//rozmiar docelowy
		C3 = GetCommand();//rozmiar docelowy
		SaveRegisters();
		Procesor::incCOUNT();
		unsigned short zmienna = stoi(C2);
		char* buffer = new char[1];
		short int zmienna2 = read(C1, buffer, zmienna, C3);
		if (zmienna2 != 0)
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Odczytano " << zmienna2 << " bajtow" << std::endl;
			for (int i = 0; i < zmienna2; i++)
			{
				std::cout << buffer[i];
			}
			std::cout << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "RD3")//Read 3
	{
		std::string C3;
		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//rozmiar docelowy
		C3 = GetCommand();//rozmiar docelowy
		SaveRegisters();
		Procesor::incCOUNT();
		unsigned short rd3 = stoi(C3);
		unsigned short zmienna = stoi(C2);
		char* buffer = new char[1];
		short int zmienna2 = read(C1, buffer, zmienna, rd3);
		if (zmienna2 != 0)
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Odczytano " << zmienna2 << " bajtow" << std::endl;
			for (int i = 0; i < zmienna2; i++)
			{
				std::cout << buffer[i];
			}
			std::cout << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "RD4")//Read 3
	{
		std::string C3, C4;
		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//rozmiar docelowy
		C3 = GetCommand();//rozmiar docelowy
		C4 = GetCommand();//rozmiar docelowy
		SaveRegisters();
		Procesor::incCOUNT();
		unsigned short rd3 = stoi(C3);
		unsigned short zmienna = stoi(C2);
		char* buffer = new char[1];
		short int zmienna2 = read(C1, buffer, zmienna, rd3, C4);
		if (zmienna2 != 0)
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Odczytano " << zmienna2 << " bajtow" << std::endl;
			for (int i = 0; i < zmienna2; i++)
			{
				std::cout << buffer[i];
			}
			std::cout << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}

	if (command == "WR1")//Write 1
	{
		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//to co mamy zapisac
		SaveRegisters();
		Procesor::incCOUNT();
		std::vector<char> kontener;
		for (int i = 0; i < C2.size(); i++)
		{
			kontener.push_back(C2[i]);
		}
		short int size = write(C1, kontener);
		if (size != 0)
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Zapisano na dysku " << size << " bajtow" << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "WR2")//Write 2
	{
		std::string C3;
		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//to co mamy zapisac
		C3 = GetCommand();//folder nadrzedny
		SaveRegisters();
		Procesor::incCOUNT();
		std::vector<char> kontener;
		for (int i = 0; i < C2.size(); i++)
		{
			kontener.push_back(C2[i]);
		}
		short int size = write(C1, kontener, C3);
		if (size != 0)
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Zapisano na dysku " << size << " bajtow" << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "WR3")//Write 3
	{
		std::string C3;

		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//to co mamy zapisac
		C3 = GetCommand();//miesjce w pliku
		SaveRegisters();
		Procesor::incCOUNT();
		std::vector<char> kontener;
		unsigned short place = stoi(C3);
		for (int i = 0; i < C2.size(); i++)
		{
			kontener.push_back(C2[i]);
		}
		short int size = write(C1, kontener, place);
		if (size != 0)
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Zapisano na dysku " << size << " bajtow" << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "WR4")//Write 4
	{
		std::string C3, C4;

		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//to co mamy zapisac
		C3 = GetCommand();//miesjce w pliku
		C4 = GetCommand();//folder nadrzedny
		SaveRegisters();
		Procesor::incCOUNT();
		std::vector<char> kontener;
		unsigned short place = stoi(C3);
		for (int i = 0; i < C2.size(); i++)
		{
			kontener.push_back(C2[i]);
		}
		short int size = write(C1, kontener, place, C4);
		if (size != 0)
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Zapisano na dysku " << size << " bajtow" << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "LS1")//lseek
	{
		std::string C3;
		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//to co mamy zapisac
		C3 = GetCommand();//miesjce w pliku
		SaveRegisters();
		Procesor::incCOUNT();
		short int c2 = stoi(C2);
		unsigned short int c3 = stoi(C3);
		short int pozycja = lseek(C1, c2, c3);
		if (pozycja != -1)
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Zmieniiono pozycje wskaznika w pliku na " << pozycja << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
		


	}
	if (command == "LS2")//lseek2
	{
		std::string C3, C4;
		C1 = GetCommand();//nazwa pliku
		C2 = GetCommand();//to co mamy zapisac
		C3 = GetCommand();//miesjce w pliku
		C4 = GetCommand();//folder nadrzedny
		SaveRegisters();
		Procesor::incCOUNT();
		short int c2 = stoi(C2);
		unsigned short int c3 = stoi(C3);
		short int pozycja = lseek(C1, c2, c3, C4);
		if (pozycja != -1)
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Zmieniiono pozycje wskaznika w pliku na " << pozycja << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}

	if (command == "FC1")//folder create
	{
		C1 = GetCommand();//nazwa folderu
		C2 = GetCommand();//nazwa folderu w ktorym chcemy stworzyc
		SaveRegisters();
		Procesor::incCOUNT();
		if (fcreate(C1, C2))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Utworzono folder " << C2 << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "FC2")//folder create2
	{
		std::string C3;
		C1 = GetCommand();//nazwa folderu
		C2 = GetCommand();//nazwa folderu w ktorym chcemy stworzyc
		C3 = GetCommand();//nazwa folderu nadrzednego
		SaveRegisters();
		Procesor::incCOUNT();
		if (fcreate(C1, C2, C3))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Utworzono folder " << C2 << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "FD1")//delete folder 1
	{
		C1 = GetCommand();
		SaveRegisters();
		Procesor::incCOUNT();
		if (fdelete(C1))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Usunieto folder " << C1 << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "FD2")//delete folder 1
	{
		C1 = GetCommand();
		C2 = GetCommand();
		SaveRegisters();
		Procesor::incCOUNT();
		if (fdelete(C1, C2))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Usunieto folder " << C1 << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "FEC1")//utworzonie wpisu katalogowego 1
	{
		std::string C3;
		C1 = GetCommand();//nazwa folderu w ktorym sie znajduje
		C2 = GetCommand();//nazwa wpisu ktory oddajemy
		C3 = GetCommand();//nazwa pliku do ktorego dopisujemy
		SaveRegisters();
		Procesor::incCOUNT();
		if (fecreate(C1, C2, fsearch(C3, 0)))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Utworzono wpis katalogowy " << C2 << " w foldeerze " << C1 << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "FEC2")//utworzonie wpisu katalogowego 2
	{
		std::string C3, C4;
		C1 = GetCommand();//nazwa folderu w ktorym sie znajduje
		C2 = GetCommand();//nazwa wpisu ktory oddajemy
		C3 = GetCommand();//nazwa pliku do ktorego dopisujemy
		C4 = GetCommand();//folder nadrzedny
		SaveRegisters();
		Procesor::incCOUNT();
		if (fecreate(C1, C2, fsearch(C3, 0, C4), C4))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Utworzono wpis katalogowy " << C2 << " w foldeerze " << C1 << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER] " << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "FED1")//usuniecie wpisu
	{
		C1 = GetCommand();//nazwa wpisu
		SaveRegisters();
		Procesor::incCOUNT();
		if (fedelete(C1))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Usunieto wpis " << C1 << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "FED2")//usuniecie wpisu 2
	{
		C1 = GetCommand();//nazwa wpisu
		C2 = GetCommand();//folder nadrzedny
		SaveRegisters();
		Procesor::incCOUNT();
		if (fedelete(C1, C2))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Usunieto wpis " << C1 << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "FR1")//zmiana nazw
	{

		C1 = GetCommand();//oryginalna nazwa
		C2 = GetCommand();//nowa nazwa
		SaveRegisters();
		Procesor::incCOUNT();
		if (frename(C1, C2))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Zmieniono nazwe " << C1 << " na " << C2 << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}

	}
	if (command == "FR2")//zmiana nazw 2
	{

		std::string C3;
		C1 = GetCommand();//oryginalna nazwa
		C2 = GetCommand();//nowa nazwa
		C3 = GetCommand();//nazwa nadrzednego
		SaveRegisters();
		Procesor::incCOUNT();
		if (frename(C1, C2, C3))
		{
			std::cout << yellow << "[ASSEMBLER]" << white << " Zmieniono nazwe " << C1 << " na " << C2 << std::endl;
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Nieprawidl這we argumenty" << white << std::endl;
			Procesor::REMOVE(pcb);
			deleteFromMemory(pcb->PID);
			drzewko.kill(pcb->PID);
			Procesor::incCOUNT();
			return;
		}
	}
	if (command == "FCO1")//wypisanie wpisow katalogowych
	{
		C1 = GetCommand();//oryginalna nazwa folderu
		SaveRegisters();
		Procesor::incCOUNT();
		std::vector<FEntry> entries = fcontent(C1);
		std::cout << yellow << "[ASSEMBLER]" << white << " " << std::endl;
		for (int i = 0; i < entries.size(); i++)
		{
			std::cout << entries[i];
		}
	}
	if (command == "FCO2")//wypisanie wpisow katalogowych 2
	{
		C1 = GetCommand();//oryginalna nazwa folderu
		C2 = GetCommand();//folder nadrzedny
		SaveRegisters();
		Procesor::incCOUNT();
		std::vector<FEntry> entries = fcontent(C1, C2);
		std::cout << yellow << "[ASSEMBLER]" << white << " " << std::endl;
		for (int i = 0; i < entries.size(); i++)
		{
			std::cout << entries[i];
		}
	}
	if (command == "DFC")//rysowanie zawartosci folderu
	{
		C1 = GetCommand();// nazwa folderu
		SaveRegisters();
		Procesor::incCOUNT();
		drawFolderContents(C1);
	}
	if (command == "DB")//wyswietlanie zawartosci bloku dyskowego
	{
		C1 = GetCommand();//numer bloku dyskowego
		SaveRegisters();
		Procesor::incCOUNT();
		unsigned short int zmienna = stoi(C1);
		drawBlockD(zmienna);

	}
	if (command == "DD")//draw disck
	{
		std::cout << std::endl;
		drawDiscD();
		SaveRegisters();
		Procesor::incCOUNT();

	}
	if (command == "DI")//draw i node
	{
		C1 = GetCommand();//numer bloku dyskowego
		unsigned short int zmienna = stoi(C1);
		drawInode(zmienna);
		SaveRegisters();
		Procesor::incCOUNT();
	}
	/////PROCESY
	if (command == "CP")//tworzenie procesu
	{
		C1 = GetCommand();//nazwa procesu
		std::cout << C1 << std::endl;
		if (drzewko.fork(pcb, C1))
		{
			SaveRegisters();
		}
		else
		{
			PCB* temp = Procesor::getRunning();
			Procesor::REMOVE(temp);
			deleteFromMemory(temp->PID);
			drzewko.kill(temp->PID);
			Procesor::incCOUNT();
			return;
		}
		Procesor::incCOUNT();	
	}

	//POTOKI
	if (command == "CPL")// create pipe line
	{
		C1 = GetCommand();
		C2 = GetCommand();
		std::cout << C1 << " "<<C2<<std::endl;
		PCB *temp = drzewko.findpcb(C1);
		PCB *temp2 = drzewko.findpcb(C2);
		Pipeline::createPipe(temp, temp2);
		SaveRegisters();
		Procesor::incCOUNT();
	}
	if (command == "DPL")//delete pipeline
	{
		C1 = GetCommand();
		std::cout << C1 << std::endl;
		PCB *temp = drzewko.findpcb(C1);
		Pipeline::deletePipe(temp);
		SaveRegisters();
		Procesor::incCOUNT();
	}
	if (command == "RPIP")//czytanie z bufora przy pipeach
	{
		PCB* temp1 = Procesor::getRunning();
		std::string C3;
		C1 = GetCommand();
		C2 = GetCommand();
		C3 = GetCommand();
		std::cout << C1 << " " << C2 <<" "<<C3<< std::endl;
		PCB *temp = drzewko.findpcb(C1);
		PCB *temp2 = drzewko.findpcb(C2);
		unsigned int temp3 = stoi(C3);
		Pipeline::GetPipe(temp->PID, temp2->PID)->read(temp3);
		if (temp1->state != WAITING)
		{
			SaveRegisters();
		}
		Procesor::incCOUNT();
	}

	if (command == "WPIP")//czytanie z bufora przy pipeach
	{
		PCB* temp1 = Procesor::getRunning();
		std::string C3;
		C1 = GetCommand();
		C2 = GetCommand();
		C3 = GetCommand();
		std::cout << C1 << " " << C2 << " " << C3 << std::endl;
		PCB *temp = drzewko.findpcb(C1);
		PCB *temp2 = drzewko.findpcb(C2);
		Pipeline::GetPipe(temp->PID, temp2->PID)->write(C3);
		if (temp1->state != WAITING)
		{
			SaveRegisters();
		}
		Procesor::incCOUNT();
	}

	// Zamki <- OPCJONALNE
	
	if (command == "LL")
	{
		std::cout << std::endl;
		PCB* temp = Procesor::getRunning();
		SaveRegisters();
		lock_l(zamek, temp);
		Procesor::incCOUNT();
	}

	if (command == "LU")
	{
		std::cout << std::endl;
		lock_u(zamek);
		SaveRegisters();
		Procesor::incCOUNT();
	}
	
	//KONIEC PROGRAMU
	if (command[0] == 'R'&&command[1] == 'S'&& command[2] == 'T')
	{
		std::cout << std::endl;
		PCB* temp = Procesor::getRunning();
		Procesor::REMOVE(temp);
		deleteFromMemory(temp->PID);
		drzewko.kill(temp->PID);
		Procesor::incCOUNT();
	}

	//ZABIJANIE PROCESU
	if (command[0] == 'K'&&command[1] == 'P')
	{
		C1 = GetCommand();
		if (drzewko.findpcb(C1)->PID != Procesor::getRunning()->PID)
		{
			PCB* temp = drzewko.findpcb(C1);
			deleteFromMemory(temp->PID);
			if(temp->state != NEW)
				Procesor::REMOVE(drzewko.findpcb(C1));
			drzewko.kill(temp->PID);
			std::cout << C1 << std::endl;
			std::cout << yellow << "[ASSEMBLER]" << white << " Zabito proces " << C1 << std::endl;
			SaveRegisters();
		
		}
		else
		{
			std::cout << yellow << "[ASSEMBLER]" << red << " Proces nie moze usunac siebie samego" << white << std::endl;
			PCB* temp = Procesor::getRunning();
			Procesor::REMOVE(temp);
			deleteFromMemory(temp->PID);
			drzewko.kill(temp->PID);
			Procesor::incCOUNT();
			return;
		}
		Procesor::incCOUNT();
	}
}





