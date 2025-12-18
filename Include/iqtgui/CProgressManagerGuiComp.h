#ifndef iqtgui_CProgressManagerGuiComp_included
#define iqtgui_CProgressManagerGuiComp_included


// ACF includes
#include <iqtgui/TDesignerGuiCompBase.h>
#include <ibase/CCumulatedProgressManagerBase.h>

#include <GeneratedFiles/iqtgui/ui_CProgressManagerGuiComp.h>


namespace iqtgui
{


class CProgressManagerGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CProgressManagerGuiComp>,
			public ibase::CCumulatedProgressManagerBase
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CProgressManagerGuiComp> BaseClass;
	typedef ibase::CCumulatedProgressManagerBase BaseClass2;

	I_BEGIN_COMPONENT(CProgressManagerGuiComp);
		I_REGISTER_INTERFACE(IProgressManager);
		I_ASSIGN(m_showCancelAttrPtr, "ShowCancel", "If true, cancel button will be visible", true, true);
		I_ASSIGN(m_automaticHideAttrPtr, "AutomaticHide", "If true, progress bar will be automatically hidden", true, false);
		I_ASSIGN(m_descriptionAttrPtr, "Description", "Description text show left to progress bar", false, "Progress");
	I_END_COMPONENT;

protected:
	// reimplemented (ibase::CCumulatedProgressManagerBase)
	virtual void OnProgressChanged(double cumulatedValue) override;
	virtual void OnTasksChanged() override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;

protected Q_SLOTS:
	void on_CancelButton_clicked();

private:
	I_ATTR(bool, m_showCancelAttrPtr);
	I_ATTR(bool, m_automaticHideAttrPtr);
	I_TEXTATTR(m_descriptionAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CProgressManagerGuiComp_included


