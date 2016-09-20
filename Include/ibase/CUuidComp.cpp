#include <ibase/CUuidComp.h>


// Qt includes
#include <QtCore/QUuid>


namespace ibase
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CUuidComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetName(QUuid::createUuid().toString());
}


} // namespace ibase


