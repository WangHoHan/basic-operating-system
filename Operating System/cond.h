#pragma once
#include "lock.h"
static lock *zamek = new lock();
class cond
{
protected:
	std::queue<PCB*>cond_queue;
	bool condition;
public:

	cond();

	cond(bool condition);

	int get_queue_size();

	bool get_condition();

	bool add_to_cond_queue(PCB *process);

	PCB* remove_from_cond_queue();
};


void cond_w(cond *c, lock *l, PCB *process);

void cond_b(cond *c);