#include "iwiz/CWizardDocumentComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iproc/IStateController.h"


namespace iwiz
{


// reimplemented (iwiz::IParamsManagerWizard)

bool CWizardDocumentComp::IsPageSelectionAllowed(int pageIndex) const
{
	if ((pageIndex < 0) || (pageIndex >= GetParamsSetsCount())){
		return false;
	}

	int currentPageIndex = GetSelectedOptionIndex();

	const iproc::IStateController* destPageInfoPtr = CompCastPtr<const iproc::IStateController>(GetParamsSet(pageIndex));
	if ((destPageInfoPtr != NULL) && !destPageInfoPtr->IsStateEnabled()){
		return false;
	}

	for (int checkPageIndex = currentPageIndex; checkPageIndex < pageIndex; ++checkPageIndex){
		const iproc::IStateController* checkedPageInfoPtr = CompCastPtr<const iproc::IStateController>(GetParamsSet(checkPageIndex));
		if ((checkedPageInfoPtr != NULL) && checkedPageInfoPtr->IsStateEnabled() && !checkedPageInfoPtr->IsLeaveAllowed()){
			return false;
		}
	}

	return true;
}


// reimplemented (iwiz::IWizardNavigationController)

bool CWizardDocumentComp::IsFinishAllowed() const
{
	int pagesCount = GetParamsSetsCount();

	for (int checkPageIndex = 0; checkPageIndex < pagesCount; ++checkPageIndex){
		const iproc::IStateController* checkedPageInfoPtr = CompCastPtr<const iproc::IStateController>(GetParamsSet(checkPageIndex));
		if ((checkedPageInfoPtr != NULL) && checkedPageInfoPtr->IsStateEnabled() && !checkedPageInfoPtr->IsLeaveAllowed()){
			return false;
		}
	}

	return true;
}


int CWizardDocumentComp::GetPrevPageIndex() const
{
	int currentPageIndex = GetSelectedOptionIndex();

	for (int checkPageIndex = currentPageIndex - 1; checkPageIndex >= 0; --checkPageIndex){
		const iproc::IStateController* checkedPageInfoPtr = CompCastPtr<const iproc::IStateController>(GetParamsSet(checkPageIndex));
		if ((checkedPageInfoPtr == NULL) || checkedPageInfoPtr->IsStateEnabled()){
			return checkPageIndex;
		}
	}

	return -1;
}


int CWizardDocumentComp::GetNextPageIndex() const
{
	int pagesCount = GetParamsSetsCount();
	int currentPageIndex = GetSelectedOptionIndex();

	const iproc::IStateController* currentPageInfoPtr = CompCastPtr<const iproc::IStateController>(GetParamsSet(currentPageIndex));
	if ((currentPageInfoPtr != NULL) && !currentPageInfoPtr->IsLeaveAllowed()){
		return -1;
	}

	for (int checkPageIndex = currentPageIndex + 1; checkPageIndex < pagesCount; ++checkPageIndex){
		const iproc::IStateController* checkedPageInfoPtr = CompCastPtr<const iproc::IStateController>(GetParamsSet(checkPageIndex));
		if ((checkedPageInfoPtr == NULL) || checkedPageInfoPtr->IsStateEnabled()){
			return checkPageIndex;
		}
	}

	return -1;
}


} // namespace iwiz


