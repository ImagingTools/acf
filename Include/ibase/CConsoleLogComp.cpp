#include "ibase/CConsoleLogComp.h"


// STL includes
#include <iostream>

// ACF includes
#include "istd/IInformation.h"


namespace ibase
{


// protected methods

// reimplemented (CStreamLogCompBase)

void CConsoleLogComp::WriteText(const QString& text)
{
	std::wcout << text.toStdWString().c_str();
}


void CConsoleLogComp::NewLine()
{
	std::wcout << std::endl;
}


} // namespace ibase


