#include "imil/CMilSearchParams.h"


#include "istd/TChangeNotifier.h"

#include "iser/CArchiveTag.h"
#include "iser/IArchive.h"


namespace imil
{


CMilSearchParams::CMilSearchParams()
{
	ResetParams();
}


double CMilSearchParams::GetTimeout() const
{
	return m_timeout;
}


void CMilSearchParams::SetTimeout(double timeout)
{
	if (m_timeout != m_timeout){
		istd::CChangeNotifier notifierPtr(this);

		m_timeout = timeout;
	}
}


const istd::CRange& CMilSearchParams::GetDownsamplingRange() const
{
	return m_downsamplingRange;
}


void CMilSearchParams::SetDownsamplingRange(const istd::CRange& downsamplingRange)
{
	if (m_downsamplingRange != downsamplingRange){
		istd::CChangeNotifier notifierPtr(this);

		m_downsamplingRange = downsamplingRange;
	}
}


int CMilSearchParams::GetSpeed() const
{
	return m_speed;
}


void CMilSearchParams::SetSpeed(int speed)
{
	if (m_speed != speed){
		istd::CChangeNotifier notifierPtr(this);

		m_speed = speed;
	}
}


int CMilSearchParams::GetAccuracy() const
{
	return m_accuracy;
}


void CMilSearchParams::SetAccuracy(int accuracy)
{
	if (m_accuracy != accuracy){
		istd::CChangeNotifier notifierPtr(this);

		m_accuracy = accuracy;
	}
}


int CMilSearchParams::GetDetailLevel() const 
{
	return m_detailLevel;
}


void CMilSearchParams::SetDetailLevel(int detailLevel)
{
	if (m_detailLevel != detailLevel){
		istd::CChangeNotifier notifierPtr(this);

		m_detailLevel = detailLevel;
	}
}


int CMilSearchParams::GetPolarity() const
{
	return m_polarity;
}


void CMilSearchParams::SetPolarity(int polarity)
{
	if (m_polarity != polarity){
		istd::CChangeNotifier notifierPtr(this);

		m_polarity = polarity;
	}
}


int CMilSearchParams::GetAcceptanceTarget() const 
{
	return m_acceptanceTarget;
}


void CMilSearchParams::SetAcceptanceTarget(int acceptanceTarget)
{
	if (m_acceptanceTarget != acceptanceTarget){
		istd::CChangeNotifier notifierPtr(this);

		m_acceptanceTarget = acceptanceTarget;
	}
}


int CMilSearchParams::GetCertainty() const
{
	return m_certainty;
}


void CMilSearchParams::SetCertainty(int certainty)
{
	if (m_certainty != certainty){
		istd::CChangeNotifier notifierPtr(this);

		m_certainty = certainty;
	}
}


int CMilSearchParams::GetCertaintyTarget() const 
{
	return m_certaintyTarget;
}


void CMilSearchParams::SetCertaintyTarget(int certaintyTarget)
{
	if (m_certaintyTarget != certaintyTarget){
		istd::CChangeNotifier notifierPtr(this);

		m_certaintyTarget = certaintyTarget;
	}
}


int CMilSearchParams::GetFitErrorWeight() const
{
	return m_fitErrorWeight;
}


void CMilSearchParams::SetFitErrorWeight(int fitErrorWeight)
{
	if (m_fitErrorWeight != fitErrorWeight){
		istd::CChangeNotifier notifierPtr(this);

		m_fitErrorWeight = fitErrorWeight;
	}
}


int CMilSearchParams::GetSmoothness() const
{
	return m_smoothness;
}


void CMilSearchParams::SetSmoothness(int smoothness)
{
	if (m_smoothness != smoothness){
		istd::CChangeNotifier notifierPtr(this);

		m_smoothness = smoothness;
	}
}


int CMilSearchParams::GetFilterType() const
{
	return m_filterType;
}


void CMilSearchParams::SetFilterType(int filterType)
{
	if (m_filterType != filterType){
		istd::CChangeNotifier notifierPtr(this);

		m_filterType = filterType;
	}
}


int CMilSearchParams::GetKernelSize() const
{
	return m_kernelSize;
}


void CMilSearchParams::SetKernelSize(int kernelSize)
{
	if (m_kernelSize != kernelSize){
		istd::CChangeNotifier notifierPtr(this);

		m_kernelSize = kernelSize;
	}
}


double CMilSearchParams::GetNominalScale() const 
{
	return m_nominalScale;
}


void CMilSearchParams::SetNominalScale(double nominalScale)
{
	if (m_nominalScale != nominalScale){
		istd::CChangeNotifier notifierPtr(this);

		m_nominalScale = nominalScale;
	}
}


double CMilSearchParams::GetNominalAngle() const 
{
	return m_nominalAngle;
}


void CMilSearchParams::SetNominalAngle(double nominalAngle)
{
	if (m_nominalAngle != nominalAngle){
		istd::CChangeNotifier notifierPtr(this);

		m_nominalAngle = nominalAngle;
	}
}


bool CMilSearchParams::IsTargetCachingEnabled() const
{
	return m_isTargetCachingEnabled;
}


void CMilSearchParams::SetTargetCachingEnabled(bool isTargetCachingEnabled)
{
	if (m_isTargetCachingEnabled != isTargetCachingEnabled){
		istd::CChangeNotifier notifierPtr(this);

		m_isTargetCachingEnabled = isTargetCachingEnabled;
	}
}


bool CMilSearchParams::AreSharedEdgesEnabled() const
{
	return m_areSharedEdgesEnabled;
}


void CMilSearchParams::SetSharedEdgesEnabled(bool areSharedEdgesEnabled)
{
	if (m_areSharedEdgesEnabled != areSharedEdgesEnabled){
		istd::CChangeNotifier notifierPtr(this);

		m_areSharedEdgesEnabled = areSharedEdgesEnabled;
	}
}


void CMilSearchParams::ResetParams()
{
	SetMinScore(60);
	SetNominalAngle(0);

	SetRotationRange(istd::CRange(-180.0, 180.0));
	SetScaleRange(istd::CRange(0.5, 1.5));

	SetNominalModelsCount(1);

	SetDownsamplingRange(istd::CRange(0, 0));

	SetTimeout(2000);

	SetSpeed(VeryHighSpeed);
	SetAccuracy(MediumAccuracy);
	SetDetailLevel(MediumLevel);
	SetRotationEnabled(true);
	SetScaleEnabled(true);
	SetTargetCachingEnabled(false);
	SetSharedEdgesEnabled(false);

	SetPolarity(SamePolarity);
	SetAcceptanceTarget(0);
	SetCertainty(90);
	SetCertaintyTarget(0);
	SetFitErrorWeight(25);
	SetSmoothness(50);
	SetFilterType(NonRecursiveFilter);
	SetKernelSize(3);
	SetNominalScale(1.0);

	m_searchModel.ResetModel();
}


const imil::CMilSearchModel& CMilSearchParams::GetModel() const
{
	return m_searchModel;
}


// reimplemented (iipr::ISearchParams)

const iimg::IBitmap& CMilSearchParams::GetModelImage() const
{
	return m_searchModel.GetImage();
}


// reimplemented (iser::ISerializable)

bool CMilSearchParams::Serialize(iser::IArchive& archive)
{
	bool retVal = BaseClass::Serialize(archive);

	int firstLevel = int(m_downsamplingRange.GetMinValue());
	int lastLevel = int(m_downsamplingRange.GetMaxValue());

	static iser::CArchiveTag downsamplingRangeMinTag("DownsamplingRangeMinimum", "Downsampling range minimum");
	retVal = retVal && archive.BeginTag(downsamplingRangeMinTag);
	retVal = retVal && archive.Process(firstLevel);
	retVal = retVal && archive.EndTag(downsamplingRangeMinTag);

	static iser::CArchiveTag downsamplingRangeMaxTag("DownsamplingRangeMaximum", "Downsampling range maximum");
	retVal = retVal && archive.BeginTag(downsamplingRangeMaxTag);
	retVal = retVal && archive.Process(lastLevel);
	retVal = retVal && archive.EndTag(downsamplingRangeMaxTag);

	static iser::CArchiveTag modelTag("Model", "Search model");
	retVal = retVal && archive.BeginTag(modelTag);

	// serialize model from the image:
	retVal = m_searchModel.Serialize(archive);
	// if we read, so we must create the model from the model image:
	if (!archive.IsStoring() && retVal){
		m_searchModel.Create(m_searchModel.GetImage(), this);
	}
	retVal = retVal && archive.EndTag(modelTag);

	// serialize advanced parameters:
	static iser::CArchiveTag speedTag("SearchSpeed", "Search speed");
	retVal = retVal && archive.BeginTag(speedTag);
	retVal = retVal && archive.Process(m_speed);
	retVal = retVal && archive.EndTag(speedTag);

	static iser::CArchiveTag accuracyTag("SearchAccuracy", "Search accuracy");
	retVal = retVal && archive.BeginTag(accuracyTag);
	retVal = retVal && archive.Process(m_accuracy);
	retVal = retVal && archive.EndTag(accuracyTag);

	static iser::CArchiveTag detailLevelTag("SearchAccuracy", "Search accuracy");
	retVal = retVal && archive.BeginTag(detailLevelTag);
	retVal = retVal && archive.Process(m_detailLevel);
	retVal = retVal && archive.EndTag(detailLevelTag);

	static iser::CArchiveTag isTargetCachingEnabledTag("TargetCachingEnabledTag", "Is target caching enabled");
	retVal = retVal && archive.BeginTag(isTargetCachingEnabledTag);
	retVal = retVal && archive.Process(m_isTargetCachingEnabled);
	retVal = retVal && archive.EndTag(isTargetCachingEnabledTag);

	static iser::CArchiveTag polarityTag("Polarity", "Polarity");
	retVal = retVal && archive.BeginTag(polarityTag);
	retVal = retVal && archive.Process(m_polarity);
	retVal = retVal && archive.EndTag(polarityTag);

	static iser::CArchiveTag acceptanceTargetTag("AcceptanceTarget", "Acceptance target");
	retVal = retVal && archive.BeginTag(acceptanceTargetTag);
	retVal = retVal && archive.Process(m_acceptanceTarget);
	retVal = retVal && archive.EndTag(acceptanceTargetTag);

	static iser::CArchiveTag certaintyTag("Certainty", "Certainty");
	retVal = retVal && archive.BeginTag(certaintyTag);
	retVal = retVal && archive.Process(m_certainty);
	retVal = retVal && archive.EndTag(certaintyTag);

	static iser::CArchiveTag certaintyTargetTag("CertaintyTarget", "Certainty target");
	retVal = retVal && archive.BeginTag(certaintyTargetTag);
	retVal = retVal && archive.Process(m_certaintyTarget);
	retVal = retVal && archive.EndTag(certaintyTargetTag);

	static iser::CArchiveTag fitErrorWeightTag("FitErrorWeight", "FitErrorWeight");
	retVal = retVal && archive.BeginTag(fitErrorWeightTag);
	retVal = retVal && archive.Process(m_fitErrorWeight);
	retVal = retVal && archive.EndTag(fitErrorWeightTag);

	static iser::CArchiveTag smoothnessTag("Smoothness", "Smoothness");
	retVal = retVal && archive.BeginTag(smoothnessTag);
	retVal = retVal && archive.Process(m_smoothness);
	retVal = retVal && archive.EndTag(smoothnessTag);

	static iser::CArchiveTag filterTypeTag("FilterType", "Filter type");
	retVal = retVal && archive.BeginTag(filterTypeTag);
	retVal = retVal && archive.Process(m_filterType);
	retVal = retVal && archive.EndTag(filterTypeTag);

	static iser::CArchiveTag kernelSizeTag("KernelSize", "Kernel size");
	retVal = retVal && archive.BeginTag(kernelSizeTag);
	retVal = retVal && archive.Process(m_kernelSize);
	retVal = retVal && archive.EndTag(kernelSizeTag);

	static iser::CArchiveTag nominalScaleTag("Smoothness", "Smoothness");
	retVal = retVal && archive.BeginTag(nominalScaleTag);
	retVal = retVal && archive.Process(m_nominalScale);
	retVal = retVal && archive.EndTag(nominalScaleTag);

	static iser::CArchiveTag areSharedEdgesEnabledTag("SharedEdgesEnabled", "Are shared edges enabled");
	retVal = retVal && archive.BeginTag(areSharedEdgesEnabledTag);
	retVal = retVal && archive.Process(m_areSharedEdgesEnabled);
	retVal = retVal && archive.EndTag(areSharedEdgesEnabledTag);
	
	static iser::CArchiveTag nominalAngleTag("NominalAngle", "Nominal angle");
	retVal = retVal && archive.BeginTag(nominalAngleTag);
	retVal = retVal && archive.Process(m_nominalAngle);
	retVal = retVal && archive.EndTag(nominalAngleTag);

	static iser::CArchiveTag timeoutTag("Timeout", "Timout for model search");
	retVal = retVal && archive.BeginTag(timeoutTag);
	retVal = retVal && archive.Process(m_timeout);
	retVal = retVal && archive.EndTag(timeoutTag);

	return retVal;
}


} // namespace imil


