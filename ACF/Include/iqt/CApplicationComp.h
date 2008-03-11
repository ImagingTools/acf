#ifndef iqt_CApplicationComp_included
#define iqt_CApplicationComp_included


#include "ibase/IApplication.h"

#include "icomp/CComponentBase.h"

#include "iqt/IGuiObject.h"


namespace iqt
{


class CApplicationComp:
			public icomp::CComponentBase, 
			public ibase::IApplication
{
public:
	typedef icomp::CComponentBase BaseClass;
	I_BEGIN_COMPONENT(CApplicationComp)
		I_REGISTER_INTERFACE(ibase::IApplication)
		I_ASSIGN(m_mainGuiCompPtr, "MainGui", "Gui object shown as main window", "MainGui", true)
		I_ASSIGN(m_splashScreenCompPtr, "SplashScreen", "Splash screen shown before application is launched", "SplashScreen", false)
	I_END_COMPONENT

	// reimplemented (ibase::IApplication)
	virtual int Execute(int argc, char** argv);
	virtual istd::CString GetHelpText() const;

private:
	I_REF(IGuiObject, m_mainGuiCompPtr);
	I_REF(IGuiObject, m_splashScreenCompPtr);
};


} // namespace iqt


#endif // iqt_CApplicationComp_included

