#include "iqtipr/CSearchParamsGuiComp.h"


#include "istd/TChangeNotifier.h"


namespace iqtipr
{


// reimplemented (imod::IModelEditor)

void CSearchParamsGuiComp::UpdateEditor()
{
	iipr::ISearchParams* paramsPtr = GetObjectPtr();
	if (paramsPtr == NULL){
		return;
	}

	m_spinMinimumAngle->setValue(paramsPtr->GetRotationRange().GetMinValue());
	m_spinMaximumAngle->setValue(paramsPtr->GetRotationRange().GetMaxValue());
	m_spinMinimumScale->setValue(paramsPtr->GetScaleRange().GetMinValue());
	m_spinMaximumScale->setValue(paramsPtr->GetScaleRange().GetMaxValue());
	m_spinMinimumScore->setValue(paramsPtr->GetMinScore());
	m_spinModelNumber->setValue(paramsPtr->GetNominalModelsCount());

	m_checkEnableRotation->setChecked(paramsPtr->IsRotationEnabled());
	m_checkEnableScale->setChecked(paramsPtr->IsScaleEnabled());

	istd::TChangeNotifier<iimg::IBitmap> updatePtr(&m_modelBitmap);
	const iimg::IBitmap* bitmapPtr = &paramsPtr->GetModelImage();
	if (bitmapPtr != NULL){
		updatePtr->CopyImageFrom(*bitmapPtr);
	}
}


void CSearchParamsGuiComp::UpdateModel() const
{
	iipr::ISearchParams* paramsPtr = GetObjectPtr();
	if (paramsPtr == NULL || !IsGuiCreated()){
		return;
	}

	if (!IsUpdateBlocked()){
		UpdateBlocker blocker(const_cast<CSearchParamsGuiComp*>(this));

		istd::TChangeNotifier<iipr::ISearchParams> updatePtr(paramsPtr);

		updatePtr->SetRotationRange(istd::CRange(m_spinMinimumAngle->value(), m_spinMaximumAngle->value()));
		updatePtr->SetScaleRange(istd::CRange(m_spinMinimumScale->value(), m_spinMaximumScale->value()));
		updatePtr->SetMinScore(m_spinMinimumScore->value());
		updatePtr->SetNominalModelsCount(m_spinModelNumber->value());
		updatePtr->SetRotationEnabled(m_checkEnableRotation->isChecked());
		updatePtr->SetScaleEnabled(m_checkEnableScale->isChecked());
	}
}


// protected slots

void CSearchParamsGuiComp::OnParameterChanged()
{
	UpdateModel();
}


// protected methods

// reimplemented (iqt::CGuiObjectBase)

void CSearchParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	connect(m_spinMinimumScore, SIGNAL(valueChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_spinModelNumber, SIGNAL(valueChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_spinMinimumAngle, SIGNAL(valueChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_spinMaximumAngle, SIGNAL(valueChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_spinMinimumScale, SIGNAL(valueChanged(double)), this, SLOT(OnParameterChanged()));
	connect(m_spinMaximumScale, SIGNAL(valueChanged(double)), this, SLOT(OnParameterChanged()));
	connect(m_checkEnableRotation, SIGNAL(stateChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_checkEnableScale, SIGNAL(stateChanged(int)), this, SLOT(OnParameterChanged()));

	if (m_modelViewCompPtr.IsValid()){
		m_modelViewCompPtr->CreateGui(ModelFrame);
	}

	if (m_modelObserverCompPtr.IsValid()){
		m_modelBitmap.AttachObserver(m_modelObserverCompPtr.GetPtr());
	}
}


void CSearchParamsGuiComp::OnGuiDestroyed()
{
	if (m_modelViewCompPtr.IsValid()){
		m_modelViewCompPtr->DestroyGui();
	}

	if (m_modelObserverCompPtr.IsValid()){
		m_modelBitmap.DetachObserver(m_modelObserverCompPtr.GetPtr());
	}
}


} // namespace iqtipr


