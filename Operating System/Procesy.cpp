
#include"Procesy.h"
int free_pid = 2;
extern int licznik;
int depth = 0;

extern class Procesor;
std::vector<PCB*> QRP;
PCB_tree drzewko;

void PCB::set_state(State a)
{
	switch (a) {
	case NEW: // proces bez przydzielonego programu
	{
		this->state = a; 
		break;
	}
	case READY: //
	{

		if (this->state == WAITING) 
		{
			QRP.push_back(this);
		}
		this->state = a;
		break;
	}
	case WAITING: // proces postrzymany przed dzia³aniem
	{
		this->state = a;
		for (int i = 0; i < QRP.size(); i++)
			if (QRP[i]->state == WAITING)
			{
				QRP.erase(QRP.begin() + i);
				licznik = -1;
			}
		break;
	}
	case RUNNING: // proces wykonuj¹cy program
	{
		this->state = a;
		break;
	}
	}
}

// wyœwietlanie jednego procesu
void PCB::display()
{
	for (int i = 0; i < depth; i++)
	{
		std::cout << char(205);
	}
	std::cout << char(187) << " " << "PID: " << PID << std::endl;
	for (int i = 0; i < depth; i++)
	{
		std::cout << char(205);
	}
	std::cout << char(185) << " " << "Nazwa procesu: " << p_name << std::endl;

	if (file_name.size())
	{
		for (int i = 0; i < depth; i++)
		{
			std::cout << char(205);
		}
		std::cout << char(185) << " " << "Nazwa otwartego pliku: " << file_name << std::endl;
	}
	for (int i = 0; i < depth; i++)
	{
		std::cout << char(205);
	}
	std::cout << char(185) << " " << "Stan procesu: " << state << std::endl;
	for (int i = 0; i < depth; i++)
	{
		std::cout << char(205);
	}
	std::cout << char(185) << " " << "Liczba rozkazow: " << counter << std::endl;
	for (int i = 0; i < depth; i++)
	{
		std::cout << char(205);
	}
	std::cout << char(188) << " " << "Wartosci rejestrow: " << registers[0] << " " << registers[1] << " " << registers[2] << " " << registers[3] << std::endl << std::endl;
}

//wyœwietlanie wszystkich procesów
void PCB_tree::display_tree()
{
	process.display();
	if (kids.size() > 0)
		for (PCB_tree* i : kids)
		{
			depth++;
			i->display_tree();
			depth--;
		}
}

//wypelnianie danymi stworzonego procesu
void PCB_tree::fill_data(PCB&p, const std::string &p_name, const std::string &file_name)
{
	p.PID = free_pid;
	p.p_name = p_name;
	p.file_name = file_name;
	p.set_state(READY);
	p.pdesk[0] = -1;
	p.pdesk[1] = -1;
	QRP.push_back(&p);
	free_pid++;
}

void PCB_tree::fill_data(PCB&p, const std::string &p_name)
{

	p.PID = free_pid;
	p.p_name = p_name;
	p.file_name = "";
	p.set_state(NEW);
	p.pdesk[0] = -1;
	p.pdesk[1] = -1;
	free_pid++;
}

void PCB_tree::fork(PCB*p)
{
	//dodawanie dziecka inita
	if (p->PID == process.PID)
	{
		PCB_tree *p_new = new PCB_tree();
		p_new->father = p;
		fill_data(p_new->process, "");
		kids.push_back(p_new);
		std::cout << darkblue << "[PM] " << white << "Stworzono proces bez programu.\n" << std::endl;
	}
	//dodawanie dziecka potomka inita
	else {
		//szukanie procesu ktorego wskazalismy jako ojca wsrod potomkow procesu init
		for (PCB_tree *temp1 : kids)
		{
			if (temp1->process.PID == p->PID)
			{
				PCB_tree *p_new = new PCB_tree();
				p_new->father = p;
				fill_data(p_new->process, "");
				temp1->kids.push_back(p_new);
				std::cout << darkblue << "[PM] " << white << "Stworzono proces bez programu.\n" << std::endl;
				break;
			}
			//szukanie procesu ktorego wskazalismy jako ojca wsrod dzieci potomkow procesu init
			else if (temp1->kids.size() != 0)
			{
				for (PCB_tree *temp2 : temp1->kids)
				{
					if (temp2->process.PID == p->PID)
					{
						PCB_tree *p_new = new PCB_tree();
						p_new->father = p;
						fill_data(p_new->process, "");
						temp2->kids.push_back(p_new);
						std::cout << darkblue << "[PM] " << white << "Stworzono proces bez programu.\n" << std::endl;
						break;
					}
				}
			}
		}
	}
};

