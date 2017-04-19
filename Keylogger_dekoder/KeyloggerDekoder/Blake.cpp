
#include <string>
#include <iomanip>
#include <iostream>
#include "Blake.h"
#include <fstream>
#include <sstream>
using namespace std;

Blake::Blake()
{

}

unsigned long Blake::bitwise_rotation(unsigned long a, unsigned long b) {
	unsigned long x, y;
	x = a / (int)pow(2, b);
	y = a * (int)pow(2, 32 - b);
	return  (x + y) & 0xFFFFFFFF;
}

void Blake::round_function(unsigned long *m, unsigned long v[4][4], long round_number) {

	unsigned long* G[8][4] =
	{
		{ &v[0][0], &v[1][0], &v[2][0], &v[3][0] },
		{ &v[0][1], &v[1][1], &v[2][1], &v[3][1] },
		{ &v[0][2], &v[1][2], &v[2][2], &v[3][2] },
		{ &v[0][3], &v[1][3], &v[2][3], &v[3][3] },
		{ &v[0][0], &v[1][1], &v[2][2], &v[3][3] },
		{ &v[0][1], &v[1][2], &v[2][3], &v[3][0] },
		{ &v[0][2], &v[1][3], &v[2][0], &v[3][1] },
		{ &v[0][3], &v[1][0], &v[2][1], &v[3][2] }
	};

	for (long i = 0; i < 8; i++)
	{
		unsigned long *a = G[i][0];
		unsigned long *b = G[i][1];
		unsigned long *c = G[i][2];
		unsigned long *d = G[i][3];

		*a = ((*a + (*b + (m[this->SIGMA[round_number % 10][2 * i]] ^ this->C[this->SIGMA[round_number % 10][2 * i + 1]]))) % 0xFFFFFFFF) % 0xFFFFFFFF;
		*d = this->bitwise_rotation((*d ^ *a), 16);

		*c = (*c + *d) % 0xFFFFFFFF;
		*b = this->bitwise_rotation((*b ^ *c), 12);

		*a = ((*a + (*b + (m[this->SIGMA[round_number % 10][2 * i + 1]] ^ this->C[this->SIGMA[round_number % 10][2 * i]]))) % 0xFFFFFFFF) % 0xFFFFFFFF;
		*d = this->bitwise_rotation((*d ^ *a), 8);

		*c = (*c + *d) % 0xFFFFFFFF;
		*b = this->bitwise_rotation((*b ^ *c), 7);
	}
}

void Blake::hashing_message(unsigned long *m, unsigned long *H) {

	unsigned long v[4][4] =
	{
		{ H[0], H[1], H[2], H[3] },
		{ H[4], H[5], H[6], H[7] },
		{ this->C[0], this->C[1], this->C[2], this->C[3] },
		{ this->T ^ this->C[4], this->T ^ this->C[5], this->C[6], this->C[7], }
	};

	for (long i = 0; i < 14; i++)
	{
		this->round_function(m, v, i);
	}

	H[0] ^= v[0][0] ^ v[2][0];
	H[1] ^= v[0][1] ^ v[2][1];
	H[2] ^= v[0][2] ^ v[2][2];
	H[3] ^= v[0][3] ^ v[2][3];
	H[4] ^= v[1][0] ^ v[3][0];
	H[5] ^= v[1][1] ^ v[3][1];
	H[6] ^= v[1][2] ^ v[3][2];
	H[7] ^= v[1][3] ^ v[3][3];

}

