#ifndef iqtproc_CProgressManagerGuiComp_included
#define iqtproc_CProgressManagerGuiComp_included


// ACF includes
#include "iqtgui/TDesignerGuiCompBase.h"

#include "iproc/CDelegatedProgressManager.h"

#include "iqtproc/Generated/ui_CProgressManagerGuiComp.h"


namespace iqtproc
{


class CProgressManagerGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CProgressManagerGuiComp>,
			public iproc::CDelegatedProgressManager
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CProgressManagerGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CProgressManagerGuiComp);
		I_REGISTER_INTERFACE(IProgressManager);
		I_ASSIGN(m_automaticHideAttrPtr, "AutomaticHide", "If true, progress bar will be automatically hidden", true, false);
		I_ASSIGN(m_descriptionAttrPtr, "Description", "Description text show left to progress bar", false, "Progress");
	I_END_COMPONENT;

	CProgressManagerGuiComp();

	// reimplemented (iproc::IProgressManager)
	virtual bool IsCanceled(int sessionId) const;

protected:
	void UpdateVisibleComponents();
	void UpdateProgressBar();

	// reimplemented (istd::IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

protected slots:
	void on_CancelButton_clicked();

private:
	I_ATTR(bool, m_automaticHideAttrPtr);
	I_ATTR(istd::CString, m_descriptionAttrPtr);

	struct ProgressInfo
	{
		double progress;
		bool isCancelable;
	};

	typedef std::map<int, ProgressInfo> ProgressMap;

	ProgressMap m_progressMap;

	int m_nextSessionId;
	double m_progressSum;

	int m_cancelableSessionsCount;
};


} // namespace iqtproc


#endif // !iqtproc_CProgressManagerGuiComp_included


