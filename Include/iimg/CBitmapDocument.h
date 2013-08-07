#ifndef iimg_CBitmapDocument_included
#define iimg_CBitmapDocument_included


// ACF includes
#include "imod/TModelWrap.h"
#include "idoc/TMultiPageDocumentWrap.h"
#include "idoc/CStandardDocumentMetaInfo.h"
#include "iimg/IMultiBitmapProvider.h"
#include "iimg/CBitmap.h"


namespace iimg
{


/**
	Definition of a multi-page bitmap document.
*/
class CBitmapDocument:
			public idoc::CMultiPageDocumentBase,
			virtual public iimg::IMultiBitmapProvider
{
public:
	typedef idoc::CMultiPageDocumentBase BaseClass;
	typedef imod::TModelWrap<CBitmap> Bitmap;

	enum
	{
		MIT_CAMERA_MODEL = idoc::IDocumentMetaInfo::MIT_USER + 1
	};

	// reimplemented (idoc::IMultiPageDocument)
	virtual istd::IChangeable* InsertPage(
		const QString& pageTitle = QString(),
		const QSizeF& pageSize = QSizeF(),
		int position = -1);

	// reimplemented (iimg::IMultiBitmapProvider)
	virtual const iprm::IOptionsList* GetBitmapListInfo() const;
	virtual int GetBitmapsCount() const;
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;
};


} // namespace iimg


#endif // !iimg_CBitmapDocument_included


