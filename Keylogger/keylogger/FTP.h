#pragma once
#include <windows.h>
#include <wininet.h>
#include <iostream>

using namespace std;

class FTP
{
private:
	char *ftp;
	char *user;
	char *password;

	HINTERNET hFtpSession;
	HINTERNET hInternet;

public:
	FTP(char ftp[], char user[], char password[]);
	bool Connetion();
	void Disconnetion();
	HINTERNET get_connection();
	bool Send(const char localFile[],const char remoteFile[]);

	bool CreateDir(string path);

	bool download_file(string path_what, string path_where);

	//bool fileExist(string path);
	bool removeFile(string path);

	void CloseFtpSession();
	void CloseInternet();
	
	~FTP();
};

