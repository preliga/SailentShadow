#include <iostream>
#include "Sosemanuk.h"
#include "Blake.h"

using namespace std;


int main(int argc, char **argv)
{
	/*
	if (argc < 2) return 0;

	int number_file = atoi(argv[1]); 
	string base_path = argv[2];
	*/
	int number_file = 2;
	string base_path = "F:\\test\\";



	Blake *blake = new Blake();

	int licznik_paczek = 1;
	string key = "A7C083FEB7";
	string IV = "00112233445566778899AABBCCDDEEFF";

	
	for (int i = 1; i <= number_file; i++)
	{
		cout << "PACKAGE :: " << i << endl;
		Sosemanuk *sosemauk = new Sosemanuk(key, IV, 2000);

		sosemauk->decryption(base_path + "\\" + to_string(i));
		cout << endl << endl;
		key = blake->Blake_256(key).substr(0, 10);
		
		delete(sosemauk);
	}

	getchar();
	return 0;
}


