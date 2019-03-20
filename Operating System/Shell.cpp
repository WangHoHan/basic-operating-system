#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <typeinfo>
#include "Shell.h"
#include "fafms.h"
#include "pipes.h"
#include "Memory.h"
#include "Procesy.h"
#include "Interpreter.h"
#include "welcome.h"

// PCB_tree drzewko;
PCB *p1 = new PCB(1);
int licznik = 0;

void Shell::fill_cmd_vec() {

	cmd.clear();

	if (line.size() > 0) {
		std::istringstream iss(line);
		std::vector<std::string> buffer(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
		cmd = buffer;
	}
}

bool Shell::is_unshort(std::string str) {

	// nie ma roznicy pomiedzy 'unsigned short int' a 'unsigned short'
	// unsigned short   -->  ma zasieg:  0 do 65,535

	for (int i = 0; i < str.size(); i++) {
		if (str[i] < 48 || str[i] > 57) {
			return false;
		}
	}
	if (str.size() > 5) {
		return false;
	}
	if (std::stoi(str) > 65535) {
		return false;
	}
	return true;
}


unsigned short Shell::stous(std::string str) {
	return (unsigned short)std::stoi(str);
}


bool Shell::stob(std::string str) {
	if (str == "rw") return true; // rw - read and write (tryb otwarcia wy³¹czny)
	else if (str == "r") return false; // r - read olny (tryb otwarcia wspo³dzielony)
}

void Shell::display_help() {

	std::cout << magenta << "[SHELL] " << white << "\nDostepne komendy:\n\n";

	// -h

	std::cout << "cmd -h - command help (wyswietlenie argumentow dla wybranej komendy)\n\n";

	// PRACA KROKOWA

	std::cout << "ENTER\n\n - step work (praca krokowa)";

	// PLIKI

	std::cout << "cf - create file (tworzenie pliku)\n"; // ok [folder_name][file_name][s_folder_name]*
	std::cout << "ul - unlink (usuwanie dowiazan)\n"; // ok [file_name][s_folder_name]*
	std::cout << "op - open file (otwarcie pliku)\n"; // ok [file_name][open_type][s_folder_name]*   open_type == (r | rw)
	std::cout << "cl - close file (zamkniecie otwartego pliku)\n"; // ok [file_name][s_folder_name]*
	std::cout << "tc - truncate (skracanie pilku)\n"; // ok [file_name][final_bytes_amount][s_folder_name]*
	std::cout << "rds - read sequential (odczytywanie z pliku sekwencyjnie)\n"; // ok [file_name][bytes_amount][s_folder_name]*
	std::cout << "rdi - read index (odczytywanie z pliku indeksowo)\n"; // ok [file_name][byte_number_in_file][bytes_amount][s_folder_name]*
	std::cout << "wrts - write sequential (zapisywanie do pliku sekwencyjnie)\n"; // ok [folder_name][data][s_folder_name]*
	std::cout << "wrti - write index (zapisywanie do pliku indeksowo)\n"; // ok [file_name][data][byte_number_in_file][s_folder_name]*
	std::cout << "lsk - lseek (zmiana wskazania biezacej pozycji)\n\n"; // ok [file_name][move_by_amount_in_bytes][reference_point][s_folder_name]*

	// KATALOGI

	std::cout << "dfc - draw folder content (wyswietlenie zawartosci folderu)\n"; // ok [folder_name]
	std::cout << "cd - create directory (tworzenie katalogu)\n"; // ok [folder_name][folder_to_create_name][sfolder_name]*
	std::cout << "dd - delete directory (usuwanie katalogu)\n"; // ok [folder_name][sfolder_name]*
	std::cout << "cdr - create directory registration (tworzenie wpisu katalogowego)\n"; // ok [folder_name][file_name][file_name][sfolder_name]*
	std::cout << "ddr - delete directory registration (usuwanie wpisu katalogowego)\n"; // ok [file_name][sfolder_name]*
	std::cout << "frn - file rename (przemianowanie pliku)\n"; // ok [file_name][new_file_name][sfolder_name]*
	std::cout << "dc - directory content (tworzenie wykazu spisu zawartosci)\n"; // ok [folder_name][sfolder_name]*
	std::cout << "fds - file/directory search (szukanie pliku/katalogu)\n\n"; // ok [file_name]

	// DYSK OGOLNIE

	std::cout << "sb - show block (wyswietlenie bloku dyskowego za pomoca symboli ASCII)\n"; // ok []
	std::cout << "sbd - show block d (wyswietlenie bloku dyskowego za pomoca wartosci liczbowych)\n"; // ok []
	std::cout << "sd - show disc (wyswietlenie zawartosci dysku za pomoca symboli ASCII)\n"; // ok []
	std::cout << "sdd - show disc d (wyswietlenie zawartosci dysku za pomoca wartosci liczbowych)\n\n"; // ok []

	// PAMIEC RAM

	std::cout << "mdr - display ram (wyswietlenie aktualnego stanu ramu)\n\n"; // ok []

	// PAMIEC WIRTUALNA

	std::cout << "mdsf - display swap file (wyswietlenie pliku wymiany)\n"; // ok []
	std::cout << "mdl - display lru list (wyswietlenie listy lru)\n\n"; // ok []

	// PROCESY

	std::cout << "cp - create process (utworzenie procesu)\n"; // [process_name]
	std::cout << "exe - execute (wykonanie programu)\n"; // ok [process_name][file_name]
	std::cout << "load (wczytywanie programu do procesu)\n"; // ok [process_name][file_name]
	std::cout << "dt - display processes tree (wyswietlenie drzewa procesow)\n\n"; // ok []

	// HELP

	std::cout << "help (wyswietlenie wszystkich komend)\n"; // ok []
	std::cout << "logo (wyswietlenie logo systemu operacyjnego)\n"; // ok []
	std::cout << "creators(wyswietlenie tworcow systemu)\n\n"; // ok []

	// EXIT

	std::cout << "exit (wyjscie z konsoli)\n"; // ok []

	// ERROR
}

void Shell::call_method() {

	// PRACA KROKOWA

	if (cmd.empty()) {

		Execute();
		
		return;
	}

	// PLIKI

	if (cmd[0] == "cf") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[folder_name][file_name][s_folder_name]*\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else if (cmd.size() == 3) {
			if (create(cmd[1], cmd[2])) {
				std::cout << magenta << "[SHELL] " << white << "Utworzono plik " << cmd[2] << ".\n";
			}
		}
		else if (cmd.size() == 4) {
			if (create(cmd[1], cmd[2], cmd[3])) {
				std::cout << magenta << "[SHELL] " << white << "Utworzono plik " << cmd[2] << " w " << cmd[3] << ".\n";
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "ul") {

		if (cmd.size() == 2) {

			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[file_name][s_folder_name]*\n";
			}
			else if (unLink(cmd[1])) {
				std::cout << magenta << "[SHELL] " << white << "Usunieto dowiazanie " << cmd[1] << ".\n";
			}
		}
		else if (cmd.size() == 3) {
			if (unLink(cmd[1], cmd[2])) {
				std::cout << magenta << "[SHELL] " << white << "Usunieto dowiazanie " << cmd[1] << ".\n";
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "op") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[file_name][open_type][s_folder_name]*   open_type == (r | rw)\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else if (cmd.size() == 3) {

			if (cmd[2] == "rw" || cmd[2] == "r") {
				if (open(cmd[1], stob(cmd[2]))) {
					std::cout << magenta << "[SHELL] " << white << "Otworzono plik.\n";
				}
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Nie istnieje taki tryb otwarcia.\n" << white;
			}
		}
		else if (cmd.size() == 4) {

			if (cmd[2] == "rw" || cmd[2] == "r") {
				if (open(cmd[1], stob(cmd[2]), cmd[3])) {
					std::cout << magenta << "[SHELL] " << white << "Otworzono plik.\n";
				}
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Nie istnieje taki tryb otwarcia.\n" << white;
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "cl") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[file_name][s_folder_name]*\n";
			}
			else if (close(cmd[1])) {
				std::cout << magenta << "[SHELL] " << white << "Zamknieto plik.\n";
			}
		}
		else if (cmd.size() == 3) {
			if (close(cmd[1], cmd[2])) {
				std::cout << magenta << "[SHELL] " << white << "Zamknieto plik.\n";
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	// zabezpieczyc zakres
	else if (cmd[0] == "tc") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[file_name][final_bytes_amount][s_folder_name]*\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else if (cmd.size() == 3) {

			if (is_unshort(cmd[2])) {

				if (truncate(cmd[1], stous(cmd[2]))) {
					std::cout << magenta << "[SHELL] " << white << "Skrocono plik.\n";
				}
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Drugi argument musi byc liczba nieujemna.\n" << white;
			}
		}
		else if (cmd.size() == 4) {

			if (is_unshort(cmd[2])) {

				if (truncate(cmd[1], stous(cmd[2]), cmd[3])) {
					std::cout << magenta << "[SHELL] " << white << "Skrocono plik.\n";
				}
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Drugi argument musi byc liczba nieujemna.\n" << white;
			}
			
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "rds") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[file_name][bytes_amount][s_folder_name]*\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else if (cmd.size() == 3) {

			char *data = new char[1];

			if (is_unshort(cmd[2])) {

				short int r = read(cmd[1], data, stous(cmd[2]));
				if (r == 0) {
					std::cout << magenta << "[SHELL] " << red << "Nieudany odczyt.\n" << white;
				}
				else if (r > 0) {
					std::cout << magenta << "[SHELL] " << white << "Udany odczyt.\n";
					for (int i = 0; i < r; i++) {
						std::cout << data[i];
					}
					std::cout << "\n";
				}
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Drugi argument musi byc liczba nieujemna.\n" << white;
			}
			
		}
		else if (cmd.size() == 4) {

			char *data = new char[1];

			if (is_unshort(cmd[2])) {

				short int r = read(cmd[1], data, stous(cmd[2]), cmd[3]);
				if (r == 0) {
					std::cout << magenta << "[SHELL] " << red << "Nieudany odczyt.\n" << white;
				}
				else if (r > 0) {
					std::cout << magenta << "[SHELL] " << white << "Udany odczyt.\n";
					for (int i = 0; i < r; i++) {
						std::cout << data[i];
					}
					std::cout << "\n";
				}
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Drugi argument musi byc liczba nieujemna.\n" << white;
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "rdi") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[file_name][byte_number_in_file][bytes_amount][s_folder_name]*\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else if (cmd.size() == 4) {
			char *data = new char[1];
			short int r = read(cmd[1], data, stous(cmd[2]), stous(cmd[3]));
			if (r == 0) {
				std::cout << magenta << "[SHELL] " << red << "Nieudany odczyt.\n" << white;
			}
			else if (r > 0) {
				std::cout << magenta << "[SHELL] " << white << "Udany odczyt.\n";
				for (int i = 0; i < r; i++) {
					std::cout << data[i];
				}
				std::cout << "\n";
			}
		}
		else if (cmd.size() == 5) {
			char *data = new char[1];
			short int r = read(cmd[1], data, stous(cmd[2]), stous(cmd[3]), cmd[4]);

			if (r == 0) {
				std::cout << magenta << "[SHELL] " << red << "Nieudany odczyt.\n" << white;
			}
			else if (r > 0) {
				std::cout << magenta << "[SHELL] " << white << "Udany odczyt.\n";
				for (int i = 0; i < r; i++) {
					std::cout << data[i];
				}
				std::cout << "\n";
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	// _ to spacja ("podloga" to spacja)
	else if (cmd[0] == "wrts") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[folder_name][data][s_folder_name]*\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else if (cmd.size() == 3) {
			std::vector<char> vec;

			for (int i = 0; i < cmd[2].size(); i++) {

				if (cmd[2][i] == (char)95) {
					vec.push_back((char)32);
				}
				else {
					vec.push_back(cmd[2][i]);
				}
			}

			short int w = write(cmd[1], vec);

			std::cout << magenta << "[SHELL] " << white << "Zapisano " << w << " bajtow.\n";
		}
		else if (cmd.size() == 4) {
			std::vector<char> vec;

			for (int i = 0; i < cmd[2].size(); i++) {

				if (cmd[2][i] == (char)95) {
					vec.push_back((char)32);
				}
				else {
					vec.push_back(cmd[2][i]);
				}
			}

			short int w = write(cmd[1], vec, cmd[3]);

			std::cout << magenta << "[SHELL] " << white << "Zapisano " << w << " bajtow.\n";
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	// zakres
	else if (cmd[0] == "wrti") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[file_name][data][byte_number_in_file][s_folder_name]*\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else if (cmd.size() == 4) {

			if (is_unshort(cmd[3])) {
				
				std::vector<char> vec;

				for (int i = 0; i < cmd[2].size(); i++) {

					if (cmd[2][i] == (char)95) {
						vec.push_back((char)32);
					}
					else {
						vec.push_back(cmd[2][i]);
					}
				}

				short int w = write(cmd[1], vec, stous(cmd[3]));

				std::cout << magenta << "[SHELL] " << white << "Zapisano " << w << " bajtow.\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Drugi argument musi byc liczba nieujemna.\n" << white;
			}
		}
		else if (cmd.size() == 5) {

			if (is_unshort(cmd[3])) {

				std::vector<char> vec;

				for (int i = 0; i < cmd[2].size(); i++) {

					if (cmd[2][i] == (char)95) {
						vec.push_back((char)32);
					}
					else {
						vec.push_back(cmd[2][i]);
					}
				}

				short int w = write(cmd[1], vec, stous(cmd[3]), cmd[4]);

				std::cout << magenta << "[SHELL] " << white << "Zapisano " << w << " bajtow.\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Drugi argument musi byc liczba nieujemna.\n" << white;
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "lsk") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[file_name][move_by_amount_in_bytes][reference_point][s_folder_name]*\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else if (cmd.size() == 4) {

			if (is_unshort(cmd[2]) || is_unshort(cmd[3])) {

				short i = lseek(cmd[1], stous(cmd[2]), stous(cmd[3]));
				if (i != -1) {
					std::cout << magenta << "[SHELL] " << white << "Zmiana wskazania biezacej pozycji.\n";
				}
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Drugi i trzeci argument musza byc liczbami nieujemnymi.\n" << white;
			}
		}
		else if (cmd.size() == 5) {

			if (is_unshort(cmd[2]) || is_unshort(cmd[3])) {

				short i = lseek(cmd[1], stous(cmd[2]), stous(cmd[3]), cmd[4]);
				if (i != -1) {
					std::cout << magenta << "[SHELL] " << white << "Zmiana wskazania biezacej pozycji.\n";
				}
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Drugi i trzeci argument musza byc liczbami nieujemnymi.\n" << white;
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	// KATALOGI

	else if (cmd[0] == "dfc") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[folder_name]\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << white << "Wyswietlenie zawartosci folderu.\n";
				drawFolderContents(cmd[1]);
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "cd") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[folder_name][folder_to_create_name][sfolder_name]*\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else if (cmd.size() == 3) {
			if (fcreate(cmd[1], cmd[2])) {
				std::cout << magenta << "[SHELL] " << white << "Utworzono katalog " << cmd[2] << ".\n";
			}
		}
		else if (cmd.size() == 4) {
			if (fcreate(cmd[1], cmd[2], cmd[3])) {
				std::cout << magenta << "[SHELL] " << white << "Utworzono katalog " << cmd[2] << ".\n";
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "dd") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[folder_name][sfolder_name]*\n";
			}
			else if (fdelete(cmd[1])) {
				std::cout << magenta << "[SHELL] " << white << "Usunieto katalog" << cmd[1] << ".\n";
			}
		}
		else if (cmd.size() == 3) {
			if (fdelete(cmd[1], cmd[2])) {
				std::cout << magenta << "[SHELL] " << white << "Usunieto katalog" << cmd[1] << ".\n";
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "cdr") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[folder_name][file_name][file_name][sfolder_name]*\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else if (cmd.size() == 4) {
			if (fecreate(cmd[1], cmd[2], fsearch(cmd[3], 0))) {
				std::cout << magenta << "[SHELL] " << white << "Utworzono wpis.\n";
			}
		}
		else if (cmd.size() == 5) {
			if (fecreate(cmd[1], cmd[2], fsearch(cmd[3], 0, cmd[4]), cmd[4])) {
				std::cout << magenta << "[SHELL] " << white << "Utworzono wpis.\n";
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "ddr") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[file_name][sfolder_name]*\n";
			}
			else if (fedelete(cmd[1])) {
				std::cout << magenta << "[SHELL] " << white << "Usunieto wpis.\n";
			}
		}
		else if (cmd.size() == 3) {
			if (fedelete(cmd[1], cmd[2])) {
				std::cout << magenta << "[SHELL] " << white << "Usunieto wpis.\n";
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "frn") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[file_name][new_file_name][sfolder_name]*\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else if (cmd.size() == 3) {
			if (frename(cmd[1], cmd[2])) {
				std::cout << magenta << "[SHELL] " << white << "Plik " << cmd[1] << " zmienil nazwe na " << cmd[2] << ".\n";
			}
		}
		else if (cmd.size() == 4) {
			if (frename(cmd[1], cmd[2], cmd[3])) {
				std::cout << magenta << "[SHELL] " << white << "Plik " << cmd[1] << " zmienil nazwe na " << cmd[2] << ".\n";
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "dc") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[folder_name][sfolder_name]*\n";
			}
			else {
				std::vector<FEntry> vec = fcontent(cmd[1]);

				if (vec.empty()) {
					std::cout << magenta << "[SHELL] " << red << "Folder nie istnieje.\n" << white;
				}
				else {
					std::cout << magenta << "[SHELL] " << white << "Zawartosc folderu.\n";
					for (int i = 0; i < vec.size(); i++) {
						std::cout << vec[i];
					}
				}
			}
		}
		else if (cmd.size() == 3) {
			std::vector<FEntry> vec = fcontent(cmd[1], cmd[2]);

			if (vec.empty()) {
				std::cout << magenta << "[SHELL] " << red << "Folder nie istnieje.\n" << white;
			}
			else {
				std::cout << magenta << "[SHELL] " << white << "Zawartosc folderu.\n";
				for (int i = 0; i < vec.size(); i++) {
					std::cout << vec[i];
				}
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "fds") {

		if (cmd.size() == 2) {
			
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[file_name]\n";
			} 
			else {
				short int num = fsearch(cmd[1], 0);

				if (num < 0) {
					std::cout << magenta << "[SHELL] " << red << "Nie znaleziono pliku.\n" << white;
				}
				else {
					std::cout << magenta << "[SHELL] " << white << "Numer i-wezla wskazanego pliku to: " << num << std::endl;
				}
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	// DYSK OGOLNIE

	else if (cmd[0] == "sb") {

		if (cmd.size() == 2) {

			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[]\n";
			}
			else {
				if (is_unshort(cmd[1])) {
					std::cout << magenta << "[SHELL] " << white << "Wyswietlenie bloku dyskowego za pomoca symbolow ASCII.\n";
					drawBlock(stous(cmd[1]));
				}
				else {
					std::cout << magenta << "[SHELL] " << red << "Argument musi byc liczba nieujemna.\n" << white;
				}
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}
	
	else if (cmd[0] == "sbd") {

		if (cmd.size() == 2) {

			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[]\n";
			} 
			else {
				if (is_unshort(cmd[1])) {
					std::cout << magenta << "[SHELL] " << white << "Wyswietlenie bloku dyskowego za pomoca wartosci liczbowych.\n";
					drawBlockD(stous(cmd[1]));
				}
				else {
					std::cout << magenta << "[SHELL] " << red << "Argument musi byc liczba nieujemna.\n" << white;
				}
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "sd") {

		if (cmd.size() == 1) {
			std::cout << magenta << "[SHELL] " << white << "Wyswietlenie zawartosci dysku za pomoca symbolow ASCII.\n";
			drawDisc();
		}
		else if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[]\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "sdd") {

		if (cmd.size() == 1) {
			std::cout << magenta << "[SHELL] " << white << "Wyswietlenie zawartosci dysku za pomoca wartosci liczbowych.\n";
			drawDiscD();
		}
		else if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[]\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	// PAMIEC RAM

	else if (cmd[0] == "mdr") {

		if (cmd.size() == 1) {
			std::cout << magenta << "[SHELL] " << white << "Wyswietlenie aktualnego stanu pamieci ram.\n";
			displayRAM();
		}
		else if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[]\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	// PAMIEC WIRTUALNA

	else if (cmd[0] == "mdsf") {

		if (cmd.size() == 1) {
			std::cout << magenta << "[SHELL] " << white << "Wyswietlenie pliku wymiany.\n";
			showSwapFile();
		}
		else if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[]\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "mdl") {

		if (cmd.size() == 1) {
			std::cout << magenta << "[SHELL] " << white << "Wyswietlenie listy LRU.\n";
			showLRUList();
		}
		else if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[]\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	// PROCESY

	else if (cmd[0] == "cp") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[process_name][file_name]\n";
			}
			else {
				drzewko.fork(p1, cmd[1]);
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "exe") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[process_name][file_name]\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else if (cmd.size() == 3) {
			drzewko.fork(p1, cmd[1], cmd[2]);
			drzewko.findpcb(cmd[1])->page_table = createPageTable(cmd[2], drzewko.findpcb(cmd[1])->PID);
			Loadfromfile(cmd[2], drzewko.findpcb(cmd[1])->PID);
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "load") {

		if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[process_name][file_name]\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else if (cmd.size() == 3) {
			drzewko.findpcb(cmd[1])->page_table = createPageTable(cmd[2], drzewko.findpcb(cmd[1])->PID);
			Loadfromfile(cmd[2], drzewko.findpcb(cmd[1])->PID);
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "dt") {

		if (cmd.size() == 1) {
			drzewko.display_tree();
		}
		else if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[]\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	// HELP

	else if (cmd[0] == "creators")
	{
		if (cmd.size() == 1) {
			display_creators();
		}
		else if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[]\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}
	else if (cmd[0] == "logo")
	{
		if (cmd.size() == 1) {
			display_logo();
		}
		else if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[]\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	else if (cmd[0] == "help") {

		if (cmd.size() == 1) {
			display_help();
		}
		else if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[]\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	// EXIT

	else if (cmd[0] == "exit") {

		if (cmd.size() == 1) {
			condition = false;
		}
		else if (cmd.size() == 2) {
			if (cmd[1] == "-h") {
				std::cout << "\n" << cmd[0] << " " << "[]\n";
			}
			else {
				std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
			}
		}
		else {
			std::cout << magenta << "[SHELL] " << red << "Niepoprawna liczba argumentow.\n" << white;
		}
	}

	// ERROR

	else {
		std::cout << magenta << "[SHELL] " << red << "Bledna nazwa komendy." << white;
	}
}

void Shell::run() {

	while (condition) {

		std::cout << "\ncmd: ";
		getline(std::cin, line);

		fill_cmd_vec();
		call_method();
	}
}

int main() {

	display_creators(); // wyswietla tworcow 
	display_logo(); // wyswietla logo

	initializeFileSystem(); // inicjalizuje dysk

	initializeRAM(); // inicjalizuje ram

	Initialize(); // inicjalizuje virtual

	drzewko.process = *p1;
	p1->page_table = createPageTableD(4, 1);
	QRP.push_back(p1);
	//FC1 ROOT BIG FC1 BIG F1 CF1 F1 P1 CF1 F1 P2 DFC BIG FED1 P1 DFC BIG RST
	std::string program1 = "MO AX 0 MO BX 1 MO CX 5 AD AX BX DC CX CO CX 0 JZ 82 AD BX AX DC CX CO CX 0 JN 24 RST";
	std::string program2 = "FC1 ROOT BIG FC1 BIG F1 CF1 F1 P1 CF1 F1 P2 DFC BIG FED1 P1 OP1 P2 1 WR1 P2 HELLO WR1 P2 hello LS1 P2 0 1 RD1 P2 2 CL1 P2 DFC BIG RST";
	std::string program3 = "CP P1 CP P2 CPL P1 P2 RPIP P1 P2 3 RPIP P1 P2 3 RPIP P1 P2 3 RPIP P1 P2 3 RPIP P1 P2 3 DPL P1 KP P1 KP P2 RST";
	std::string program4 = "WPIP P1 P2 RuMpElStIlTsKiN RST";
	std::string program5 = "CP P3 KP P3 LL MO AX 0 IN AX IN AX IN AX IN AX LU RST";
	
	std::vector<char> PLIK1;
	for (int u = 0; u < program1.size(); u++) {
		PLIK1.push_back(program1[u]);
	}
	std::vector<char> PLIK2;
	for (int u = 0; u < program2.size(); u++) {
		PLIK2.push_back(program2[u]);
	}
	std::vector<char> PLIK3;
	for (int u = 0; u < program3.size(); u++) {
		PLIK3.push_back(program3[u]);
	}
	std::vector<char> PLIK4;
	for (int u = 0; u < program4.size(); u++) {
		PLIK4.push_back(program4[u]);
	}

	// zamki <- opcjonalnie
	std::vector<char> PLIK5;
	for (int u = 0; u < program5.size(); u++) {
		PLIK5.push_back(program5[u]);
	}

	fcreate("root", "PROGRAM");
	create("PROGRAM", "file1");
	open("file1", true);
	write("file1", PLIK1);
	close("file1");

	create("PROGRAM", "file2");
	open("file2", true);
	write("file2", PLIK2);
	close("file2");

	create("PROGRAM", "file3");
	open("file3", true);
	write("file3", PLIK3);
	close("file3");

	create("PROGRAM", "file4");
	open("file4", true);
	write("file4", PLIK4);
	close("file4");

	// zamki <- opcjonalnie
	create("PROGRAM", "file5");
	open("file5", true);
	write("file5", PLIK5);
	close("file5");

	Shell shell;

	shell.run();

	return 0;
}