#include "ibase/CConsoleLogComp.h"


// STL includes
#include <iostream>

// ACF includes
#include "istd/IInformationProvider.h"


namespace ibase
{


// protected methods

// reimplemented (CStreamLogCompBase)

void CConsoleLogComp::WriteText(const QString& text)
{
	std::wcout << text.toLocal8Bit().constData();
}


void CConsoleLogComp::NewLine()
{
	std::wcout << std::endl;
}


} // namespace ibase


