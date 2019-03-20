#include "pipes.h"

static lock *mutex = new lock();
std::vector<Pipe*> pipes;

//znaczenia desktrypor�w:
// -1 - proces nie chce dzia�a� ju� na tym potoku
// 1 - proces ma prawo zapisywa�
// 0 - proces ma prawo odczytywa�
//sam dysktyprot jest umieszczony w PCB i przechowuje inkeds potoku w tablicy potok�w oraz prawid�owy Descriptor
//pdesk -> pipe deskryptor

Pipe::Pipe(PCB * pi1, PCB* p2)
{
	this->pi1 = pi1;
	this->p2 = p2;
}

Pipe::~Pipe()
{
	pi1->pdesk[0] = -1;
	p2->pdesk[0] = -1;
}

cond c_empty(1);
cond c_full(0);

std::string Pipe::read(unsigned int l)
{
	lock_l(mutex, Procesor::getRunning());
	std::string temp;
	if (p2->pdesk[1] != 0)	//sprawdzanie czy dany proces ma prawa do tej funkcji
		std::cout << darkgreen << "[PIPE] "<< white << "Error pipe - proces nie ma prawa do odczytu" << std::endl;
	else if (p2->pdesk[1] == 0)
	{
		if (c_empty.get_condition() == buffer.empty())
		{
			cond_w(&c_empty, mutex, Procesor::getRunning());//blokowanie procesu w funkcji read
			//jesli bufor pusty
		}
		else
		{
			//w przeciwnym wypadku
			if (l > buffer.size())	//sprawdznie czy wiadomo�c kt�ra chcemy odczyta� jest d�u�sza ni� wiadomo�c w potoku
			{
				while (buffer.size() > 0)
				{
					temp.push_back(buffer.front());
					buffer.pop_front();
				}
				Pipeline::deletePipe((pi1));
			}
			else
			{
				for (int i = 0; i < l; i++)
				{
					temp.push_back(buffer.front());
					buffer.pop_front();
				}
			}
			//wybudzenie czekajacych na zapis!!!!
			cond_b(&c_full);
		}
	}
	lock_u(mutex);
	if (temp.size() != 0)
		std::cout << darkgreen << "[PIPE] " << white << "Odczytano z potoku: " << temp << std::endl;
	return temp;
}

void Pipe::write(const std::string& msg)
{
	lock_l(mutex, Procesor::getRunning());
	if (pi1->pdesk[1] != 1)	//sprawdzanie czy dany proces ma prawa do tej funkcji
		std::cout << darkgreen << "[PIPE] " << white << "Error pipe - Proces nie ma prawa do zapisu" << std::endl;
	else if (pi1->pdesk[1] == 1)
	{
		if (c_full.get_condition() != (buffer.size() == max_buf_size))//buffor jest pelny, nie mozna pisac do niego
		{
			cond_w(&c_full, mutex, Procesor::getRunning());
		}
		else
		{
			if (msg.size() <= max_buf_size - buffer.size())//wiadomosc zmiesci sie cala do buffora, zapisujemy cala
			{
				for (auto x : msg)	//zapisywanie wiadomo�ci do potoku
					buffer.push_back(x);
			}
			else//cala sie nie zmiesci, zapisujemy tyle ile sie zmiesci
			{
				int i = 0;
				while (max_buf_size - buffer.size() != 0)
				{
					buffer.push_back(msg[i]);
					i++;
				}

			}
		}
		cond_b(&c_empty);//wybudzanie procesu oczekujacego na odczyt!!!
	}
	std::cout << darkgreen << "[PIPE] " << white << "Zapisano do potoku: " << msg << std::endl;
	lock_u(mutex);
}

Pipeline::Pipeline() {}

void Pipeline::createPipe(PCB* pi1, PCB* p2)	//tworzenie potoku
{
	bool check = false;
	int i;
	for (i = 0; i < pipes.size(); i++)	//szukanie pustego miejsca w tablicy potok�w
	{
		if (pipes[i] == NULL)
		{
			check = true;
			break;
		}
	}
	if (check)	//dodawanie potoku w puste miejsce
	{
		Pipe* temp = new Pipe(pi1, p2);
		pipes[i] = temp;
		pi1->pdesk[0] = i;
		pi1->pdesk[1] = 1;
		p2->pdesk[0] = i;
		p2->pdesk[1] = 0;
	}
	else	//dodawanie potoku na ko�cu tablicy
	{
		Pipe* temp = new Pipe(pi1, p2);
		pipes.push_back(temp);
		pi1->pdesk[0] = pipes.size() - 1;
		pi1->pdesk[1] = 1;
		p2->pdesk[0] = pipes.size() - 1;
		p2->pdesk[1] = 0;
	}
	std::cout << darkgreen << "[PIPE] " << white << "Utworzono potok" << std::endl;
}

void Pipeline::deletePipe(PCB* pi1)	//usuwanie potoku
{
	if (pi1->pdesk[0] != -1)
	{
		int temp = pi1->pdesk[0];
		delete(pipes[temp]);
		pipes[temp] = NULL;
	}
	else
	{
		std::cout << darkgreen << "[PIPE] " << white << "Nie mozna usunac nieistniejacego pipe" << std::endl;
	}
	std::cout << darkgreen << "[PIPE] " << white << "Usunieto" << std::endl;
}

bool Pipeline::Pipe_exist(PCB * pi1, PCB * p2)
{
	if (pi1->pdesk[0] == p2->pdesk[0] && pi1->pdesk[1] == 1 && pi1->pdesk[0] != -1)
	{
		return true;
	}
	else
	{
		createPipe(pi1, p2);
		return true;
	}
}

Pipe* Pipeline::GetPipe(int PID_1, int PID_2) {
	for (int i = 0; i < pipes.size(); i++) {
		if (pipes[i]->p2->PID == PID_1 || pipes[i]->pi1->PID == PID_1) {
			if (pipes[i]->p2->PID == PID_2 || pipes[i]->pi1->PID == PID_2) {
				return pipes[i];
			}
		}
	}
}
