#include "iqtipr/CSearchGuiComp.h"


#include "istd/TChangeNotifier.h"

#include "iqt/CBitmap.h"

#include "iqtipr/CSearchResultShape.h"

#include "iqt2d/CImageShape.h"


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

		istd::TDelPtr<iqt2d::CImageShape> imageShapePtr(new iqt2d::CImageShape());
		if (imageShapePtr.IsValid()){
			if (m_bitmapModelCompPtr->AttachObserver(imageShapePtr.GetPtr())){
				result.PushBack(imageShapePtr.PopPtr());
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


// protected slots
	
void CSearchGuiComp::on_FindButton_clicked()
{
	DoSearch();
}


// private methods

void CSearchGuiComp::DoSearch()
{
	if (		!m_searchProcessorCompPtr.IsValid() || 
				!m_bitmapAcquisitionCompPtr.IsValid() || 
				!m_searchResultsCompPtr.IsValid() ||
				!m_bitmapCompPtr.IsValid()){
		return;
	}
		
	int retVal = m_bitmapAcquisitionCompPtr->DoProcessing(m_paramsSetCompPtr.GetPtr(), NULL, m_bitmapCompPtr.GetPtr());
	if (retVal == icam::IBitmapAcquisition::TS_OK){
		m_searchProcessorCompPtr->DoProcessing(m_paramsSetCompPtr.GetPtr(), m_bitmapCompPtr.GetPtr(), m_searchResultsCompPtr.GetPtr());
	}
}


} // namespace iqtipr


