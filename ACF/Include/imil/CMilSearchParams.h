#ifndef imil_CMilSearchParams_included
#define imil_CMilSearchParams_included


#include "iipr/CSearchParams.h"

#include "imil/CMilSearchModel.h"


namespace imil
{


class CMilSearchParams: public iipr::CSearchParams
{
public:
	typedef iipr::CSearchParams BaseClass;

	enum Speed
	{
		LowSpeed = M_LOW, 
		MediumSpeed = M_MEDIUM,
		HighSpeed = M_HIGH,
		VeryHighSpeed = M_VERY_HIGH
	};

	enum Accuracy
	{
		MediumAccuracy = M_MEDIUM,
		HighAccuracy = M_HIGH
	};

	enum DetailLevel
	{
		MediumLevel = M_MEDIUM,
		HighLevel = M_HIGH,
		VeryHighLevel = M_VERY_HIGH
	};

	enum Polarity
	{
		AnyPolarity = M_ANY,
		SamePolarity = M_SAME,
		ReversePolarity = M_REVERSE,
		SameOrReversePolarity = M_SAME_OR_REVERSE
	};

	enum FilterType
	{
		RecursiveFilter = M_RECURSIVE,
		NonRecursiveFilter = M_KERNEL
	};
	
	CMilSearchParams();

	virtual double GetTimeout() const; 
	virtual void SetTimeout(double timeout);
	virtual const istd::CRange& GetDownsamplingRange() const;
	virtual void SetDownsamplingRange(const istd::CRange& downsamplingRange);
	virtual int GetSpeed() const; 
	virtual void SetSpeed(int searchSpeed);
	virtual int GetAccuracy() const;
	virtual void SetAccuracy(int accuracy);
	virtual int GetDetailLevel() const; 
	virtual void SetDetailLevel(int detailLevel);
	virtual int GetPolarity() const;
	virtual void SetPolarity(int polarity);
	virtual int GetAcceptanceTarget() const; 
	virtual void SetAcceptanceTarget(int acceptanceTarget);
	virtual int GetCertainty() const;
	virtual void SetCertainty(int certainty);
	virtual int GetCertaintyTarget() const; 
	virtual void SetCertaintyTarget(int certaintyTarget);
	virtual int GetFitErrorWeight() const;
	virtual void SetFitErrorWeight(int fitErrorWeight);
	virtual int GetSmoothness() const;
	virtual void SetSmoothness(int smoothness);
	virtual int GetFilterType() const;
	virtual void SetFilterType(int filterType);
	virtual int GetKernelSize() const;
	virtual void SetKernelSize(int kernelSize);
	virtual double GetNominalScale() const; 
	virtual void SetNominalScale(double nominalScale);
	virtual double GetNominalAngle() const; 
	virtual void SetNominalAngle(double nominalAngle);
	virtual bool IsTargetCachingEnabled() const;
	virtual void SetTargetCachingEnabled(bool isTargetCachingEnabled);
	virtual bool AreSharedEdgesEnabled() const;
	virtual void SetSharedEdgesEnabled(bool areSharedEdgesEnabled);
	virtual void ResetParams();

	virtual const imil::CMilSearchModel& GetModel() const;

	// reimplemented (iipr::ISearchParams)
	virtual const iimg::IBitmap& GetModelImage() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive & archive);

private:
	imil::CMilSearchModel m_searchModel;
	istd::CRange m_downsamplingRange;
	double m_timeout;
	int m_detailLevel;
	int m_speed;
	int m_accuracy;
	double m_nominalScale;
	double m_nominalAngle;
	bool m_isTargetCachingEnabled;
	bool m_areSharedEdgesEnabled;
	int m_polarity;
	int m_acceptanceTarget;
	int m_certainty;
	int m_certaintyTarget;
	int m_fitErrorWeight;
	int m_smoothness;
	int m_filterType;
	int m_kernelSize;
};


} // namespace imil


#endif //!imil_CMilSearchParams_included


