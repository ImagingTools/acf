#ifndef iqtprm_CWizardGuiComp_included
#define iqtprm_CWizardGuiComp_included


// Qt includes
#include <QWizard>


// ACF includes
#include "imod/TSingleModelObserverBase.h"

#include "iprm/ISelectionParam.h"
#include "iprm/IParamsManager.h"
#include "iprm/IWizardNavigationController.h"

#include "idoc/IHelpViewer.h"

#include "iqtgui/TGuiObserverWrap.h"
#include "iqtgui/TGuiComponentBase.h"


namespace iqtprm
{


/**
	Parameter manager UI implemented as wizard.
*/
class CWizardGuiComp:
			public iqtgui::TGuiObserverWrap<
						iqtgui::TGuiComponentBase<QWizard>,
						imod::TSingleModelObserverBase<iprm::IParamsManager> >
{
	Q_OBJECT

public:
	typedef iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<QWizard>,
				imod::TSingleModelObserverBase<iprm::IParamsManager> > BaseClass;

	I_BEGIN_COMPONENT(CWizardGuiComp);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_REGISTER_INTERFACE(imod::IModelEditor);
		I_ASSIGN(m_wizardControllerCompPtr, "LinearWizardController", "Wizard navigation controller", false, "LinearWizardController");
		I_ASSIGN(m_sideGuiCompPtr, "SideWidget", "Side widget of the wizard", false, "SideWidget");
		I_ASSIGN(m_helpViewerCompPtr, "HelpViewer", "Help view component", false, "HelpViewer");
		I_ASSIGN_MULTI_0(m_editorsCompPtr, "Editors", "List of GUI's for parameters edition", true);
		I_ASSIGN_TO(m_guisCompPtr, m_editorsCompPtr, true);
		I_ASSIGN_TO(m_observersCompPtr, m_editorsCompPtr, true);
		I_ASSIGN_MULTI_0(m_titelsAttrPtr, "PageTitels", "List of page titels", false);
		I_ASSIGN_MULTI_0(m_subTitelsAttrPtr, "PageSubTitels", "List of page sub-titels", false);
		I_ASSIGN(m_wizardStyleAttrPtr, "WizardStyle", "Set the look of the wizard UI\n0 - Classic\n1 - Modern\n2 - MacStyle\n3 - Aero", false, 0);
	I_END_COMPONENT;

protected Q_SLOTS:
	void OnCurrentPageChanged(int pageId);
	void OnHelpRequested();

protected:
	int GetNextPageId() const;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	class CWizardPage: public QWizardPage
	{
	public:
		typedef QWizardPage BaseClass;

		CWizardPage(CWizardGuiComp& parent);

		// reimplemented (QWizardPage)
		virtual int nextId() const;
	
	private:
		CWizardGuiComp& m_parent;
	};

private:
	void GoToPage(int pageIndex);

private:
	I_REF(iprm::IWizardNavigationController, m_wizardControllerCompPtr);
	I_REF(iqtgui::IGuiObject, m_sideGuiCompPtr);
	I_REF(idoc::IHelpViewer, m_helpViewerCompPtr);
	I_MULTIREF(imod::IModelEditor, m_editorsCompPtr);
	I_MULTIREF(iqtgui::IGuiObject, m_guisCompPtr);
	I_MULTIREF(imod::IObserver, m_observersCompPtr);
	I_MULTIATTR(istd::CString, m_titelsAttrPtr);
	I_MULTIATTR(istd::CString, m_subTitelsAttrPtr);
	I_ATTR(int, m_wizardStyleAttrPtr);
};


} // namespace iqtprm


#endif // !iqtprm_CWizardGuiComp_included


