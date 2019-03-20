#pragma once
#include "cond.h"

extern PCB *p1;
extern std::vector<PCB*> QRP;
extern int licznik;

static bool endwhile;
static int iteration;
static bool hereIam;

class Procesor
{
private:

	int TOTAL_WT;
	int ALL_PROCESSES = 0;

	static void RMV(PCB* to_remove, int ind)
	{
		QRP.erase(QRP.begin() + ind);
	}

public:

	static void SetRunning()//a b c d
	{

		hereIam = 1;
		if (QRP.size() == 0)
		{
			QRP.push_back(p1);
			std::cout << cyan << "[RR] " << white << "RUNNING = DUMMY" << std::endl;
		}
		else
		{
			iteration = 0;
			endwhile = 1;
		
			while (endwhile)
			{
				if (QRP[0]->state == DUMMY && QRP.size() != 1)
				{
					QRP.erase(QRP.begin());
					licznik = 0;
				}
				if (RUNNING == QRP[iteration]->state)
				{
					if ((licznik < 1))
					{
						QRP[iteration]->set_state(READY);
						iteration++;
						if (QRP.size() < iteration + 1) iteration = 0;
						if (iteration == 0 || (iteration + 1 % 10 == 0 && iteration + 1 != 10))
							std::cout << cyan << "[RR] " << white << "RUNNING = " << iteration + 1 << "st PROCESS" << std::endl;
						else if (iteration == 1 || (iteration + 1 % 10 == 1 && iteration + 1 != 11))
							std::cout << cyan << "[RR] " << white << "RUNNING = " << iteration + 1 << "nd PROCESS" << std::endl;
						else if (iteration == 2 || (iteration + 1 % 10 == 2 && iteration + 1 != 12))
							std::cout << cyan << "[RR] " << white << "RUNNING = " << iteration + 1 << "rd PROCESS" << std::endl;
						else std::cout << cyan << "[RR] " << white << "RUNNING = " << iteration + 1 << "th PROCESS" << std::endl;

						QRP[iteration]->set_state(RUNNING);
						endwhile = 0;
					}
					else
					{
						std::cout << cyan << "[RR] " << white << "RUNNING = STILL THE SAME" << std::endl;
						endwhile = 0;
					}
				}
				for (int j = 0; j < QRP.size(); j++)
				{
					if (QRP[j]->state == RUNNING || QRP[0]->state == DUMMY)
					{
						hereIam = 0;
					}
				}
				if (hereIam)
				{
					iteration = 0;
					std::cout << cyan << "[RR] " << white << "RUNNING = 1st PROCESS" << std::endl;
					QRP[iteration]->set_state(RUNNING);
					endwhile = 0;
				}
				iteration++;
				if (QRP[0]->state == DUMMY && QRP.size() == 1)
				{
					std::cout << cyan << "[RR] " << white << "RUNNING = DUMMY" << std::endl;
					endwhile = 0;
				}
			}
		}
	}

	static void REMOVE(PCB* to_remove)// a b c d
	{
		if (to_remove->state == RUNNING)
			licznik = -1;
		for (int i = 0; i < QRP.size(); i++)
		{
			if (to_remove == QRP[i])
				iteration = i;
		}
		RMV(to_remove, iteration);
	}

	static void incCOUNT()//przy wykonywaniu rozkazow przez interpreter, musi byc wywolywana ta funkcja
	{
		licznik++;
		if (licznik > 4)
		{
			licznik = 0;
		}

		if (QRP.size() != 0)
		{
			if (QRP[0]->state == DUMMY) licznik = 0;
		}
		SetRunning();
	}

	static PCB* getRunning()
	{
		for (int i = 0; i < QRP.size(); i++)
		{
			if (QRP[0]->state == DUMMY && QRP.size() == 1)
				return QRP[0];
			else if (QRP[0]->state == DUMMY)
				SetRunning();

			if (RUNNING == QRP[i]->state)
				return QRP[i];
		}
		for (int i = 0; i < QRP.size(); i++)
		{
			if (QRP[i]->state == WAITING) return QRP[i];
		}
	}
};