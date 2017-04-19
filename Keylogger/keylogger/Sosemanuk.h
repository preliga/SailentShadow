#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
using namespace std;

class Sosemanuk
{
private:
	unsigned int klucz[8];
	unsigned int IV[4];
	unsigned char *klucz_szyfrujacy;

	string szyfrogram;
	int key_position;

public:
	Sosemanuk(string klucz, string IV, int dlugosc_strumienia_klucza);
	~Sosemanuk();
	int Rol(unsigned int a, unsigned int b);
	int Ror(unsigned int a, unsigned int b);
	void Funkcja_SBOX(int runda, unsigned int &w0, unsigned int &w1, unsigned int &w2, unsigned int &w3);
	unsigned int* Key_schedule();
	unsigned int* IV_injection(unsigned int* subkey);
	unsigned int Little_endian(unsigned int val);
	unsigned int* Make_stream(unsigned int* lfsr, unsigned int* fsmR, unsigned int* mulAlpha, unsigned int* divAlpha);
	void Create_stream(int dlugosc_strumienia_klucza);
	string normalizuj_klucz(string key);
	void encryption(string str);
	void decryption(string file_name);

	string getSzyfrogram();
};
