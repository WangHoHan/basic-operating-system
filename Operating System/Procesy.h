#pragma once
#include<vector>
#include<queue>
#include<string>
#include<iostream>
#include<deque>
#include "ConsoleColor.h"

extern class PageTable;


enum State {
	NEW, READY, RUNNING, WAITING, DUMMY
};

class PCB {
public:
	int PID;
	std::string p_name;
	State state;
	std::vector<PageTable> *page_table;
	std::string file_name;
	int counter;
	int registers[4];
	int pdesk[2];

	PCB() {
		for (int i = 0; i < 4; i++)
		{
			this->registers[i] = 0;
		}
		this->counter = 0;
		this->state = NEW;
		this->pdesk[0] = -1;
		this->pdesk[1] = -1;
	};

	PCB(int a) {
		this->PID = a;
		this->file_name = "";
		this->p_name = "Dummy";
		for (int i = 0; i < 4; i++)
		{
			this->registers[i] = 0;
		}
		this->counter = 0;
		this->state = DUMMY;
	}

	void display();

	void set_state(State a);


};

class PCB_tree {
public:
	PCB process;
	PCB *father;
	std::vector<PCB_tree*>kids;

	bool fork(PCB *p, const std::string &name);
	bool fork(PCB *p, const std::string &name, const std::string &filename);
	void fork(PCB*p);
	void kill(const int &pid);

	PCB* findpcb(const int &PID);
	PCB* findpcb(const std::string &nazwa);
	void display_tree();

	void fill_data(PCB &p, const std::string &p_name, const std::string &file_name);
	void fill_data(PCB &p, const std::string &file_name);
	void toDummy(PCB_tree *tree);

};
