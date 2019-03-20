#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>



/*
extern class bla bla bla


*/
#include "Memory.h"
/*#include "PCB"
#include "PCB_tree"
#include "HardDrive"

#include "Pipelines"
#include "ProcessManager"
*/





	 int AX;
	 int BX;
	 int CX;
	 int DX;
	//static int FX;
	static int CommandCounter;
	static int PID;
	//ewentualnie potrzebna zmienna do zamk�w

	 void SaveRegisters(); //zapisanie rejetr�w po wykonanym rozkazie
	 void ShowRegisters();  //wy�wietlenie warto��i rejestr�w
	 void GetRegisters(); //pobranie rejestr�w z RUNNING procesu
	 std::string GetCommand(); //pobranie rozkazu z pami�ci RAM

	 void SaveRegisters(PCB &pcb)
	{
		/*
	PCB &pcb = Processor::getRunning();*/
	pcb.counter = CommandCounter;
	pcb.registers[0] = AX;
	pcb.registers[1] = BX;
	pcb.registers[2] = CX;
	pcb.registers[3] = DX;
	pcb.PID = PID;
	}

	 void ShowRegisters()
	{
		std::cout << "Process PID: " << PID << std::endl;
		std::cout << "AX: " << AX << std::endl;
		std::cout << "BX: " << BX << std::endl;
		std::cout << "CX: " << CX << std::endl;
		std::cout << "DX: " << DX << std::endl;
		std::cout << "CommandCounter: " << CommandCounter << std::endl;
	}

	 void GetRegisters(PCB &pcb)
	{

		//PCB &pcb = Processor::getRunning();
		AX = pcb.registers[0];
		BX = pcb.registers[1];
		CX = pcb.registers[2];
		DX = pcb.registers[3];
		PID = pcb.PID;
		CommandCounter = pcb.counter;
		
	}

	 std::string GetCommand(PCB &pcb)
	{
		
	//PCB &pcb = Processor::getRunning();
	std::string command;
	command = getOrder(pcb, CommandCounter);//tomek w definicji musis miec &
	CommandCounter += command.size() + 1;//zeby wiedziec spacje
	return command;
	}




	//static void Execute(PCB &pcb);

	//wykonanie rozkazu, argumenty funckcji do dopisania
	static void Execute(PCB &pcb)
	{
		//PCB &pcb = Processor::getRunning(); to jest chyba napewno niepoitrzebne,ale jendak moze do writa i reada sie przyda
		std::string C1, C2;
		int temporary = 0;
		GetRegisters(pcb);
		std::string command;
		//command.clear(); zobaczymy czy to jest potrzebne
		command = GetCommand(pcb);
		std::cout << "Command is: " << command << " ";

		//DODAC ZABEZPIECZENIE PRZED ZEREM I INNE PIERDOLY
		//checking operation
		if (command == "AD")
		{
			C1 = GetCommand(pcb);
			C2 = GetCommand(pcb);
			std::cout << C1 << " " << C2 << std::endl;
			if (C2 != "AX" && C2 != "BX" && C2 != "CX" && C2 != "DX")
			{
				temporary = stoi(C2);
			}

			//C1==AX

			if (C1 == "AX" && temporary != 0)
			{
				AX += temporary;
			}
			else if (C1 == "AX" && C2 == "BX")
			{
				AX += BX;
			}
			else if (C1 == "AX" && C2 == "CX")
			{
				AX += BX;
			}
			else if (C1 == "AX" && C2 == "AX")
			{
				AX += AX;
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

			ShowRegisters();
			SaveRegisters(pcb);
			//Processor::incCOUNT();
		}

		//odejmowanie 
		if (command == "SB")
		{
			C1 = GetCommand(pcb);
			C2 = GetCommand(pcb);
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
				AX -= BX;
			}
			else if (C1 == "AX" && C2 == "AX")
			{
				AX -= AX;
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

			ShowRegisters();
			SaveRegisters(pcb);
			//Processor::incCOUNT();
		}

		//DZIELENIE
		if (command == "DV")
		{
			C1 = GetCommand(pcb);
			C2 = GetCommand(pcb);
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
				AX /= BX;
			}
			else if (C1 == "AX" && C2 == "AX")
			{
				AX /= AX;
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
			ShowRegisters();
			SaveRegisters(pcb);
			//Processor::incCOUNT();
		}


		//MNOZENIE
		if (command == "ML")
		{
			C1 = GetCommand(pcb);
			C2 = GetCommand(pcb);
			std::cout << C1 << " " << C2 << std::endl;
			if (C2 != "AX" && C2 != "BX" && C2 != "CX" && C2 != "DX")
			{
				temporary = stoi(C2);
			}

			//C1==AX

			if (C1 == "AX" && temporary != 0)
			{
				AX *= temporary;
			}
			else if (C1 == "AX" && C2 == "BX")
			{
				AX *= BX;
			}
			else if (C1 == "AX" && C2 == "CX")
			{
				AX *= BX;
			}
			else if (C1 == "AX" && C2 == "AX")
			{
				AX *= AX;
			}


			// C1=BX
			if (C1 == "BX" && temporary != 0)
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


			//C1=CX
			if (C1 == "CX" && temporary != 0)
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

			ShowRegisters();
			SaveRegisters(pcb);
			//Processor::incCOUNT();
		}
		if (command == "CO")
		{
			int x, y;
			C1 = GetCommand(pcb);
			C2 = GetCommand(pcb);
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
			else
			{
				y = stoi(C2);
			}

			DX = (x - y) % 2;

		}


		//INKREMENTACJA I DEKREMENTACJA REJETRU
		if (command == "DC")
		{
			C1 = GetCommand(pcb);
			std::cout << C1 << std::endl;
			if (C1 == "AX")
				AX--;
			if (C1 == "BX")
				BX--;
			if (C1 == "CX")
				CX--;


			ShowRegisters();
			SaveRegisters(pcb);
			//Processor::incCOUNT();
		}
		if (command == "IN")
		{
			C1 = GetCommand(pcb);
			std::cout << C1 << std::endl;
			if (C1 == "AX")
				AX++;
			if (C1 == "BX")
				BX++;
			if (C1 == "CX")
				CX++;


			ShowRegisters();
			SaveRegisters(pcb);
			//Processor::incCOUNT();
		}

		//ZAPIS DO REJESTRU
		if (command == "MO")
		{

			C1 = GetCommand(pcb);
			C2 = GetCommand(pcb);
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
			SaveRegisters(pcb);
			//Processor::incCOUNT();
		}

		////////////// POBRANE WARTOSCI Z RAMU
		if (command == "MV")
		{

			C1 = GetCommand(pcb);
			C2 = GetCommand(pcb);
			std::cout << C1 << " " << C2 << std::endl;
			temporary = stoi(C2);

			if (C1 == "AX");
			{
				//AX = stoi(RAM::Get(&pcb, temporary));
			}
			if (C1 == "BX")
			{
				//BX = stoi(RAM::Get(&pcb, temporary));
			}
			if (C1 == "CX")
			{
				//CX = stoi(RAM::Get(&pcb, temporary));
			}
			/*if (C1 == "DX")
				//DX = stoi(RAM::Get(&pcb, temporary));*/

			ShowRegisters();
			SaveRegisters(pcb);
			//Processor::incCOUNT();

		}

		//ZAPIS WARTOSCI DO RAMU//SPRAWDZIC TO Z TOMKIEM
		if (command == "MR")
		{

			C1 = GetCommand(pcb); //gdzie 
			C2 = GetCommand(pcb); //co
			std::cout << C1 << " " << C2 << std::endl;
			temporary = stoi(C1);

			if (C2 == "AX");
			{
				//	C1 = std::to_string(AX);
				//RAM::Write(&pcb, temporary, C1 );
			}
			if (C2 == "BX")
			{
				//	C1 = std::to_string(BX);
				//RAM::Write(&pcb, temporary, C1 );
			}
			if (C2 == "CX")
			{
				//	C1 = std::to_string(CX);
				//RAM::Write(&pcb, temporary, C1 );
			}
			if (C2 == "DX")
			{//	C1 = std::to_string(DX);
				//RAM::Write(&pcb, temporary, C1 );
			}
			//ShowRegisters();
			//SaveRegisters();
			//Processor::incCOUNT();

		}

		////////SKOKI
		if (command == "JP")
		{

			C1 = GetCommand(pcb);
			std::cout << C1 << std::endl;
			CommandCounter = stoi(C1);

			ShowRegisters();
			SaveRegisters(pcb);
			//Processor::incCOUNT();

		}

		//SKOK POD WARUNKIEM

		if (command == "JZ")//skacze jak jest 0
		{
			C1 = GetCommand(pcb);
			int skok = stoi(C1);
			if (DX == 0)
			{

				CommandCounter = skok;
			}


			ShowRegisters();
			SaveRegisters(pcb);
			//Processor::incCOUNT();
		}
		if (command == "JN")//skacze jak nie jest 0
		{
			C1 = GetCommand(pcb);
			int skok = stoi(C1);
			if (DX != 0)
			{

				CommandCounter = skok;
			}


			ShowRegisters();
			SaveRegisters(pcb);
			//Processor::incCOUNT();
		}



		if (command == "OT")
		{
			C1 = GetCommand(pcb);
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
		}


		// PROCESY
		if (command == "CP")
		{
			C1 = GetCommand(pcb);
			C2 = GetCommand(pcb);
			//ProccesManager::createprocess(C1, C2);
			ShowRegisters();
			SaveRegisters(pcb);
			//Processor::incCOUNT();
		}

		if (command == "SM")
		{
			C1 = GetCommand(pcb);
			C2 = GetCommand(pcb);
			//Pipilines::sendMessage(C1, C2);
		}

	}









