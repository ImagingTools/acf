#include <ilog/CConsoleLogComp.h>


// STL includes
#include <iostream>

// ACF includes
#include <istd/IInformationProvider.h>

// Windows includes
#ifdef Q_OS_WIN
#include <windows.h>
#ifndef FOREGROUND_BLUE
	#define FOREGROUND_BLUE	  0x0001 // text color contains blue.
#endif
#ifndef FOREGROUND_GREEN
	#define FOREGROUND_GREEN	 0x0002 // text color contains green.
#endif
#ifndef FOREGROUND_RED
	#define FOREGROUND_RED	   0x0004 // text color contains red.
#endif
#ifndef FOREGROUND_INTENSITY
	#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#endif
extern WINBASEAPI BOOL WINAPI SetConsoleTextAttribute(HANDLE hConsoleOutput, WORD wAttributes);
#endif


namespace ilog
{


// protected methods

// reimplemented (CStreamLogCompBase)

void CConsoleLogComp::WriteText(const QString& text, istd::IInformationProvider::InformationCategory category)
{
	SetConsoleColor(category);

	std::wcout << text.toLocal8Bit().constData();

	SetConsoleColor(istd::IInformationProvider::IC_INFO);
}


// provate methods

void CConsoleLogComp::SetConsoleColor(istd::IInformationProvider::InformationCategory category)
{
	switch (category){
	case istd::IInformationProvider::IC_ERROR:
	case istd::IInformationProvider::IC_CRITICAL:
#ifdef Q_OS_WIN
		SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
		std::wcerr << "\033[33m";
#endif
		break;

	case istd::IInformationProvider::IC_WARNING:
#ifdef Q_OS_WIN
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
		std::wcout << "\033[31m";
#endif
		break;

	default:
#ifdef Q_OS_WIN
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
#else
		std::wcout << "\033[0m";
#endif
		break;
	}
}


} // namespace ilog


