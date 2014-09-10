#include "ilog/CConsoleLogComp.h"


// STL includes
#include <iostream>

// ACF includes
#include "istd/IInformationProvider.h"

// Windows includes
#ifdef Q_OS_WIN
	#include <windows.h>
	#define FOREGROUND_BLUE	  0x0001 // text color contains blue.
	#define FOREGROUND_GREEN	 0x0002 // text color contains green.
	#define FOREGROUND_RED	   0x0004 // text color contains red.
	#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
	extern WINBASEAPI BOOL WINAPI SetConsoleTextAttribute(HANDLE hConsoleOutput, WORD wAttributes);
#endif


enum TextColors
{
	TC_DEFAULT,
	TC_RED,
	TC_YELLOW
};


void SetConsoleColor(TextColors color)
{
	switch (color){
		case TC_DEFAULT:
			#ifdef Q_OS_WIN
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
			#else
			std::wcout << "\033[0m";
			#endif
			break;

		case TC_RED:
			#ifdef Q_OS_WIN
			SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
			#else
			std::wcerr << "\033[33m";
			#endif
			break;

		case TC_YELLOW:
			#ifdef Q_OS_WIN
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
			#else
			std::wcout << "\033[31m";
			#endif
			break;

	}
}


namespace ilog
{


// protected methods

// reimplemented (CStreamLogCompBase)

void CConsoleLogComp::WriteText(const QString& text, istd::IInformationProvider::InformationCategory category)
{
	if (category >= istd::IInformationProvider::IC_ERROR){
		SetConsoleColor(TC_RED);

		std::wcerr << text.toLocal8Bit().constData();
	}
	else if (category == istd::IInformationProvider::IC_WARNING){
		SetConsoleColor(TC_YELLOW);

		std::wcout << text.toLocal8Bit().constData();
	}
	else{
		SetConsoleColor(TC_DEFAULT);

		std::wcout << text.toLocal8Bit().constData();
	}

	SetConsoleColor(TC_DEFAULT);
}


} // namespace ilog


