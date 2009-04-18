#include "ibase/CConsoleLogComp.h"


// STL includes
#include <iostream>

#include "ibase/IMessage.h"


namespace ibase
{


// public methods

CConsoleLogComp::CConsoleLogComp()
:	m_isLastDotShown(false)
{
}


// reimplemented (ibase::IMessageConsumer)

void CConsoleLogComp::AddMessage(IMessage* messagePtr)
{
	if (messagePtr != NULL){
		if (messagePtr->GetCategory() >= *m_minPriorityAttrPtr){
			if (m_isLastDotShown){
				std::cout << std::endl;

				m_isLastDotShown = false;
			}

			std::cout << messagePtr->GetText().ToString() << std::endl;
		}
		else if (*m_isDotEnabledAttrPtr){
			std::cout << ".";

			m_isLastDotShown = true;
		}
	}
}


// reimplemented (icomp::IComponent)

void CConsoleLogComp::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();

	if (m_isLastDotShown){
		std::cout << std::endl;

		m_isLastDotShown = false;
	}
}


} // namespace ibase


