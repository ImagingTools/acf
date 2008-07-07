#ifndef iipr_CRectangularFilterParams_included
#define iipr_CRectangularFilterParams_included


#include "iser/ISerializable.h"

#include "ibase/CSize.h"


namespace iipr
{


/**
	Basic implementation for the local operators with a rectangular kernel
*/
class CRectangularFilterParams: virtual public iser::ISerializable
{
public:
	CRectangularFilterParams();

	virtual const ibase::CSize& GetSize() const;
	virtual void SetSize(const ibase::CSize& filterSize);

	virtual int GetIterationsCount() const;
	virtual void SetIterationsCount(int iterationsCount);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
private:
	ibase::CSize m_filterSize;
	int m_iterationsCount;
};


} // namespace iipr


#endif // !iipr_CRectangularFilterParams_included

