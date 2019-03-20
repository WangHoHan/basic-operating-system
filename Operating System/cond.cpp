#include "cond.h"

// klasa zmienna warunkowa
cond::cond()
{
	std::queue<PCB> cond_queue;
}

cond::cond(bool condition)
{
	std::queue<PCB> cond_queue;
	this->condition = condition;
}

// informacja o rozmiarze kolejki
int cond::get_queue_size()
{
	return this->cond_queue.size();
}

// informacja o warunku
bool cond::get_condition()
{
	return this->condition;
		 
}

// dodawanie pid procesu na koniec kolejki
bool cond::add_to_cond_queue(PCB *process)
{
	this->cond_queue.push(process);
	return 1;
}

PCB *cond::remove_from_cond_queue()
{
	if (this->cond_queue.size())
	{
		PCB *process = this->cond_queue.front();
		this->cond_queue.pop();
		return process;
	}
	else return &PCB(); //// coœ nie tak ////
}

//uœpienie procesu
void cond_w(cond *c, lock *l, PCB *process)
{
	std::cout << red << "[Sync] " << white << "Uspienie watku.\n";
	if (c->add_to_cond_queue(process))
	{
		process->set_state(WAITING);
		lock_u(l);
	}
}

//wybudzanie wszystkiech procesów
void cond_b(cond *c)
{
	if (c->get_queue_size())
	{
		PCB *process;
		std::vector<PCB*> vec;
		if (c->get_queue_size() == 1)
			std::cout << red << "[Sync] " << white << "Wybudzam 1 proces.\n";
		else
			std::cout << red << "[Sync] " << white << "Wybudzam " << c->get_queue_size() << " procesow.\n";

		while (c->get_queue_size())
		{
			process = c->remove_from_cond_queue();
			vec.push_back(process);
			process->set_state(READY);
		}
	}
	else
	{
		std::cout << red << "[Sync] " << white << "Brak procesow do wybudzenia.\n";
	}
}