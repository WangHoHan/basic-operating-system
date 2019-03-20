#pragma once
#include"Procesy.h"
#include"fafms.h"
#include<iostream>
#include<string>
#include<vector>
#include<list>
#include<map>

extern class PCB;


class PageTable
{
public:
	bool is_in_memory;
	int frame_number;
	PageTable() = default;
	PageTable(bool is_in_memory, int frame_number)
	{
		this->is_in_memory = is_in_memory;
		this->frame_number = frame_number;
	}
	~PageTable() = default;
};

class Page
{
public:
	char page[16];
	Page() = default;
	Page(std::string data)
	{
		while (data.size() < 16)
		{
			data += " ";
		}
		for (int i = 0; i < 16; i++)
		{
			page[i] = data[i];
		}
	}
	~Page() = default;
	void display()
	{
		for (int i = 0; i < 16; i++)
		{
			std::cout << page[i];
		}
	}
};

class Frame
{
public:
	bool is_busy;
	int page_number;
	int PID;
	std::vector<PageTable> *page_table;
	Frame() = default;
	Frame(bool is_busy, int page_number, int PID, std::vector<PageTable> *page_table)
	{
		this->is_busy = is_busy;
		this->page_number = page_number;
		this->PID = PID;
		this->page_table = page_table;
	}
	~Frame() = default;
};

//-------------------------------------------------------------------------
//Struktury nalezace do MemoryManager
std::vector<Frame> frames_vector;
char RAM[128];
//-------------------------------------------------------------------------
//Struktury nalezace do VirtualMemory
std::list<int> LRU;
std::map<int, std::vector<Page>> SwapFile;
//-------------------------------------------------------------------------
//Funkcje nalezace do MemoryManager
static std::string getOrder(PCB * process, int command_counter);
std::vector<PageTable> *createPageTable(std::string file_name, int PID);
std::vector<PageTable> *createPageTableD(int size_of_program, int PID);
void deleteFromMemory(int PID);
void displayRAM();
void initializeRAM();
void swapPage(Page page, int frame_number, int page_number);
bool writeToMemory(PCB * process, int adress, std::string data);
//-------------------------------------------------------------------------
// Funkcje nalezace do VirtualMemory
void Initialize();
void showSwapFile();
void showLRUList();
void getFrame(int frame_number, int PID, int page_number);
void Update(int PID, int page_number, std::string data);
void Loadfromfile(std::string file_name, int PID);
int whichFrame();
void usedFrame(int frame);
int FindVictimAndSwap(int PID, int num_P);
void remove_process(int PID);
//--------------------------------------------------------------

//Szukanie wolnej ramki
int findFreeFrame()
{
	int free_frame = -1;
	for (int i = 0; i < frames_vector.size(); i++)
	{
		if (frames_vector[i].is_busy == 0)
		{
			free_frame = i;
			break;
		}
	}
	return free_frame;
}

//Wpisywanie do pamieci
int loadIntoMemory(int page_number, int PID, std::vector<PageTable> *page_table)
{
	int frame_number = findFreeFrame();
	if (frame_number == -1)
	{
		frame_number = FindVictimAndSwap(PID, page_number);
	}
	else
	{
		getFrame(frame_number, PID, page_number);
	}
	page_table->at(page_number).is_in_memory = 1;
	page_table->at(page_number).frame_number = frame_number;
	frames_vector[frame_number].is_busy = 1;
	frames_vector[frame_number].page_number = page_number;
	frames_vector[frame_number].PID = PID;
	frames_vector[frame_number].page_table = page_table;
	return frame_number;
}

//Zamiana ramek
void swapPage(Page page, int frame_number, int page_number)
{
	int j = 0;
	for (int i = frame_number * 16; i < frame_number * 16 + 16; i++)
	{
		RAM[i] = page.page[j];
		j++;
	}
	if (frames_vector[frame_number].page_table != NULL) frames_vector[frame_number].page_table->at(frames_vector[frame_number].page_number).is_in_memory = 0;
}

