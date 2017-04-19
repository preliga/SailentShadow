#include "Sosemanuk.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

using namespace std;

Sosemanuk::Sosemanuk(string klucz_str, string IV_str, int dlugosc_strumienia_klucza)
{
	this->szyfrogram = "";

	klucz_str = normalizuj_klucz(klucz_str);

	for (size_t i = 0; i < 8; i++) {
		this->klucz[i] = strtoul(klucz_str.substr(i * 8, 8).c_str(), NULL, 16);
	}

	// normalizowanie IV
	for (size_t i = 0; i < 4; i++)
	{
		this->IV[3 - i] = Little_endian(strtoul(IV_str.substr(i * 8, 8).c_str(), NULL, 16));
	}
	
	this->Create_stream(dlugosc_strumienia_klucza);

	this->key_position = 0;
}

Sosemanuk::~Sosemanuk()
{
}

int Sosemanuk::Rol(unsigned int a, unsigned int b)
{
	unsigned int x = a >> (32 - b);
	unsigned int y = a << b;

	return x ^ y;
}
int Sosemanuk::Ror(unsigned int a, unsigned int b)
{
	unsigned int x = a << (32 - b);
	unsigned int y = a >> b;
	return x ^ y;
}
void Sosemanuk::Funkcja_SBOX(int runda, unsigned int &w0, unsigned int &w1, unsigned int &w2, unsigned int &w3)
{
	runda %= 8;
	const int SBOX[8][16] =
	{
		{ 0x3, 0x8, 0xF, 0x1, 0xA, 0x6, 0x5, 0xB, 0xE, 0xD, 0x4, 0x2, 0x7, 0x0, 0x9, 0xC },
		{ 0xF, 0xC, 0x2, 0x7, 0x9, 0x0, 0x5, 0xA, 0x1, 0xB, 0xE, 0x8, 0x6, 0xD, 0x3, 0x4 },
		{ 0x8, 0x6, 0x7, 0x9, 0x3, 0xC, 0xA, 0xF, 0xD, 0x1, 0xE, 0x4, 0x0, 0xB, 0x5, 0x2 },
		{ 0x0, 0xF, 0xB, 0x8, 0xC, 0x9, 0x6, 0x3, 0xD, 0x1, 0x2, 0x4, 0xA, 0x7, 0x5, 0xE },
		{ 0x1, 0xF, 0x8, 0x3, 0xC, 0x0, 0xB, 0x6, 0x2, 0x5, 0x4, 0xA, 0x9, 0xE, 0x7, 0xD },
		{ 0xF, 0x5, 0x2, 0xB, 0x4, 0xA, 0x9, 0xC, 0x0, 0x3, 0xE, 0x8, 0xD, 0x6, 0x7, 0x1 },
		{ 0x7, 0x2, 0xC, 0x5, 0x8, 0x4, 0x6, 0xB, 0xE, 0x9, 0x1, 0xF, 0xD, 0x3, 0xA, 0x0 },
		{ 0x1, 0xD, 0xF, 0x0, 0xE, 0x8, 0x2, 0xB, 0x7, 0x4, 0xC, 0xA, 0x9, 0x3, 0x5, 0x6 },
	};

	unsigned int temp_w0 = w0;
	unsigned int temp_w1 = w1;
	unsigned int temp_w2 = w2;
	unsigned int temp_w3 = w3;

	w0 = w1 = w2 = w3 = 0;
	int i = 0;
	for (i = 0; i < 32; i++)
	{
		int a = (temp_w0 & 1) ^ ((temp_w1 << 1) & 2) ^ ((temp_w2 << 2) & 4) ^ ((temp_w3 << 3) & 8);

		int temp_s = SBOX[runda][a];

		w0 ^= (temp_s & 1) << i;
		w1 ^= ((temp_s >> 1) & 1) << i;
		w2 ^= ((temp_s >> 2) & 1) << i;
		w3 ^= ((temp_s >> 3) & 1) << i;

		temp_w0 >>= 1;
		temp_w1 >>= 1;
		temp_w2 >>= 1;
		temp_w3 >>= 1;

	}

}

