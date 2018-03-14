#ifndef iqtgui_CDialogApplicationComp_included
#define iqtgui_CDialogApplicationComp_included


// ACF includes
#include <iqtgui/IDialog.h>
#include <iqtgui/CApplicationCompBase.h>


namespace iqtgui
{


/**
	Standard component for a dialog based GUI application.
*/
class CDialogApplicationComp: public CApplicationCompBase
{
public:
	typedef CApplicationCompBase BaseClass;

	I_BEGIN_COMPONENT(CDialogApplicationComp);
		I_REGISTER_INTERFACE(ibase::IApplication);
		I_ASSIGN(m_mainDialogCompPtr, "MainDialogWindow", "Dialog window shown as main window", true, "MainDialogWindow");
	I_END_COMPONENT;

	// reimplemented (ibase::IApplication)
	virtual int Execute(int argc, char** argv);
	virtual QString GetHelpText() const;

private:
	I_REF(IDialog, m_mainDialogCompPtr);
};


} // namespace iqtgui


#endif // iqtgui_CDialogApplicationComp_included

