#include "iqt/CSignalBlocker.h"


// Qt includes
#include <QObjectList>


namespace iqt
{


CSignalBlocker::CSignalBlocker(QObject* objectPtr, bool blockChilds)
{	
	m_objectPtr = objectPtr;
	m_blockChilds = blockChilds;
	if (m_objectPtr != NULL){
		m_objectPtr->blockSignals(true);

		if (blockChilds){
			BlockChilds(m_objectPtr, true);
		}
	}
}

CSignalBlocker::~CSignalBlocker()
{
	if (m_objectPtr != NULL){
		m_objectPtr->blockSignals(false);

		if (m_blockChilds){
			BlockChilds(m_objectPtr, false);
		}
	}
}


// private methods

void CSignalBlocker::BlockChilds(QObject* objectPtr, bool block)
{
	I_ASSERT(m_objectPtr != NULL);

	QObjectList childs = objectPtr->children();
	QObject* childObjectPtr = NULL;
	foreach(childObjectPtr, childs){
		childObjectPtr->blockSignals(block);

		BlockChilds(childObjectPtr, block);
	}
}


} // namespace iqt


