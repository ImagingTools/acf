#include <ibase/CSize.h>

#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace ibase
{		


// static constants
static const iser::CArchiveTag s_widthTag("Width", "Width", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_heightTag("Height", "Height", iser::CArchiveTag::TT_LEAF);


CSize::CSize()
{
}


CSize::CSize(int width, int height)
:	BaseClass(width, height)
{
}


CSize::CSize(const istd::CIndex2d& index)
:	BaseClass(index)
{
}


CSize::CSize(const QPoint& point)
:	BaseClass(point)
{
}


CSize::CSize(const QSize& size)
:	BaseClass(size)
{
}


CSize CSize::operator+(const CSize& size)
{
	CSize retVal(*this);

	retVal.SetX(GetX() + size.GetX());
	retVal.SetY(GetY() + size.GetY());

	return retVal;
}


CSize CSize::operator-(const CSize& size)
{
	CSize retVal(*this);

	retVal.SetX(GetX() - size.GetX());
	retVal.SetY(GetY() - size.GetY());

	return retVal;
}


CSize& CSize::operator+=(const CSize& size)
{
	SetX(GetX() + size.GetX());
	SetY(GetY() + size.GetY());

	return *this;
}


CSize& CSize::operator-=(const CSize& size)
{
	SetX(GetX() - size.GetX());
	SetY(GetY() - size.GetY());

	return *this;
}


bool CSize::IsNull() const
{
	return (GetX() == 0 || GetY() == 0);
}


bool CSize::Serialize(iser::IArchive& archive)
{
	int width = GetX();
	int height = GetY();

	bool retVal = archive.BeginTag(s_widthTag);
	retVal = retVal && archive.Process(width);
	retVal = retVal && archive.EndTag(s_widthTag);

	retVal = retVal && archive.BeginTag(s_heightTag);
	retVal = retVal && archive.Process(height);
	retVal = retVal && archive.EndTag(s_heightTag);

	if (!archive.IsStoring()){
		SetX(width);
		SetY(height);
	}

	return retVal;
}


} // namespace ibase

