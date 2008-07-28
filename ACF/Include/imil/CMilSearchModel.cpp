#include "imil/CMilSearchModel.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "ibase/CMessage.h"

#include "iimg/CBitmapBase.h"

#include "imil/CMilSearchParams.h"


namespace imil
{


// public methods

CMilSearchModel::CMilSearchModel()
:	m_milSearchContextId(M_NULL)
{
	MmodAlloc(m_milEngine.GetSystemId(), M_GEOMETRIC, M_DEFAULT, &m_milSearchContextId);
}


CMilSearchModel::~CMilSearchModel()
{
	ReleaseMilContext();

	ResetModel();
}


void CMilSearchModel::ReleaseMilContext()
{
	if (m_milSearchContextId != M_NULL){
		MmodFree(m_milSearchContextId);
		m_milSearchContextId = M_NULL;
	}
}


MIL_ID CMilSearchModel::GetContextId() const
{
	return m_milSearchContextId;
}


void CMilSearchModel::EnsurePreprocessing(const imil::CMilSearchParams& paramsPtr, bool preprocess) const
{
	int modelNumber = paramsPtr.GetNominalModelsCount();
	if (modelNumber == -1){
		modelNumber = M_ALL;
	}

	double minScore = paramsPtr.GetMinScore();
	double angleNeg = -paramsPtr.GetRotationRange().GetMinValue() + I_EPSILON;
	if (angleNeg > 180.0){
		angleNeg = 180.0;
	}

	double anglePos = paramsPtr.GetRotationRange().GetMaxValue() + I_EPSILON;
	if (anglePos > 180){
		anglePos = 180.0;
	}

	double minScale = paramsPtr.GetScaleRange().GetMinValue();
	double maxScale = paramsPtr.GetScaleRange().GetMaxValue();
	double nominalScale = paramsPtr.GetNominalScale();
	int firstLevel = int(paramsPtr.GetDownsamplingRange().GetMinValue());
	int lastLevel = int(paramsPtr.GetDownsamplingRange().GetMaxValue());
	int timeout = int(paramsPtr.GetTimeout()) * 1000;

	int speed = paramsPtr.GetSpeed();
	int accuracy = paramsPtr.GetAccuracy();
	int detailLevel = paramsPtr.GetDetailLevel();
	int scaleRangeEnabled = paramsPtr.IsScaleEnabled() ? M_ENABLE : M_DISABLE;
	int angleRangeEnabled = paramsPtr.IsRotationEnabled() ? M_ENABLE : M_DISABLE;
	int isTargetCachingEnabled = paramsPtr.IsTargetCachingEnabled() ? M_ENABLE : M_DISABLE;
	int areSharedEdgesEnabled = paramsPtr.AreSharedEdgesEnabled() ? M_ENABLE : M_DISABLE;
	double nominalAngle = paramsPtr.GetNominalAngle();
	int kernelSize = paramsPtr.GetKernelSize();
	int filterType = paramsPtr.GetFilterType();
	int acceptanceTarget = paramsPtr.GetAcceptanceTarget();
	int certainty = paramsPtr.GetCertainty();
	int certaintyTarget = paramsPtr.GetCertaintyTarget();
	int fitErrorWeight = paramsPtr.GetFitErrorWeight();
	int polarity = paramsPtr.GetPolarity();
	int smoothness = paramsPtr.GetSmoothness();

	// Advanced:
	MmodControl(m_milSearchContextId, M_ALL, M_POLARITY, polarity);
	MmodControl(m_milSearchContextId, M_ALL, M_ACCEPTANCE_TARGET, acceptanceTarget);
	MmodControl(m_milSearchContextId, M_ALL, M_CERTAINTY, certainty);
	MmodControl(m_milSearchContextId, M_ALL, M_CERTAINTY_TARGET, certaintyTarget);
	MmodControl(m_milSearchContextId, M_ALL, M_FIT_ERROR_WEIGHTING_FACTOR, fitErrorWeight);
	MmodControl(m_milSearchContextId, M_CONTEXT, M_SMOOTHNESS, smoothness);
	MmodControl(m_milSearchContextId, M_CONTEXT, M_TARGET_CACHING, isTargetCachingEnabled);
	MmodControl(m_milSearchContextId, M_CONTEXT, M_SHARED_EDGES, areSharedEdgesEnabled);
	MmodControl(m_milSearchContextId, M_CONTEXT, M_DETAIL_LEVEL, detailLevel);
	MmodControl(m_milSearchContextId, M_CONTEXT, M_FILTER_MODE, filterType);
	MmodControl(m_milSearchContextId, M_CONTEXT, M_KERNEL_WIDTH, kernelSize);

	// Set the search speed:
	MmodControl(m_milSearchContextId, M_CONTEXT, M_SPEED, speed);
	MmodControl(m_milSearchContextId, M_CONTEXT, M_ACCURACY, accuracy);
	MmodControl(m_milSearchContextId, M_CONTEXT, M_FIRST_LEVEL, firstLevel);
	MmodControl(m_milSearchContextId, M_CONTEXT, M_LAST_LEVEL, lastLevel);
	MmodControl(m_milSearchContextId, M_CONTEXT, M_TIMEOUT, timeout); // in [ms]

	// Set the maximum number of all model occurrences
	MmodControl(m_milSearchContextId, M_DEFAULT, M_NUMBER, modelNumber);
	
	// Set the minimal score
	MmodControl(m_milSearchContextId, M_ALL, M_ACCEPTANCE, minScore);

	// Scale range enabled
	MmodControl(m_milSearchContextId, M_CONTEXT, M_SEARCH_SCALE_RANGE, scaleRangeEnabled);

	// Angle range enabled
	MmodControl(m_milSearchContextId, M_CONTEXT, M_SEARCH_ANGLE_RANGE, angleRangeEnabled);

	// Position range enabled
	MmodControl(m_milSearchContextId, M_CONTEXT, M_SEARCH_POSITION_RANGE, M_ENABLE);

	// Define angle range
	MmodControl(m_milSearchContextId, M_ALL, M_ANGLE, nominalAngle);
	MmodControl(m_milSearchContextId, M_ALL, M_ANGLE_DELTA_NEG, angleNeg);
	MmodControl(m_milSearchContextId, M_ALL, M_ANGLE_DELTA_POS, anglePos);

	// Define scale range
	MmodControl(m_milSearchContextId, M_ALL, M_SCALE, nominalScale);
	MmodControl(m_milSearchContextId, M_ALL, M_SCALE_MIN_FACTOR, minScale);
	MmodControl(m_milSearchContextId, M_ALL, M_SCALE_MAX_FACTOR, maxScale);

	bool needPreprocessing = (MmodInquire(m_milSearchContextId, M_CONTEXT, M_PREPROCESSED, M_NULL) == 0);

	if (needPreprocessing || preprocess){
		// Preprocess the search context:
		MmodPreprocess(m_milSearchContextId, M_DEFAULT);
	}
}


// reimplemented (iipr::ISearchModel)

bool CMilSearchModel::IsValid() const
{
	int numberModels =  MmodInquire(m_milSearchContextId, M_CONTEXT, M_NUMBER_MODELS, M_NULL);

	return numberModels && (MmodInquire(m_milSearchContextId, M_CONTEXT, M_PREPROCESSED, M_NULL) != 0);
}


void CMilSearchModel::ResetModel()
{
	if (m_milSearchContextId != M_NULL){
		double numberOfModels = 0;

		MmodInquire(m_milSearchContextId, M_CONTEXT, M_NUMBER_MODELS, &numberOfModels);
		for (int modelIndex = 0; modelIndex < numberOfModels; ++modelIndex){
			MmodDefine(m_milSearchContextId, M_DELETE, modelIndex, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);
		}
	}
}


const iimg::IBitmap& CMilSearchModel::GetImage() const
{
	return m_modelImage;
}


bool CMilSearchModel::Create(const iimg::IBitmap& modelImage, const iipr::ISearchParams* paramsPtr)
{
	bool isModelCreated = IsValid();

	const iimg::IBitmap* bitmapPtr = dynamic_cast<const iimg::IBitmap*>(&modelImage);
	I_ASSERT(bitmapPtr != NULL);

	if (bitmapPtr == NULL || bitmapPtr->IsEmpty()){
	//	SendErrorMessage(0, "Input bitmap is empty or invalid.");
		
		return false;
	}

	i2d::CRectangle modelRectangle = i2d::CRectangle(0, 0, 
				bitmapPtr->GetImageSize().GetX() - 1, 
				bitmapPtr->GetImageSize().GetY() - 1);

	if (isModelCreated){
		ResetModel();
	}

	if (modelRectangle.IsEmpty()){
	//	SendErrorMessage(0, "model- or aoi area is empty or invalid.");

		return false;
	}

	// Create the model image:
	if (bitmapPtr != &m_modelImage){
		m_modelImage.CopyImageFrom(*bitmapPtr);
	}

	// Create MIL-image:
	MIL_ID milImage = M_NULL;
	MbufCreate2d(
				m_milEngine.GetSystemId(), 
				m_modelImage.GetImageSize().GetX(), 
				m_modelImage.GetImageSize().GetY(), 
				8, 
				M_IMAGE, 
				M_HOST_ADDRESS | M_PITCH_BYTE, 
				m_modelImage.GetLineBytesCount(), 
				m_modelImage.GetLinePtr(0), 
				&milImage);

	I_ASSERT(milImage != M_NULL);

	// Define the model:
	MmodDefine(m_milSearchContextId, 
		M_IMAGE, 
		milImage,
		M_DEFAULT, 
		M_DEFAULT, 
		M_DEFAULT, 
		M_DEFAULT);

	// Do model preprocessing:
	const imil::CMilSearchParams* milParamsPtr = dynamic_cast<const imil::CMilSearchParams*>(paramsPtr);
	if (milParamsPtr != NULL){
		EnsurePreprocessing(*milParamsPtr, true);
	}

	MbufFree(milImage);

	return true; 
}


// reimplemented (iser::ISerializable)

bool CMilSearchModel::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag modelImageTag("ModeImage", "ModelImage");

	bool retVal = archive.BeginTag(modelImageTag);

	iimg::CBitmapBase* bitmapPtr = dynamic_cast<iimg::CBitmapBase*>(&m_modelImage);
	if(bitmapPtr != NULL){
		retVal = retVal && bitmapPtr->Serialize(archive);
	}

	retVal = retVal && archive.EndTag(modelImageTag);

	return retVal;
}


} // namespace imil


