#ifndef iprm_CWizardPageComp_included
#define iprm_CWizardPageComp_included


#include "imod/CMultiModelDispatcherBase.h"

#include "iprm/IWizardPageInfo.h"
#include "iprm/IWizardPageController.h"
#include "iprm/CComposedParamsSetComp.h"


namespace iprm
{


/**
	Component implementation of a wizard page.
*/
class CWizardPageComp:
			public iprm::CComposedParamsSetComp,
			protected imod::CMultiModelDispatcherBase,
			virtual public iprm::IWizardPageInfo
{
public:
	typedef iprm::CComposedParamsSetComp BaseClass;
	typedef imod::CMultiModelDispatcherBase BaseClass2;

	I_BEGIN_COMPONENT(CWizardPageComp);
		I_REGISTER_INTERFACE(iprm::IWizardPageInfo);
		I_ASSIGN(m_pageControllerCompPtr, "PageController", "Wizard page controller", false, "PageController");
	I_END_COMPONENT;

	CWizardPageComp();

	// reimplemented (iprm::IWizardPageInfo)
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

	I_REF(iprm::IWizardPageController, m_pageControllerCompPtr);
};


} // namespace iprm


#endif // !iprm_CWizardPageComp_included