bool PCB_tree::fork(PCB*p, const std::string &name)
{
	if (findpcb(name))
	{
		std::cout << darkblue << "[PM] " << red << "Proces o danej nazwie juz istnieje" << white << std::endl;
		return 0;
	}
	else
	{
		//dodawanie dziecka inita
		if (p->PID == process.PID)
		{
			PCB_tree *p_new = new PCB_tree();
			p_new->father = p;
			fill_data(p_new->process, name);
			kids.push_back(p_new);
			std::cout << darkblue << "[PM] " << white << "Stworzono proces " << p_new->process.p_name << " bez programu.\n" << std::endl;
		}
		//dodawanie dziecka potomka inita
		else {
			//szukanie procesu ktorego wskazalismy jako ojca wsrod potomkow procesu init
			for (PCB_tree *temp1 : kids)
			{
				if (temp1->process.PID == p->PID)
				{
					PCB_tree *p_new = new PCB_tree();
					p_new->father = p;
					fill_data(p_new->process, name);
					temp1->kids.push_back(p_new);
					std::cout << darkblue << "[PM] " << white << "Stworzono proces " << p_new->process.p_name << " bez programu.\n" << std::endl;
					break;
				}
				//szukanie procesu ktorego wskazalismy jako ojca wsrod dzieci potomkow procesu init
				else if (temp1->kids.size() != 0)
				{
					for (PCB_tree *temp2 : temp1->kids)
					{
						if (temp2->process.PID == p->PID)
						{
							PCB_tree *p_new = new PCB_tree();
							p_new->father = p;
							fill_data(p_new->process, name);
							temp2->kids.push_back(p_new);
							std::cout << darkblue << "[PM] " << white << "Stworzono proces " << p_new->process.p_name << " bez programu.\n" << std::endl;
							break;
						}
					}
				}
			}
		}
		return 1;
	}
	
};

bool PCB_tree::fork(PCB*p, const std::string &name, const std::string &file_name)
{
	if (findpcb(name))
	{
		std::cout << darkblue << "[PM] " << red << "Proces o danej nazwie juz istnieje" << white << std::endl;
		return 0;
	}
	else
	{
		//dodawanie dziecka inita
		if (p->PID == process.PID)
		{
			PCB_tree *p_new = new PCB_tree();
			p_new->father = p;
			fill_data(p_new->process, name, file_name);
			kids.push_back(p_new);
			std::cout << darkblue << "[PM] " << white << "Stworzono proces " << p_new->process.p_name << std::endl;
		}

		//dodawanie dziecka potomka inita
		else {

			//szukanie procesu ktorego wskazalismy jako ojca wsrod potomkow procesu init
			for (PCB_tree *temp1 : kids)
			{
				if (temp1->process.PID == p->PID)
				{
					PCB_tree *p_new = new PCB_tree();
					p_new->father = p;
					fill_data(p_new->process, name, file_name);
					temp1->kids.push_back(p_new);
					std::cout << darkblue << "[PM] " << white << "Stworzono proces " << p_new->process.p_name << std::endl;
					break;
				}

				//szukanie procesu ktorego wskazalismy jako ojca wsrod dzieci potomkow procesu init
				else if (temp1->kids.size() != 0)
				{
					for (PCB_tree *temp2 : temp1->kids)
					{
						if (temp2->process.PID == p->PID)
						{
							PCB_tree *p_new = new PCB_tree();
							p_new->father = p;
							fill_data(p_new->process, name, file_name);
							temp2->kids.push_back(p_new);
							std::cout << darkblue << "[PM] " << white << "Stworzono proces " << p_new->process.p_name << std::endl;
							break;
						}
					}
				}
			}
		}
		return 1;
	}
};

void PCB_tree::toDummy(PCB_tree *tree)
{
	int howmanykids1 = tree->kids.size();

	for (int j = 0; j < howmanykids1; j++)
	{
		kids.push_back(tree->kids[0]);
		tree->kids.erase(tree->kids.begin());
	}
}

void PCB_tree::kill(const int &pid)
{
	if (pid != 1)
	{
		int howmanykids = kids.size();
		for (int i = 0; i < howmanykids; i++)
		{
			//usuwanie jesli proces nie ma potomkow
			if (kids[i]->process.PID == pid && kids[i]->kids.size() == 0)
			{
				kids.erase(kids.begin() + i);
				break;
			}
			else if (kids[i]->process.PID == pid && kids[i]->kids.size() != 0)
			{
				toDummy(kids[i]);
				kids.erase(kids.begin() + i);
				break;
			}
			//usuwanie potomka dziecka inita
			else if (kids[i]->kids.size() != 0)
			{
				kids[i]->kill(pid);
			}
		}
	}
	else
	{
		std::cout << darkblue <<  "[PM] " << red <<"Nie mozna usunac procesu Dummy"  << white << std::endl;
	}
}

//ZnajdŸ PCB po PID
PCB* PCB_tree::findpcb(const int &PID)
{
	if (process.PID == PID)
	{
		return &process;
	}
	else
	{
		for (PCB_tree* i : kids)
		{
			if (i->process.PID == PID)

				return &i->process;
			;
		}
		for (PCB_tree* i : kids)
		{
			PCB* foundPCB = i->findpcb(PID);
			if (foundPCB != nullptr)return foundPCB;
		}
		return nullptr;
	}
}

//ZnajdŸ PCB po nazwie
PCB* PCB_tree::findpcb(const std::string &name)
{
	if (process.p_name == name)
	{
		return &process;
	}
	else
	{
		for (PCB_tree* i : kids)
		{
			if (i->process.p_name == name)

				return &i->process;
			;
		}
		for (PCB_tree* i : kids)
		{
			PCB* foundPCB = i->findpcb(name);
			if (foundPCB != nullptr)return foundPCB;
		}
		return nullptr;
	}
}

