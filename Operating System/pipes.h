#pragma once
#include "Procesor_Manager.h"



extern class PCB;


class Pipe
{
public:
	Pipe(PCB* pi1, PCB* p2);
	~Pipe();
	std::string read(unsigned int l);	//funkcja odczytu z parametrem d�ugo�ci wiadomo�ci kt�r� chcemy zapisa�
	void write(const std::string& msg);	//funkcja zapisu z parametrem wiadomo�ci do przes�ania
	std::deque<char> buffer; //potok jako kolejka
	PCB  *pi1;	//wska�nik na proces wysy�aj�cy komunikaty
	PCB  *p2;	//wska�nik na proces odczytuj�cy
	//Pipeline *pl;	//wska�nik do klasy nadrz�dnej
	int max_buf_size = 16;// max ilosc bajtow w potoku/queue<char>buffer

						  //Sync sync;
};

static class Pipeline
{
public:
	Pipeline();
	static void createPipe(PCB *pi1, PCB *p2); //funkcja tworz�ca potok gdzie pi1 to referencja do procesu wysy�aj�cego a p2 do odbieraj�cego
	static void deletePipe(PCB* pi1);	// funkcja usuwaj�ca potok
	bool Pipe_exist(PCB* pi1, PCB *p2); //istnieje

		//wektor przechowuj�cy wszystkie aktywne potoki
	static Pipe* GetPipe(int PID_1, int PID_2);
};
