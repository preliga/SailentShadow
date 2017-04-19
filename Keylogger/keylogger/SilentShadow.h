#pragma once
#include "FTP.h"

class SilentShadow
{
private:
	FTP *ftp;
public:
	SilentShadow(char ftp[], char user[], char password[]);
	~SilentShadow();
	void start();
};