void Blake::transform(string keyIn, unsigned long *keyOut, int klucz) {

	unsigned int Heks1[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
	unsigned int Heks2[16] = { 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0 };

	int a, b;
	for (int i = 0, j = 0; i < klucz; i++, j += 2)
	{

		if (keyIn[j] == 'a' || keyIn[j] == 'A') a = 0xA;
		else if (keyIn[j] == 'b' || keyIn[j] == 'B') a = 0xB;
		else if (keyIn[j] == 'c' || keyIn[j] == 'C') a = 0xC;
		else if (keyIn[j] == 'd' || keyIn[j] == 'D') a = 0xD;
		else if (keyIn[j] == 'e' || keyIn[j] == 'E') a = 0xE;
		else if (keyIn[j] == 'f' || keyIn[j] == 'F') a = 0xF;
		else a = (int)keyIn[j] - 0x30;

		if (keyIn[j + 1] == 'a' || keyIn[j + 1] == 'A') b = 0xA;
		else if (keyIn[j + 1] == 'b' || keyIn[j + 1] == 'B') b = 0xB;
		else if (keyIn[j + 1] == 'c' || keyIn[j + 1] == 'C') b = 0xC;
		else if (keyIn[j + 1] == 'd' || keyIn[j + 1] == 'D') b = 0xD;
		else if (keyIn[j + 1] == 'e' || keyIn[j + 1] == 'E') b = 0xE;
		else if (keyIn[j + 1] == 'f' || keyIn[j + 1] == 'F') b = 0xF;
		else b = (int)keyIn[j + 1] - 0x30;

		*keyOut <<= 8;
		*keyOut += Heks2[a] + Heks1[b];
	}
}

string Blake::normal(string message)
{
	string hash = "";

	for (int i = 0; i < message.length(); i++)
	{
		ostringstream ss;
		ss << hex << uppercase << setw(2) << setfill('0') << (int)message[i];
		string str = ss.str();

		hash += str;
	}

	return hash;
}

string Blake::Blake_256(string message)
{

	unsigned char SIGMA[10][16] =
	{
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
		{ 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 },
		{ 11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 },
		{ 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 },
		{ 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 },
		{ 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 },
		{ 12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11 },
		{ 13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 },
		{ 6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5 },
		{ 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0 },
	};

	for (int i = 0; i < 10; i++)
	{
		this->SIGMA[i] = SIGMA[i];
	}

	unsigned long C[16] =
	{
		0x243F6A88, 0x85A308D3,
		0x13198A2E, 0x03707344,
		0xA4093822, 0x299F31D0,
		0x082EFA98, 0xEC4E6C89,
		0x452821E6, 0x38D01377,
		0xBE5466CF, 0x34E90C6C,
		0xC0AC29B7, 0xC97C50DD,
		0x3F84D5B5, 0xB5470917
	};

	this->C = C;
	this->T = 0;
	unsigned long H[8] =
	{
		0x6A09E667, 0xBB67AE85,
		0x3C6EF372, 0xA54FF53A,
		0x510E527F, 0x9B05688C,
		0x1F83D9AB, 0x5BE0CD19
	};

	string msg = this->normal(message);

	int size_message = (int)msg.length() * 4;
	int amount_bloks = (size_message / 512) + 1;

	if (512 - (size_message % 512) < 67)
	{
		amount_bloks++;
	}

	msg += "8";
	while (((int)msg.length() * 4) < 512 * amount_bloks - 70)
	{
		msg += "0"; // 70 = 64 + 8 - 2;
	}
	msg += "1";


	for (int i = 0; i < amount_bloks; i++)
	{
		unsigned long m[16] = { 0 };

		if (size_message - this->T > 512)
		{
			this->T += 512;
		}
		else
		{
			if (size_message == this->T)
			{
				this->T = 0;
			}
			else
			{
				this->T = size_message;
			}
		}

		int bloks_counter = 16;
		if (i == amount_bloks - 1)
		{
			bloks_counter = 14;
			m[14] = 0;
			m[15] = size_message;
		}

		for (int j = 0; j < bloks_counter; j++)
		{
			this->transform(msg, &m[j], 4);
			msg = msg.substr(8);
		}

		this->hashing_message(m, H);
	}

	string hash = "";

	for (int i = 0;i < 8;i++)
	{
		ostringstream ss;
		ss << hex << uppercase << setw(8) << setfill('0') << H[i];
		string str = ss.str();

		hash += str;
	}

	return hash;

}
