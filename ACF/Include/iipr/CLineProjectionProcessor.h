#ifndef iipr_CLineProjectionProcessor_included
#define iipr_CLineProjectionProcessor_included


#include "i2d/CLine2d.h"

#include "iproc/TSyncProcessorWrap.h"

#include "iipr/ILineProjectionProcessor.h"
#include "iipr/IProjectionConstraints.h"


namespace iipr
{


class CLineProjectionProcessor:
			public iproc::TSyncProcessorWrap<ILineProjectionProcessor>,
			virtual public IProjectionConstraints
{
public:
	typedef iproc::TSyncProcessorWrap<ILineProjectionProcessor> BaseClass;

	/**
		Do projection along specified line with variable projection size.
	*/
	bool DoAutosizeProjection(
				const iimg::IBitmap& bitmap,
				const i2d::CLine2d& projectionLine,
				CProjectionData& results) const;

	/**
		Get parameter ID used to extract line object from parameter set.
	*/
	const std::string& GetLineParamId() const;

	/**
		Set parameter ID used to extract line object from parameter set.
		It is only needed while using general processing interface iproc::TIProcessor.
	*/
	void SetLineParamId(const std::string& id);

	// reimplemented (iipr::ILineProjectionProcessor)
	virtual bool DoProjection(
				const iimg::IBitmap& bitmap,
				const i2d::CLine2d& projectionLine,
				const IProjectionParams* paramsPtr,
				CProjectionData& results);

	// reimplemented (iproc::TIProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap* inputPtr,
				CProjectionData* outputPtr);

	// reimplemented (iipr::IProjectionConstraints)
	virtual istd::CRange GetLineWidthRange() const;
	virtual int GetMinProjectionSize() const;
	virtual int GetMaxProjectionSize() const;
	virtual bool IsAutoProjectionSizeSupported() const;

private:
	std::string m_lineParamId;
};


} // namespace iipr


#endif // !iipr_CLineProjectionProcessor_included


