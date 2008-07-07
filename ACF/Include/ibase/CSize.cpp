#include "ibase/CSize.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ibase
{		


CSize::CSize()
{
}


CSize::CSize(int width, int height)
	:BaseClass(width, height)
{
}


bool CSize::IsNull() const
{
	return (GetX() == GetY());
}


bool CSize::Serialize(iser::IArchive& archive)
{
	int width = GetX();
	int height = GetY();

	static iser::CArchiveTag widthTag("Width", "Width");
	bool retVal = archive.BeginTag(widthTag);
	retVal = retVal && archive.Process(width);
	retVal = retVal && archive.EndTag(widthTag);

	static iser::CArchiveTag heightTag("Height", "Height");
	retVal = retVal && archive.BeginTag(heightTag);
	retVal = retVal && archive.Process(height);
	retVal = retVal && archive.EndTag(heightTag);

	if (!archive.IsStoring()){
		SetX(width);
		SetY(height);
	}

	return retVal;
}


} // namespace ibase