unsigned int* Sosemanuk::Key_schedule()
{
	unsigned int *subkey = new unsigned int[100];

	unsigned int w[108] = { 0 };

	w[0] = this->klucz[7];
	w[1] = this->klucz[6];
	w[2] = this->klucz[5];
	w[3] = this->klucz[4];
	w[4] = this->klucz[3];
	w[5] = this->klucz[2];
	w[6] = this->klucz[1];
	w[7] = this->klucz[0];

	int i = 0;

	for (i = 8; i < 108; i++)
	{

		w[i] = Rol(w[i - 8] ^ w[i - 5] ^ w[i - 3] ^ w[i - 1] ^ 0x9e3779b9 ^ (i - 8), 11);

		subkey[i - 8] = w[i];

	}

	for (i = 0; i < 25; i++)
	{
		Funkcja_SBOX((32 + 3 - i) % 32, subkey[4 * i], subkey[4 * i + 1], subkey[4 * i + 2], subkey[4 * i + 3]);
	}

	return subkey;
}

unsigned int * Sosemanuk::IV_injection(unsigned int* subkey)
{
	unsigned int *output = new unsigned int[12];

	unsigned int blok[4];

	blok[0] = this->IV[3];
	blok[1] = this->IV[2];
	blok[2] = this->IV[1];
	blok[3] = this->IV[0];

	int k = 0;

	for (int i = 0; i < 24; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			blok[j] ^= subkey[i * 4 + j];
		}



		unsigned int a = blok[0];
		unsigned int b = blok[1];
		unsigned int c = blok[2];
		unsigned int d = blok[3];


		///  SBOX
		this->Funkcja_SBOX(i, a, b, c, d);

		///  Funkcja Liniowa
		//  a <<< 13
		a = Rol(a, 13);
		//  c <<< 3
		c = Rol(c, 3);
		// b = b + c + a
		b ^= c ^ a;
		// d = c + a << 3 + d
		d ^= c ^ (a << 3);
		// b <<< 1
		b = Rol(b, 1);
		// d <<< 7
		d = Rol(d, 7);
		// a = b + d
		a ^= b ^ d;
		// c = c + b << 7 + d
		c ^= d ^ (b << 7);
		// a <<< 5
		a = Rol(a, 5);
		// c <<< 22
		c = Rol(c, 22);
		///

		blok[0] = a;
		blok[1] = b;
		blok[2] = c;
		blok[3] = d;



		if (i == 23)
		{
			for (int j = 0; j < 4; j++)
			{
				blok[j] ^= subkey[(i + 1) * 4 + j];
			}
		}

		if (i == 11 || i == 17 || i == 23)
		{
			for (int j = 0; j < 4; j++)
			{
				output[k * 4 + j] = blok[j];
			}
			k++;
		}

	}

	return output;
}

unsigned int Sosemanuk::Little_endian(unsigned int val)
{
	unsigned int a = (val & 0xFF000000) >> 24;
	unsigned int b = (val & 0xFF0000) >> 8;
	unsigned int c = (val & 0xFF00) << 8;
	unsigned int d = (val & 0xFF) << 24;

	return a ^ b ^ c ^ d;
}

