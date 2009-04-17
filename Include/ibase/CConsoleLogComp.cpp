#include "ibase/CConsoleLogComp.h"


// STL includes
#include <iostream>

#include "ibase/IMessage.h"


namespace ibase
{


// public methods

// reimplemented (ibase::IMessageConsumer)

void CConsoleLogComp::AddMessage(IMessage* messagePtr)
{
	if (messagePtr != NULL){
		std::cout << messagePtr->GetText().ToString() << std::endl;
	}
}


} // namespace ibase


