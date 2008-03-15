#ifndef OLD_ACF_SUPPORT
#include <QGridLayout>

#include "iqt/CLoginGuiComp.h"


namespace iqt
{


// protected methods

// reimplemented (iqt::CGuiComponentBase)

void CLoginGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();
}


} // namespace iqt


#else

#include <QGridLayout>

#include "iqt/CLoginGuiComp.h"


namespace iqt
{


CLoginGuiComp::CLoginGuiComp()
:	BaseClass()
{

}


CLoginGuiComp::~CLoginGuiComp()
{

}


// protected methods

// reimplemented (acf::QtAbstractGuiComponent)

void CLoginGuiComp::initializeGui()
{

}


} // namespace iqt


#endif // OLD_ACF_SUPPORT

