#include "iimg/CBitmapSurface.h"


// Qt includes
#include <QtCore/QVector>
#include <QtGui/QColor>


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iimg
{


CBitmapSurface::CBitmapSurface()
{
}


CBitmapSurface::CBitmapSurface(const CBitmapSurface& bitmap)
:	BaseClass(bitmap),
	BaseClass2()
{
}


// reimplemented (istd::IChangeable)

void CBitmapSurface::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	CreateSurfaceFromBitmap();

	BaseClass::OnEndChanges(changeFlags, changeParamsPtr);
	BaseClass2::OnEndChanges(changeFlags, changeParamsPtr);
}


// private methods

void CBitmapSurface::CreateSurfaceFromBitmap()
{
	if (GetQImageRef().isNull()){
		return;
	}

	BaseClass2::CreateGrid2d(GetImageSize());

	int componentBitsCount = GetComponentBitsCount();
	int pixelBitsCount = GetPixelBitsCount();
	int bufferIncrement = 	pixelBitsCount >> 3;

	for (int y = 0; y < GetImageSize().GetY(); y++){
		quint8* bufferPtr = (quint8*)GetLinePtr(y);

		for (int x = 0; x < GetImageSize().GetX(); x++){
			double functionValue = 0;
			if (componentBitsCount == 8){
				if (pixelBitsCount == 32){
					functionValue = qGray(*((int*)(bufferPtr)));
				}
				else if (pixelBitsCount == 8){
					functionValue = *bufferPtr;					
				}
			}

			ArgumentType index;
			index.SetAt(0, x);
			index.SetAt(1, y);
			BaseClass2::SetSampleValue(index, functionValue);

			bufferPtr += bufferIncrement;
		}
	}
}


} // namespace iimg


