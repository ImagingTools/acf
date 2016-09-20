#include <iqt2d/CSelectableSceneExtenderComp.h>


// ACF includes
#include <imod/IModel.h>
#include <imod/IObserver.h>


namespace iqt2d
{


CSelectableSceneExtenderComp::CSelectableSceneExtenderComp()
	:m_currentSelectedIndex(-1)	
{
}


// reimplemented (imod::IObserver)

bool CSelectableSceneExtenderComp::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	if (BaseClass2::OnModelAttached(modelPtr, changeMask)){
		AttachCurrent();

		return true;
	}

	return false;
}


bool CSelectableSceneExtenderComp::OnModelDetached(imod::IModel* modelPtr)
{
	DetachCurrent();

	return BaseClass2::OnModelDetached(modelPtr);
}


// reimplemented (iqt2d::IViewExtender)

void CSelectableSceneExtenderComp::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	Q_ASSERT(providerPtr != NULL);

	m_connectedSceneFlags[providerPtr] = flags;

	iprm::ISelectionParam* objectPtr = GetObservedObject();
	if (objectPtr == NULL){
		return;
	}

	int currentIndex = objectPtr->GetSelectedOptionIndex();

	if ((currentIndex >= 0) && (currentIndex < m_extendersCompPtr.GetCount())){
		iqt2d::IViewExtender* extenderPtr = m_extendersCompPtr[currentIndex];
		if (extenderPtr != NULL){
			extenderPtr->AddItemsToScene(providerPtr, flags);
		}
	}
}


void CSelectableSceneExtenderComp::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	Q_ASSERT(providerPtr != NULL);

	iprm::ISelectionParam* objectPtr = GetObservedObject();
	if (objectPtr == NULL){
		return;
	}

	int currentIndex = objectPtr->GetSelectedOptionIndex();

	if ((currentIndex >= 0) && (currentIndex < m_extendersCompPtr.GetCount())){
		iqt2d::IViewExtender* extenderPtr = m_extendersCompPtr[currentIndex];
		if (extenderPtr != NULL){
			extenderPtr->RemoveItemsFromScene(providerPtr);
		}
	}

	m_connectedSceneFlags.remove(providerPtr);
}


// protected methods

void CSelectableSceneExtenderComp::AttachCurrent()
{
	iprm::ISelectionParam* objectPtr = GetObservedObject();
	if (objectPtr == NULL){
		return;
	}

	DetachCurrent();

	int currentIndex = objectPtr->GetSelectedOptionIndex();
	m_currentSelectedIndex = currentIndex;

	if (currentIndex >= 0 && currentIndex < m_extendersCompPtr.GetCount()){
		iqt2d::IViewExtender* extenderPtr = m_extendersCompPtr[currentIndex];
		if (extenderPtr != NULL){
			for (		ConnectedSceneFlags::const_iterator sceneIter = m_connectedSceneFlags.begin();
						sceneIter != m_connectedSceneFlags.end();
						++sceneIter){
				iqt2d::IViewProvider* providerPtr = sceneIter.key();
				Q_ASSERT(providerPtr != NULL);

				extenderPtr->AddItemsToScene(providerPtr, sceneIter.value());
			}
		}
	}
}


void CSelectableSceneExtenderComp::DetachCurrent()
{
	if (m_currentSelectedIndex >= 0 && m_currentSelectedIndex < m_extendersCompPtr.GetCount()){
		iqt2d::IViewExtender* extenderPtr = m_extendersCompPtr[m_currentSelectedIndex];
		if (extenderPtr != NULL){
			for (		ConnectedSceneFlags::const_iterator sceneIter = m_connectedSceneFlags.begin();
						sceneIter != m_connectedSceneFlags.end();
						++sceneIter){
				iqt2d::IViewProvider* providerPtr = sceneIter.key();
				Q_ASSERT(providerPtr != NULL);

				extenderPtr->RemoveItemsFromScene(providerPtr);
			}
		}
	}
}


// reimplemented (imod::CSingleModelObserverBase)

void CSelectableSceneExtenderComp::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	if (changeSet.Contains(iprm::ISelectionParam::CF_SELECTION_CHANGED)){
		DetachCurrent();
		AttachCurrent();
	}

	BaseClass2::AfterUpdate(modelPtr, changeSet);
}


} // namespace iqt2d


