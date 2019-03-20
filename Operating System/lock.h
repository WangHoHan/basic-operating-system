#pragma once
#include "Procesy.h"
#include "ConsoleColor.h"

class lock
{
protected:

	bool is_open;
	PCB* who_closed;
	std::queue<PCB*> lock_queue;

public:

	lock();

	bool get_is_open();

	PCB* get_who_closed();

	int get_queue_size();

	void set_is_open(bool b);

	void set_who_closed(PCB* process);

	bool add_to_lock_queue(PCB *process);

	PCB* remove_from_lock_queue();

};

void lock_l(lock *l, PCB *process);

void lock_u(lock *l);