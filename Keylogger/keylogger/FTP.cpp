
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <wininet.h>
#include <string>
#include <sstream>
#pragma comment (lib, "Wininet.lib")
#include "FTP.h";

using namespace std;

FTP::FTP(char ftp[], char user[], char password[])
{
	this->ftp = ftp;
	this->user = user;
	this->password = password;

	this->hFtpSession = NULL;
	this->hInternet = NULL;
}

bool FTP::Connetion()
{
	if (InternetAttemptConnect(0) == ERROR_SUCCESS) 
	{
		cout << "Internet dostepny, connection..\n" << endl;
	}
	else 
	{ 
		cout << "Internet niedostepny.\n" << endl; 
		return false; 
	}
	
	this->hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (hInternet != NULL)
	{		
		this->hFtpSession = InternetConnect(this->hInternet, this->ftp, INTERNET_DEFAULT_FTP_PORT, this->user, this->password, INTERNET_SERVICE_FTP, 0, 0);
	}
	else 
	{
		return false;
	}
	
	return true;
}

void FTP::Disconnetion()
{
	this->CloseFtpSession();
	this->CloseInternet();
}


bool FTP::Send(const char localFile[],const char remoteFile[])
{	
	if (this->hFtpSession != NULL) 
	{

		if (FtpPutFile(this->hFtpSession, localFile, remoteFile, FTP_TRANSFER_TYPE_BINARY, INTERNET_FLAG_PASSIVE))
		{
			return true;
		}
		else 
		{
			cout << "Blad podczas wysylania pliu. FTP" << endl;
			return false;
		}
	}
	else
	{
		return false;
	}
	
	return false;
}

bool FTP::CreateDir(string path)
{
	if (this->hFtpSession != NULL)
	{
		
		if (FtpCreateDirectoryA(this->hFtpSession, path.c_str() ))
		{
			return true;
		}
		else
		{
			cout << "Blad podczas tworzenia katalogu lub ju¿ taki istnieje" << endl;
			return false;
		}
	}
	else
	{
		return false;
	}

	return false;
}
/*
bool FTP::fileExist(string path)
{
	if (this->hFtpSession != NULL)
	{
		Ftp
		if (FtpOpenFileA(this->hFtpSession, path.c_str(), GENERIC_READ, INTERNET_FLAG_TRANSFER_BINARY, NULL))
		{
			cout << "TRUE" << endl;
			return true;
		}
		else
		{
			cout << "BRAK PLIKU "<< path << endl;
			return false;
		}
	}
	else
	{
		return false;
	}

	return false;
}
*/
bool FTP::removeFile(string path)
{
	if (this->hFtpSession != NULL)
	{

		if (FtpDeleteFile(this->hFtpSession, path.c_str()))
		{
			//cout << "TRUE" << endl;
			return true;
		}
		else
		{
			//cout << "BRAK PLIKU " << path << endl;
			return false;
		}
	}
	else
	{
		return false;
	}

	return false;
}

bool FTP::download_file(string path_what, string path_where)
{
	if (this->hFtpSession != NULL)
	{
		if (FtpGetFile(hFtpSession, path_what.c_str(), path_where.c_str(), FALSE, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY, 0))
		{
			cout << "TRUE" << endl;
			return true;
		}
		else
		{
			cout << "FALSE " << path_what << endl;
			return false;
		}
	}
	else
	{
		return false;
	}

	return false;
}

HINTERNET FTP::get_connection()
{
	return this->hFtpSession;
}

void FTP::CloseFtpSession()
{
	InternetCloseHandle(this->hFtpSession);
}

void FTP::CloseInternet()
{
	InternetCloseHandle(this->hInternet);
}

FTP::~FTP()
{

}
