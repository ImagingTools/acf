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
	CSize(const istd::CIndex2d& index);

	CSize operator+(const CSize& size);
	CSize operator-(const CSize& size);
	CSize& operator+=(const CSize& size);
	CSize& operator-=(const CSize& size);

	bool IsNull() const;
	
	bool Serialize(iser::IArchive& archive);
};


} // namespace ibase


#endif // !ibase_CSize_included