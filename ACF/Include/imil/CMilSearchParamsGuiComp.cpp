#include "imil/CMilSearchParamsGuiComp.h"


#include "istd/TChangeNotifier.h"


namespace imil
{


CMilSearchParamsGuiComp::CMilSearchParamsGuiComp()
{
	m_speedMap[0] = imil::CMilSearchParams::VeryHighSpeed;
	m_speedMap[1] = imil::CMilSearchParams::HighSpeed;
	m_speedMap[2] = imil::CMilSearchParams::MediumSpeed;
	m_speedMap[3] = imil::CMilSearchParams::LowSpeed;

	m_accuracyMap[0] = imil::CMilSearchParams::MediumAccuracy;
	m_accuracyMap[1] = imil::CMilSearchParams::HighAccuracy;

	m_detailLevelMap[0] = imil::CMilSearchParams::MediumLevel;
	m_detailLevelMap[1] = imil::CMilSearchParams::HighLevel;
	m_detailLevelMap[2] = imil::CMilSearchParams::VeryHighLevel;

	m_polarityMap[0] = imil::CMilSearchParams::SamePolarity;
	m_polarityMap[1] = imil::CMilSearchParams::ReversePolarity;
	m_polarityMap[2] = imil::CMilSearchParams::SameOrReversePolarity;
}


// reimplemented (imod::IModelEditor)

void CMilSearchParamsGuiComp::UpdateEditor()
{
	imil::CMilSearchParams* paramsPtr = GetObjectPtr();
	if (paramsPtr == NULL){
		return;
	}

	m_spinFirstLevel->setValue(paramsPtr->GetDownsamplingRange().GetMinValue());
	m_spinSecondLevel->setValue(paramsPtr->GetDownsamplingRange().GetMaxValue());
	m_spinTimeout->setValue(paramsPtr->GetTimeout());
	m_spinNominalScale->setValue(paramsPtr->GetNominalScale());
	m_spinNominalAngle->setValue(paramsPtr->GetNominalAngle());
	m_spinSmoothness->setValue(paramsPtr->GetSmoothness());
	m_spinAcceptanceTarget->setValue(paramsPtr->GetAcceptanceTarget());
	m_spinCertainty->setValue(paramsPtr->GetCertainty());
	m_spinCertaintyTarget->setValue(paramsPtr->GetCertaintyTarget());
	m_spinFitErrorWeight->setValue(paramsPtr->GetFitErrorWeight());
	m_spinKernelSize->setValue(paramsPtr->GetKernelSize());

	m_comboSpeed->setCurrentIndex(m_speedMap.keys(paramsPtr->GetSpeed()).at(0));
	m_comboAccuracy->setCurrentIndex(m_accuracyMap.keys(paramsPtr->GetAccuracy()).at(0));
	m_comboDetailLevel->setCurrentIndex(m_detailLevelMap.keys(paramsPtr->GetDetailLevel()).at(0));

	int searchPolarity = paramsPtr->GetPolarity();
	int indexKey = m_polarityMap.keys(searchPolarity).at(0);

	m_comboPolarity->setCurrentIndex(indexKey);

	int sharedEdgesIndex = paramsPtr->AreSharedEdgesEnabled() ? 1 : 0;
	m_comboSharedEdges->setCurrentIndex(sharedEdgesIndex);

	int targetCachingIndex = paramsPtr->IsTargetCachingEnabled() ? 1 : 0;
	m_comboTargetCaching->setCurrentIndex(targetCachingIndex);

	int filterTypeIndex = paramsPtr->GetFilterType() == imil::CMilSearchParams::NonRecursiveFilter ? 0 : 1;
	m_comboFilterType->setCurrentIndex(filterTypeIndex);

	istd::TChangeNotifier<iimg::IBitmap> updatePtr(&m_modelBitmap);
	const iimg::IBitmap* bitmapPtr = &paramsPtr->GetModelImage();
	if (bitmapPtr != NULL){
		updatePtr->CopyImageFrom(*bitmapPtr);
	}
}


void CMilSearchParamsGuiComp::UpdateModel() const
{
	imil::CMilSearchParams* paramsPtr = GetObjectPtr();
	if (paramsPtr == NULL || !IsGuiCreated()){
		return;
	}

	if (!IsUpdateBlocked()){
		UpdateBlocker blocker(const_cast<CMilSearchParamsGuiComp*>(this));

		istd::TChangeNotifier<imil::CMilSearchParams> updatePtr(paramsPtr);

		updatePtr->SetDownsamplingRange(istd::CRange(m_spinFirstLevel->value(), m_spinSecondLevel->value()));
		updatePtr->SetTimeout(m_spinTimeout->value());
		updatePtr->SetNominalScale(m_spinNominalScale->value());
		updatePtr->SetNominalAngle(m_spinNominalAngle->value());
		updatePtr->SetSmoothness(m_spinSmoothness->value());
		updatePtr->SetAcceptanceTarget(m_spinAcceptanceTarget->value());
		updatePtr->SetCertainty(m_spinCertainty->value());
		updatePtr->SetCertaintyTarget(m_spinCertaintyTarget->value());
		updatePtr->SetFitErrorWeight(m_spinFitErrorWeight->value());
		updatePtr->SetKernelSize(m_spinKernelSize->value());
		updatePtr->SetSpeed(m_speedMap[m_comboSpeed->currentIndex()]);
		updatePtr->SetAccuracy(m_accuracyMap[m_comboAccuracy->currentIndex()]);
		updatePtr->SetDetailLevel(m_detailLevelMap[m_comboDetailLevel->currentIndex()]);

		int indexKey = m_comboPolarity->currentIndex();
		int searchPolarity = m_polarityMap[indexKey];

		updatePtr->SetPolarity(searchPolarity);

		bool sharedEdgesEnabled = m_comboSharedEdges->currentIndex() == 0 ? false : true;
		updatePtr->SetSharedEdgesEnabled(sharedEdgesEnabled);

		bool targetCachingEnabled = m_comboTargetCaching->currentIndex() == 0 ? false : true;
		updatePtr->SetTargetCachingEnabled(targetCachingEnabled);

		int filterType = (m_comboFilterType->currentIndex() == 0) ? imil::CMilSearchParams::NonRecursiveFilter : imil::CMilSearchParams::RecursiveFilter;
		updatePtr->SetFilterType(filterType);
	}
}


// protected slots

void CMilSearchParamsGuiComp::OnParameterChanged()
{
	UpdateModel();
}


void CMilSearchParamsGuiComp::OnFilterTypeChanged(int filterType)
{
	bool enableKernelSize = (filterType == 0);

	m_labelKernelSize->setEnabled(enableKernelSize);
	m_spinKernelSize->setEnabled(enableKernelSize);
}


// protected methods

// reimplemented (iqt::CGuiObjectBase)

void CMilSearchParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	connect(m_spinFirstLevel, SIGNAL(valueChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_spinSecondLevel, SIGNAL(valueChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_spinTimeout, SIGNAL(valueChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_spinNominalScale, SIGNAL(valueChanged(double)), this, SLOT(OnParameterChanged()));
	connect(m_spinNominalAngle, SIGNAL(valueChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_spinSmoothness, SIGNAL(valueChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_spinAcceptanceTarget, SIGNAL(valueChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_spinCertainty, SIGNAL(valueChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_spinCertaintyTarget, SIGNAL(valueChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_spinFitErrorWeight, SIGNAL(valueChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_spinKernelSize, SIGNAL(valueChanged(int)), this, SLOT(OnParameterChanged()));

	connect(m_comboSpeed, SIGNAL(currentIndexChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_comboAccuracy, SIGNAL(currentIndexChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_comboSpeed, SIGNAL(currentIndexChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_comboAccuracy, SIGNAL(currentIndexChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_comboDetailLevel, SIGNAL(currentIndexChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_comboPolarity, SIGNAL(currentIndexChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_comboSharedEdges, SIGNAL(currentIndexChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_comboTargetCaching, SIGNAL(currentIndexChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_comboFilterType, SIGNAL(currentIndexChanged(int)), this, SLOT(OnParameterChanged()));
	connect(m_comboFilterType, SIGNAL(currentIndexChanged(int)), this, SLOT(OnFilterTypeChanged(int)));
}


} // namespace imil


