#include "iqtipr/CSearchGuiComp.h"


#include "istd/TChangeNotifier.h"

#include "iqtipr/CSearchResultShape.h"


namespace iqtipr
{


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CSearchGuiComp::CreateShapes(int /*sceneId*/, bool /*inactiveOnly*/, Shapes& result)
{
	if (m_searchResultsModelCompPtr.IsValid()){
		istd::TDelPtr<iqtipr::CSearchResultShape> shapePtr(new CSearchResultShape());
		if (shapePtr.IsValid()){
			if (m_searchResultsModelCompPtr->AttachObserver(shapePtr.GetPtr())){
				result.PushBack(shapePtr.PopPtr());
			}
		}
	}
}


// protected methods

// reimplemented (iqt::CGuiComponentBase)

void CSearchGuiComp::OnGuiCreated()
{
	bool areParamsEditable = false;
	if (m_paramsSetModelCompPtr.IsValid() && m_paramsSetGuiCompPtr.IsValid() && m_paramsSetObserverCompPtr.IsValid()){
		m_paramsSetModelCompPtr->AttachObserver(m_paramsSetObserverCompPtr.GetPtr());
		m_paramsSetGuiCompPtr->CreateGui(ParamsFrame);

		areParamsEditable = true;
	}

	ParamsFrame->setVisible(m_paramsSetCompPtr.IsValid() && areParamsEditable);
}


void CSearchGuiComp::OnGuiDestroyed()
{
	if (		m_paramsSetModelCompPtr.IsValid() &&
				m_paramsSetObserverCompPtr.IsValid() &&
				m_paramsSetModelCompPtr->IsAttached(m_paramsSetObserverCompPtr.GetPtr())){
		m_paramsSetModelCompPtr->DetachObserver(m_paramsSetObserverCompPtr.GetPtr());
	}

	if (m_paramsSetGuiCompPtr.IsValid() && m_paramsSetGuiCompPtr->IsGuiCreated()){
		m_paramsSetGuiCompPtr->DestroyGui();
	}
}


} // namespace iqtipr


