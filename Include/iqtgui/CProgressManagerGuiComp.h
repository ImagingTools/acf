#ifndef iqtgui_CProgressManagerGuiComp_included
#define iqtgui_CProgressManagerGuiComp_included


// ACF includes
#include "iqtgui/TDesignerGuiCompBase.h"

#include "iproc/CDelegatedProgressManager.h"

#include "iqtgui/Generated/ui_CProgressManagerGuiComp.h"


namespace iqtgui
{


class CProgressManagerGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CProgressManagerGuiComp>,
			public iproc::CDelegatedProgressManager
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CProgressManagerGuiComp> BaseClass;
	typedef iproc::CDelegatedProgressManager BaseClass2;

	I_BEGIN_COMPONENT(CProgressManagerGuiComp);
		I_REGISTER_INTERFACE(IProgressManager);
		I_ASSIGN(m_showCancelAttrPtr, "ShowCancel", "If true, cancel button will be visible", true, true);
		I_ASSIGN(m_automaticHideAttrPtr, "AutomaticHide", "If true, progress bar will be automatically hidden", true, false);
		I_ASSIGN(m_descriptionAttrPtr, "Description", "Description text show left to progress bar", false, "Progress");
	I_END_COMPONENT;

	CProgressManagerGuiComp();

protected:
	void UpdateVisibleComponents();
	void UpdateProgressBar();

	// reimplemented (iproc::CDelegatedProgressManager)
	void OnCancelable(bool cancelState);

	// reimplemented (ibase::IProgressManager)
	virtual int BeginProgressSession(
				const QByteArray& progressId,
				const QString& description,
				bool isCancelable);
	virtual bool IsCanceled(int sessionId) const;

	// reimplemented (istd::IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

protected Q_SLOTS:
	void on_CancelButton_clicked();

private:
	I_ATTR(bool, m_showCancelAttrPtr);
	I_ATTR(bool, m_automaticHideAttrPtr);
	I_ATTR(QString, m_descriptionAttrPtr);

	bool m_isCanceled;

	bool m_isCancelable;
};


} // namespace iqtgui


#endif // !iqtgui_CProgressManagerGuiComp_included


