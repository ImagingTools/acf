#include "iqt/CSignalBlocker.h"


namespace iqt
{


CSignalBlocker::CSignalBlocker(QObject* objectPtr)
{	
	m_objectPtr = objectPtr;
	if (m_objectPtr != NULL){
		m_objectPtr->blockSignals(true);
	}
}

CSignalBlocker::~CSignalBlocker()
{
	if (m_objectPtr != NULL){
		m_objectPtr->blockSignals(false);
	}
}


} // namespace iqt
