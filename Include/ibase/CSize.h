#ifndef ibase_CSize_included
#define ibase_CSize_included


// ACF includes
#include <istd/CIndex2d.h>
#include <iser/ISerializable.h>


namespace ibase
{		


/**
	Definition of simple 2D size based on integer values.
*/
class CSize: public istd::CIndex2d
{
public:
	typedef istd::CIndex2d BaseClass;

	CSize();
	CSize(int width, int height);
	CSize(const istd::CIndex2d& index);
	CSize(const QPoint& point);
	CSize(const QSize& size);

	CSize operator+(const CSize& size);
	CSize operator-(const CSize& size);
	CSize& operator+=(const CSize& size);
	CSize& operator-=(const CSize& size);

	bool IsNull() const;
	
	bool Serialize(iser::IArchive& archive);
};


} // namespace ibase


#endif // !ibase_CSize_included


