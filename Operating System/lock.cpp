#include "lock.h"
//#include "stdafx.h"
// klasa zamek
lock::lock()
{
	this->is_open = true;
	this->who_closed;
	this->lock_queue;
}

// informacja o stanie zamka
bool lock::get_is_open()
{
	return is_open;
}

// informacja kto zaj¹³ zamek
PCB* lock::get_who_closed()
{
	return who_closed;
}

// informacja o rozmiarze kolejki
int lock::get_queue_size()
{
	return this->lock_queue.size();
}

// ustawianie stanu zamka
void lock::set_is_open(bool b)
{
	this->is_open = b;
}

// ustawianie pid procesu zajmuj¹cego zamek
void lock::set_who_closed(PCB* process)
{
	if (!this->is_open)
	{
		this->who_closed = process;
		std::cout << red << "[Sync] " << white << "Proces o pid " << process->PID << " zamknal zamek.\n";
	}
	else std::cout << red << "[Sync] " << white << "Zamek otwarty";
}

// dodawanie pid procesu na koniec kolejki
bool lock::add_to_lock_queue(PCB *process)
{
	if (!this->is_open)
	{
		this->lock_queue.push(process);
		return 1;
	}
	else return 0;
}

// usuwanie pid procesu z pocz¹tku kolejki
PCB* lock::remove_from_lock_queue()
{
	PCB* process = this->lock_queue.front();
	this->lock_queue.pop();
	return process;
}

// zamykanie zamka
void lock_l(lock *l, PCB *process)
{
	if (process->state == RUNNING)
	{
		if (l->get_is_open())
		{
			l->set_is_open(false);
			l->set_who_closed(process);
			// proces wykonuje sekcjê krytyczn¹.
		}

		else
		{
			if (l->get_who_closed() != process)
			{
				if (l->add_to_lock_queue(process))
				{
					process->set_state(WAITING);
				}
				std::cout << red << "[Sync] " << white << "Zamek zamkniety. Proces laduje w kolejce.\n";
			}
			// proces zmienia stan na waiting.
			else
			{
				std::cout << red << "[Sync] " << white << "Zamek jest juz zajety przez ten proces.\n";
			}
		}
	}
	else
		std::cout << red << "[Sync] " << white << "Tylko proces w stanie RUNNING moze zamknac zamek.\n";

}

// otwieranie zamka
void lock_u(lock* l)
{
	if (l->get_is_open())
	{
		std::cout << red << "[Sync] " << white << "Zamek jest juz otwarty.\n";
	}
	else
	{
		if (l->get_queue_size())
		{
			PCB *process;
			process = l->remove_from_lock_queue();
			process->set_state(READY);
			if (l->get_queue_size() == 1)
				std::cout << red << "[Sync] " << white << "Nastepny proces: " << process->PID << ". W kolejce oczekuje jeszcze 1 proces.\n";
			else
				std::cout << red << "[Sync] " << white << "Nastepny proces: " << process->PID << ". W kolejce oczekuje jeszcze " << l->get_queue_size() << " procesow.\n";
		}
		else
		{
			l->set_is_open(true);
			std::cout << red << "[Sync] " << white << "Brak procesow w kolejce. Otwieranie zamka.\n";
		}
	}
}