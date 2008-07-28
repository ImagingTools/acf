#ifndef imil_CMilSearchModel_included
#define imil_CMilSearchModel_included


#include "i2d/CRectangle.h"

#include "ibase/TMessageProducerWrap.h"

#include "iimg/CGeneralBitmap.h"

#include "iipr/ISearchModel.h"
#include "iipr/ISearchParams.h"

#include "imil/CMilEngine.h"


namespace imil
{


class CMilSearchParams;


class CMilSearchModel: virtual public iipr::ISearchModel
{
public:
	CMilSearchModel();
	~CMilSearchModel();

	void ReleaseMilContext();
	MIL_ID GetContextId() const;
	void EnsurePreprocessing(const imil::CMilSearchParams& params, bool preprocess = false) const;

	// reimplemented (iipr::ISearchModel)
	virtual bool IsValid() const;
	virtual void ResetModel();
	virtual const iimg::IBitmap& GetImage() const;
	bool Create(const iimg::IBitmap& modelImage, const iipr::ISearchParams* params);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive & archive);
	
private:
	iimg::CGeneralBitmap m_modelImage;
	CMilEngine m_milEngine;
	MIL_ID m_milSearchContextId;
};


} // namespace imil


#endif // !imil_CMilSearchModel_included


