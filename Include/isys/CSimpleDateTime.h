#ifndef isys_CSimpleDateTime_included
#define isys_CSimpleDateTime_included


#include "istd/TIndex.h"

#include "isys/CDateTimeBase.h"


namespace isys
{


/**
	Implementation of isys::IDateTime interface using no operating system.
*/
class CSimpleDateTime: public isys::CDateTimeBase
{
public:
	// reimplemented (isys::IDateTime)
	virtual int GetComponent(int component) const;
	virtual void SetComponent(int component, int value);
	virtual bool SetCurrentTime();
	virtual double ToCTime() const;
	virtual void FromCTime(double ctime);

private:
	istd::TIndex<TC_LAST + 1> m_components;
};


} // namespace isys


#endif // !isys_CSimpleDateTime_included


