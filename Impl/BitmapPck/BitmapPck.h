#ifndef BitmapPck_included
#define BitmapPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iimg/CBitmap.h"
#include "iimg/CBitmapSurface.h"
#include "iimg/CBitmapLoaderComp.h"
#include "iimg/CGeneralBitmap.h"
#include "iimg/CBitmapDocument.h"


/**
	Bitmap package.
*/
namespace BitmapPck
{


typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						iimg::CBitmap,
						iimg::IBitmap,
						iimg::IRasterImage,
						ibase::IObjectSnap,
						iser::ISerializable> > Bitmap;

typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						iimg::CGeneralBitmap,
						iimg::IBitmap,
						iimg::IRasterImage,
						ibase::IObjectSnap,
						iser::ISerializable> > GeneralBitmap;

typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						iimg::CBitmapSurface,
						iimg::IBitmap,
						iimg::IRasterImage,
						iser::ISerializable,
						imath::ISampledFunction2d> > BitmapSurface;

typedef iimg::CBitmapLoaderComp BitmapLoader;

typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						iimg::CBitmapDocument,
						idoc::IMultiPageDocument,
						iprm::IOptionsList,
						iser::ISerializable> > BitmapDocument;



} // namespace BitmapPck


#endif // !BitmapPck_included