//Pobranie rozkazu z RAMu
static std::string getOrder(PCB * process, int command_counter)
{
	std::string order;
	int page = (command_counter / 16);
	bool end = false;
	int frame = -1;
	if (process->page_table->size() <= page)
	{
		end = true;
		order = "out of range";
	}
	while (!end)
	{
		page = (command_counter / 16);
		if (process->page_table->size() <= page)
		{
			end = true;
			break;
		}
		if (process->page_table->at(page).is_in_memory == 0)
		{
			loadIntoMemory(page, process->PID, process->page_table);
		}
		if (process->page_table->at(page).is_in_memory == 1)
		{
			frame = process->page_table->at(page).frame_number;
			if (RAM[frame * 16 + command_counter - (16 * page)] == ' ')
			{
				end = true;
			}
			else
			{
				order += RAM[frame * 16 + command_counter - (16 * page)];
			}
			command_counter++;
			usedFrame(frame);
		}
	}
	return order;
}
//Tworzenie Page Table
std::vector<PageTable> * createPageTable(std::string file_name, int PID)
{
	int size_of_program = inodeTable[fsearch(file_name, 0)].getSize();
	double number_of_pages;
	number_of_pages = (ceil)((double)size_of_program / 16);
	std::vector<PageTable> *page_table = new std::vector<PageTable>();
	for (int i = 0; i < number_of_pages; i++)
	{
		page_table->push_back(PageTable(0, -1));
	}
	if (drzewko.findpcb(PID)->state == NEW)
	{
		drzewko.findpcb(PID)->set_state(READY);
		QRP.push_back(drzewko.findpcb(PID));
	}
	return page_table;
}

//Tworzenie Page Table dla Dummy
std::vector<PageTable> *createPageTableD(int size_of_program, int PID) {
	{
		double number_of_pages;
		number_of_pages = (ceil)((double)size_of_program / 16);
		std::vector<PageTable> *page_table = new std::vector<PageTable>();
		for (int i = 0; i < number_of_pages; i++)
		{
			page_table->push_back(PageTable(0, -1));
		}
		return page_table;
	}
}

//Usuwanie programu z pamieci
void deleteFromMemory(int PID)
{
	for (int i = 0; i < frames_vector.size(); i++)
	{
		if (frames_vector[i].PID == PID)
		{
			for (int j = i * 16; j < i * 16 + 16; j++)
			{
				RAM[j] = ' ';
			}
			frames_vector.at(i).is_busy = 0;
			frames_vector.at(i).page_number = -1;
			frames_vector.at(i).PID = -1;
			frames_vector.at(i).page_table = NULL;
			remove_process(PID);
		}
	}
}
//Wyswietlanie RAMu
void displayRAM()
{
	std::cout << darkred << "[PAMIEC]" << white << " Zawartosc RAM:" << std::endl;
	for (int i = 0; i < 128; i++)
	{
		std::cout << RAM[i];
	}
	std::cout << std::endl;
}
//Inicjalizacja RAMu
void initializeRAM()
{
	Frame initialize_frame(0, -1, -1, NULL);
	for (int i = 0; i < 8; i++)
	{
		frames_vector.push_back(initialize_frame);
	}
	for (int i = 0; i < 128; i++)
	{
		RAM[i] = ' ';
	}
}

bool writeToMemory(PCB *process, int adress, std::string data)
{
	int page_number = adress / 16;
	if (data.size() == 0)
	{
		return true;
	}
	if (adress + data.size() - 1 > process->page_table->size() * 16 - 1 || adress < 0)
	{
		std::cout << "out of range" << std::endl;
		return false;
	}
	for (int i = 0; i < data.size(); i++) {
		page_number = (adress + i) / 16;
		if (process->page_table->at(page_number).is_in_memory == 0)
		{
			loadIntoMemory(page_number, process->PID, process->page_table);
		}
		RAM[process->page_table->at(page_number).frame_number * 16 + adress + i - (16 * page_number)] = data[i];
	}
	Update(process->PID, adress, data);
	return true;
}

