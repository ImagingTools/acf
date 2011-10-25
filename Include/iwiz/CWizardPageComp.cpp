#include "iwiz/CWizardPageComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iwiz
{


// public methods

CWizardPageComp::CWizardPageComp()
	:m_isPageFinished(false)
{
}


// reimplemented (iwiz::IWizardPageInfo)

bool CWizardPageComp::IsPageFinished() const
{
	return m_isPageFinished;
}


void CWizardPageComp::SetPageFinished(bool isPageFinished)
{
	if (m_isPageFinished != isPageFinished){
		istd::CChangeNotifier changePtr(this, CF_PAGE_FINISHED);

		m_isPageFinished = isPageFinished;
	}
}


// protected methods

// reimplemented (imod::CMultiModelDispatcherBase)

void CWizardPageComp::OnModelChanged(int modelId, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (m_pageControllerCompPtr.IsValid()){
		imod::IModel* modelPtr = GetObjectAt<imod::IModel>(modelId);

		I_ASSERT(modelPtr != NULL);

		m_pageControllerCompPtr->UpdateWizardPage(*this, *modelPtr);		
	}
}


// reimplemented (icomp::CComponentBase)

void CWizardPageComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	// if no page controller was set, the page can be immediately marked as finished:
	if (!m_pageControllerCompPtr.IsValid()){
		m_isPageFinished = true;
	}

	const ParameterInfos& parameterInfos = GetParameterInfos();
	for (int parameterIndex = 0; parameterIndex < parameterInfos.GetCount(); parameterIndex++){
		imod::IModel* parameterModelPtr = dynamic_cast<imod::IModel*>(parameterInfos.GetAt(parameterIndex)->parameterPtr.GetPtr());
		if (parameterModelPtr != NULL){
			RegisterModel(parameterModelPtr, parameterIndex);
		}
	}
}


void CWizardPageComp::OnComponentDestroyed()
{
	BaseClass2::UnregisterAllModels();

	BaseClass::OnComponentDestroyed();
}


} // namespace iprm


