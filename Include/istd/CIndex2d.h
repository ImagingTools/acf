#ifndef istd_CIndex2d_included
#define istd_CIndex2d_included


#include "istd/TIndex.h"


namespace istd
{


class CIndex2d: public TIndex<2>
{
public:
	typedef TIndex<2> BaseClass;

	CIndex2d();
	CIndex2d(int x, int y);
	CIndex2d(const TIndex<2>& index);

	int GetX() const;
	void SetX(int value);
	int GetY() const;
	void SetY(int value);

	bool operator<(const CIndex2d& index) const;
	bool operator>(const CIndex2d& index) const;
	bool operator<=(const CIndex2d& index) const;
	bool operator>=(const CIndex2d& index) const;

	CIndex2d& operator=(const TIndex<2>& index);
};


// inline methods

inline CIndex2d::CIndex2d()
{
}


inline CIndex2d::CIndex2d(int x, int y)
{
	SetAt(0, x);
	SetAt(1, y);
}


inline CIndex2d::CIndex2d(const TIndex<2>& index)
:	BaseClass(index)
{
}


inline int CIndex2d::GetX() const
{
	return GetAt(0);
}


inline void CIndex2d::SetX(int value)
{
	SetAt(0, value);
}


inline int CIndex2d::GetY() const
{
	return GetAt(1);
}


inline void CIndex2d::SetY(int value)
{
	SetAt(1, value);
}


inline bool CIndex2d::operator<(const CIndex2d& index) const
{
	if (GetY() < index.GetY()){
		return true;
	}
	else if(GetY() > index.GetY()){
		return false;
	}
	else if (GetX() < index.GetX()){
		return true;
	}

	return false;
}


inline bool CIndex2d::operator>(const CIndex2d& index) const
{
	return (!operator<(index) && !operator==(index));
}


inline bool CIndex2d::operator<=(const CIndex2d& index) const
{
	return (operator<(index) || operator==(index));
}


inline bool CIndex2d::operator>=(const CIndex2d& index) const
{
	return (!operator<(index) || operator==(index));
}


inline CIndex2d& CIndex2d::operator=(const TIndex<2>& index)
{
	BaseClass::operator=(index);

	return *this;
}


} // namespace istd


#endif // !istd_CIndex2d_included


