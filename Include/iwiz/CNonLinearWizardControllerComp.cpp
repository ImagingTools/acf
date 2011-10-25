#include "iwiz/CNonLinearWizardControllerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iwiz
{


// public methods

int CNonLinearWizardControllerComp::GetPagesCount() const
{
	if (m_wizardPagesCompPtr.IsValid()){
		return m_wizardPagesCompPtr->GetParamsSetsCount();
	}

	return 0;
}


// reimplemented (iwiz::IWizardNavigationController)

int CNonLinearWizardControllerComp::GetNextPageId(int currentPageId)
{
	if (m_wizardPagesCompPtr.IsValid()){
		int nextIndex = currentPageId + 1;

		if (nextIndex < GetPagesCount()){
			return nextIndex;
		}
	}

	return -1;
}


// reimplemented (iwiz::IWizardPageController)

bool CNonLinearWizardControllerComp::UpdateWizardPage(iwiz::IWizardPageInfo& page, imod::IModel& pageDataModel)
{
	return true;
}


} // namespace iwiz


