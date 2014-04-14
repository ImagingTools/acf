#include "ilog/CConsoleLogComp.h"


// STL includes
#include <iostream>

// ACF includes
#include "istd/IInformationProvider.h"


namespace ilog
{


// protected methods

// reimplemented (CStreamLogCompBase)

void CConsoleLogComp::WriteText(const QString& text, istd::IInformationProvider::InformationCategory category)
{
	if (category >= istd::IInformationProvider::IC_ERROR){
		std::wcerr << text.toLocal8Bit().constData();
	}
	else{
		std::wcout << text.toLocal8Bit().constData();
	}
}


} // namespace ilog


