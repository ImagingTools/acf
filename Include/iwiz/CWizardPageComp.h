#ifndef iwiz_CWizardPageComp_included
#define iwiz_CWizardPageComp_included


#include "imod/CMultiModelDispatcherBase.h"

#include "iprm/CComposedParamsSetComp.h"

#include "iwiz/IWizardPageInfo.h"
#include "iwiz/IWizardPageController.h"


namespace iwiz
{


/**
	Component implementation of a wizard page.
*/
class CWizardPageComp:
			public iprm::CComposedParamsSetComp,
			protected imod::CMultiModelDispatcherBase,
			virtual public iwiz::IWizardPageInfo
{
public:
	typedef iprm::CComposedParamsSetComp BaseClass;
	typedef imod::CMultiModelDispatcherBase BaseClass2;

	I_BEGIN_COMPONENT(CWizardPageComp);
		I_REGISTER_INTERFACE(iwiz::IWizardPageInfo);
		I_ASSIGN(m_pageControllerCompPtr, "PageController", "Wizard page controller", false, "PageController");
	I_END_COMPONENT;

	CWizardPageComp();

	// reimplemented (iwiz::IWizardPageInfo)
	virtual bool IsPageFinished() const;
	virtual void SetPageFinished(bool isPageFinished = true);

protected:
	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	bool m_isPageFinished;

	I_REF(iwiz::IWizardPageController, m_pageControllerCompPtr);
};


} // namespace iwiz


#endif // !iwiz_CWizardPageComp_included


