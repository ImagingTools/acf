#ifndef ibase_CSize_included
#define ibase_CSize_included


#include "istd/CIndex2d.h"

#include "iser/ISerializable.h"


namespace ibase
{		


class CSize: public istd::CIndex2d
{
public:
	typedef istd::CIndex2d BaseClass;

	CSize();
	CSize(int width, int height);

	bool IsNull() const;
	
	bool Serialize(iser::IArchive& archive);
};


} // namespace ibase


#endif // !ibase_CSize_included