unsigned int* Sosemanuk::Make_stream(unsigned int* lfsr, unsigned int* fsmR, unsigned int* mulAlpha, unsigned int* divAlpha)
{

	/// LFSR & FSM
	unsigned int fsm_klucz[4] = { 0 };
	unsigned int lfsr_klucz[4] = { 0 };
	int i = 0;
	for (i = 0; i < 4; i++)
	{

		/// FSM
		unsigned int x = fsmR[0];
		fsmR[0] = fsmR[1] + (lfsr[1] ^ ((fsmR[0] & 0x01) != 0 ? lfsr[8] : 0));
		fsmR[1] = Rol(x * 0x54655307, 7);

		fsm_klucz[i] = (lfsr[9] + fsmR[0]) ^ fsmR[1];

		/// LFSR
		unsigned int v1 = lfsr[9];
		unsigned int v2 = (lfsr[3] >> 8) ^ divAlpha[lfsr[3] & 0xFF];
		unsigned int v3 = (lfsr[0] << 8) ^ mulAlpha[lfsr[0] >> 24];
		lfsr_klucz[i] = lfsr[0];

		int j = 0;
		for (j = 0; j < 9; j++) {
			lfsr[j] = lfsr[j + 1];
		}

		lfsr[9] = v1 ^ v2 ^ v3;
		///
	}
	
	//cout << "FMS_KLUCZ: " << fsm_klucz[3] << " " << fsm_klucz[2] << " " << fsm_klucz[1] << " " << fsm_klucz[0] << endl;
	unsigned int f0 = fsm_klucz[0];
	unsigned int f1 = fsm_klucz[1];
	unsigned int f2 = fsm_klucz[2];
	unsigned int f3 = fsm_klucz[3];
	unsigned int f4 = f0;
	f0 &= f2;
	f0 ^= f3;
	f2 ^= f1;
	f2 ^= f0;
	f3 |= f4;
	f3 ^= f1;
	f4 ^= f2;
	f1 = f3;
	f3 |= f4;
	f3 ^= f0;
	f0 &= f1;
	f4 ^= f0;
	f1 ^= f3;
	f1 ^= f4;
	f4 = ~f4;
	fsm_klucz[0] = f4;
	fsm_klucz[1] = f1;
	fsm_klucz[2] = f3;
	fsm_klucz[3] = f2;

	unsigned int *output_stream = new unsigned int[4];

	output_stream[0] = Little_endian(fsm_klucz[3] ^ lfsr_klucz[0]);
	output_stream[1] = Little_endian(fsm_klucz[2] ^ lfsr_klucz[1]);
	output_stream[2] = Little_endian(fsm_klucz[1] ^ lfsr_klucz[2]);
	output_stream[3] = Little_endian(fsm_klucz[0] ^ lfsr_klucz[3]);

	return output_stream;
}

string Sosemanuk::normalizuj_klucz(string key)
{
	if (key.length() % 2 == 1) key += "0";
	string key_temp = "";

	for (size_t i = 0; i < key.length(); i += 2) {
		key_temp = key.substr(i, 1) + key.substr(i + 1, 1) + key_temp;
	}

	key_temp = "1" + key_temp;
	while ((key_temp.length() * 4) < 256) {
		key_temp = "0" + key_temp;
	}

	return key_temp;

}


