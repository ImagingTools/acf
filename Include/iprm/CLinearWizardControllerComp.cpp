#include "iprm/CLinearWizardControllerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iprm
{


// public methods

int CLinearWizardControllerComp::GetPagesCount() const
{
	if (m_wizardPagesCompPtr.IsValid()){
		return m_wizardPagesCompPtr->GetParamsSetsCount();
	}

	return 0;
}


// reimplemented (iprm::IWizardNavigationController)

int CLinearWizardControllerComp::GetNextPageId(int currentPageId)
{
	if (m_wizardPagesCompPtr.IsValid()){
		int nextIndex = currentPageId + 1;

		if (nextIndex < GetPagesCount()){
			return nextIndex;
		}
	}

	return -1;
}


} // namespace iprm


