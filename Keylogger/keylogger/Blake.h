#pragma once
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;
class Blake
{
public:
	unsigned long *C;
	unsigned char *SIGMA[16];
	unsigned long T;

public:
	Blake();
	unsigned long bitwise_rotation(unsigned long a, unsigned long b);
	void round_function(unsigned long *m, unsigned long v[4][4], long round_number);
	void hashing_message(unsigned long *m, unsigned long *H);
	void transform(string keyIn, unsigned long *keyOut, int klucz);
	string Blake_256(string message);
	string normal(string message);
};