void Sosemanuk::Create_stream(int dlugosc_strumienia_klucza)
{
	dlugosc_strumienia_klucza += dlugosc_strumienia_klucza % 4;

	unsigned int *subkey = Key_schedule();

	unsigned int *output = IV_injection(subkey);

	unsigned int lsfr[10] = { output[11], output[10], output[9], output[8], output[5], output[7], output[3], output[2], output[1], output[0] };

	unsigned int fsmR[2] = { output[4] , output[6] };

	/// ALFA
	unsigned int *mulAlpha = new unsigned int[256];
	unsigned int *divAlpha = new unsigned int[256];

	unsigned int *expb = new unsigned int[256];
	for (int i = 0, x = 0x01; i < 0xFF; i++) {
		expb[i] = x;
		x <<= 1;
		if (x > 0xFF)
			x ^= 0x1A9;
	}

	expb[0xFF] = 0x00;
	unsigned int *logb = new unsigned int[256];
	for (int i = 0; i < 0x100; i++)
		logb[expb[i]] = i;

	mulAlpha[0x00] = 0x00000000;
	divAlpha[0x00] = 0x00000000;
	for (int x = 1; x < 0x100; x++) {
		int ex = logb[x];
		mulAlpha[x] = (expb[(ex + 23) % 255] << 24)
			| (expb[(ex + 245) % 255] << 16)
			| (expb[(ex +  48) % 255] << 8)
			|  expb[(ex + 239) % 255];
		divAlpha[x] = (expb[(ex + 16) % 255] << 24)
			| (expb[(ex + 39) % 255] << 16)
			| (expb[(ex +  6) % 255] << 8)
			|  expb[(ex + 64) % 255];
	}
	///

	unsigned int *output_stream = NULL;
	this->klucz_szyfrujacy = new unsigned char[dlugosc_strumienia_klucza * 4];

	for (int i = 0; i < dlugosc_strumienia_klucza /4; i++) {
		output_stream = Make_stream(lsfr, fsmR, mulAlpha, divAlpha);

		for (int j = 0; j < 4; j++) {

			this->klucz_szyfrujacy[3 + j * 4 + i * 16] = (output_stream[j]);
			this->klucz_szyfrujacy[2 + j * 4 + i * 16] = (output_stream[j] & 0x0000FF00) >> 8;
			this->klucz_szyfrujacy[1 + j * 4 + i * 16] = (output_stream[j] & 0x00FF0000) >> 16;
			this->klucz_szyfrujacy[0 + j * 4 + i * 16] = (output_stream[j] & 0xFF000000) >> 24;
		}
	}	
	/*
	cout << "KLUCZ SZyFRUJACY :: ";
	for(int i = 0; i < dlugosc_strumienia_klucza * 4; i++)
	{
		cout << (int)this->klucz_szyfrujacy[i] << " ";
	}
	cout << endl << endl;
	*/
}

string Sosemanuk::getSzyfrogram()
{
	return this->szyfrogram;
}

void Sosemanuk::encryption(string str)
{
	for each (char znak in str)
	{
		unsigned char dd = (unsigned char)(znak ^ this->klucz_szyfrujacy[this->key_position]);
		
		this->szyfrogram += dd;
		this->key_position++;
	}
	//cout << endl << "Szyfrogram: " <<  this->szyfrogram << endl;
}

string PL(unsigned char text)
{
	string result;
	switch (text)
		{
		case 'π': result += static_cast < unsigned char >(0xa5); break;
		case 'Ê': result += static_cast < unsigned char >(0x86); break;
		case 'Í': result += static_cast < unsigned char >(0xa9); break;
		case '≥': result += static_cast < unsigned char >(0x88); break;
		case 'Ò': result += static_cast < unsigned char >(0xe4); break;
		case 'Û': result += static_cast < unsigned char >(0xa2); break;
		case 'ú': result += static_cast < unsigned char >(0x98); break;
		case 'ø': result += static_cast < unsigned char >(0xbe); break;
		case 'ü': result += static_cast < unsigned char >(0xab); break;
		case '•': result += static_cast < unsigned char >(0xa4); break;
		case '∆': result += static_cast < unsigned char >(0x8f); break;
		case ' ': result += static_cast < unsigned char >(0xa8); break;
		case '£': result += static_cast < unsigned char >(0x9d); break;
		case '—': result += static_cast < unsigned char >(0xe3); break;
		case '”': result += static_cast < unsigned char >(0xe0); break;
		case 'å': result += static_cast < unsigned char >(0x97); break;
		case 'Ø': result += static_cast < unsigned char >(0xbd); break;
		case 'è': result += static_cast < unsigned char >(0x8d); break;
		default: result += text; break;
		}
	return result;
}

void Sosemanuk::decryption(string file_name)
{
	ifstream file;

	file.open(file_name, ios::in||ios::binary);

	int position = 0;
	if(file.good())
	{
		while(!file.eof())
		{
			unsigned char temp;
			file >> temp;
			
			if (!file.eof())
			{
				unsigned char dd = temp ^ this->klucz_szyfrujacy[position++];

				cout << PL(dd);
			}

		}
	}
	else
	{
		cout << "BLAD ODCZYTU PLIKU" << endl;
	}
}