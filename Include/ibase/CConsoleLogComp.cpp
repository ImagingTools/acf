#include "ibase/CConsoleLogComp.h"


// STL includes
#include <iostream>

#include "ibase/IMessage.h"


namespace ibase
{


// protected methods

// reimplemented (CStreamLogCompBase)

void CConsoleLogComp::WriteLine(const istd::CString& text)
{
	std::wcout << text.c_str();
}


void CConsoleLogComp::NewLine()
{
	std::wcout << std::endl;
}


} // namespace ibase