//-----------------------------------------------
//Inicjalizacja Pamieci Wirtualnej
void Initialize() {
	for (int i = 0; i < 8; i++) {
		LRU.push_back(i);
	}
	std::vector<Page> A;
	std::string R = "JP 0";
	Page Dummy(R);
	A.push_back(Dummy);
	SwapFile.insert(std::pair<int, std::vector<Page>>(1, A));
}

//Wyswietlenie pliku wymiany
void showSwapFile() {
	std::vector<Page> A;
	for (std::map<int, std::vector<Page>>::iterator it = SwapFile.begin(); it != SwapFile.end(); ++it) {
		std::cout << "ID procesu: " << it->first << std::endl;
		for (int i = 0; i < it->second.size(); i++) {
			std::cout << "Strona " << i + 1 << ": ";
			it->second[i].display();
			std::cout << std::endl;
		}
	}
}

//Wyswietlenie listy LRU
void showLRUList() {
	for (std::list<int>::iterator it = LRU.begin(); it != LRU.end(); ++it) {
		std::cout << *it;
		if (++it != LRU.end()) std::cout << ", ";
		it--;
	}
	std::cout << "}" << std::endl;
}

//Ladowanie programu z pliku na dysku
void Loadfromfile(std::string file_name, int process_ID) {
	std::vector<Page> A;
	char* a;
	int size = inodeTable[fsearch(file_name, 0)].getSize();

	if (open(file_name, false)) {
		read(file_name, a, size);
		close(file_name);
	}
	else {
		int buff = lseek(file_name, 0, 0);
		lseek(file_name, 0, 1);
		read(file_name, a, size);
		lseek(file_name, buff, 1);
	}

	std::string tmp_page = "";
	int j = 0;
	Page B;
	for (int i = 0; i < size; i++) {
		tmp_page.push_back(a[i]);
		j++;

		if (i == size - 1) {
			while (tmp_page.size() < 16) {
				tmp_page += " ";
				j++;
			}
		}
		if (j == 16) {
			for (int i = 0; i < 16; i++) {
				B.page[i] = tmp_page[i];
			}
			A.push_back(B);
			tmp_page = "";
			j = 0;
		}
	}
	std::pair<std::map<int, std::vector<Page>>::iterator, bool> is_inserted;

	is_inserted = SwapFile.insert(std::pair<int, std::vector<Page>>(process_ID, A));
	if (is_inserted.second == false) {
		std::cout << gray << "[PW]" << red << " Program istnieje juz w pliku wymiany" << white <<std::endl;
	}
	else {
		std::cout << gray << "[PW]" << white << " Program zostal zaladowany pomyslnie" << std::endl;
	}
}

//Ramka zostala uzyta
void usedFrame(int frame) {
	LRU.remove(frame);
	LRU.push_front(frame);
};
//Okreslanie ramki do wymiany
int whichFrame() {
	int a = LRU.back();
	usedFrame(a);
	return a;
}

//Wymiana ramek
int FindVictimAndSwap(int PID, int num_P) {
	int frame = -1;
	std::vector<Page> A = SwapFile.at(PID);
	Page B = A[num_P];
	frame = whichFrame();
	swapPage(B, frame, num_P);
	return frame;

}

//Usuwanie programu z pliku wymiany
void remove_process(int PID) {
	if (PID != 1) SwapFile.erase(PID);
}

//Wymiana konkretnej ramki(okreslonej jako wolna przez RAM
void getFrame(int frame_number, int PID, int page_number)
{
	std::vector<Page> A = SwapFile.at(PID);
	Page B = A[page_number];
	swapPage(B, frame_number, page_number);
}

//Zmiana czesci programu zaladowanego do pliku wymiany
void Update(int PID, int adress, std::string data)
{
	int page_number = adress / 16;
	adress = adress - (page_number * 16);
	std::vector<Page> A = SwapFile.at(PID);
	for (int i = 0; i < data.size(); i++) {
		A[page_number].page[i + adress] = data[i];
	}
	SwapFile.at(PID) = A;
}