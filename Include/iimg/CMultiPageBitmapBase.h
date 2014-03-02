#ifndef iimg_CMultiPageBitmapBase_included
#define iimg_CMultiPageBitmapBase_included


// ACF includes
#include "imod/TModelWrap.h"
#include "idoc/TMultiPageDocumentWrap.h"
#include "idoc/CStandardDocumentMetaInfo.h"
#include "iimg/IMultiBitmapProvider.h"
#include "iimg/IMultiPageBitmapController.h"


namespace iimg
{


/**
	Definition of a multi-page bitmap document.
*/
class CMultiPageBitmapBase:
			public idoc::CMultiPageDocumentBase,
			virtual public IMultiBitmapProvider,
			virtual public IMultiPageBitmapController
{
public:
	typedef idoc::CMultiPageDocumentBase BaseClass;

	enum
	{
		MIT_CAMERA_MODEL = idoc::IDocumentMetaInfo::MIT_USER + 1
	};

	// reimplemented (idoc::IMultiPageDocument)
	virtual istd::IChangeable* InsertPage(
		const idoc::IDocumentMetaInfo* pageMetaInfoPtr = NULL,
		const iprm::IParamsSet* pageParameterPtr = NULL,
		int position = -1);

	// reimplemented (iimg::IMultiBitmapProvider)
	virtual const iprm::IOptionsList* GetBitmapListInfo() const;
	virtual int GetBitmapsCount() const;
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const;

	// reimplemented (iimg::IMultiPageBitmapController)
	virtual void Reset();
	virtual iimg::IBitmap* InsertBitmap(
		iimg::IBitmap::PixelFormat pixelFormat,
		const istd::CIndex2d& size);
	virtual iimg::IBitmap* InsertBitmap(
		iimg::IBitmap::PixelFormat pixelFormat,
		const istd::CIndex2d& size,
		void* dataPtr,
		bool releaseFlag,
		int linesDifference = 0);
	virtual void RemoveBitmap(int index);

protected:
	// abstract methods
	virtual IBitmap* CreateBitmap() const = 0;
};


} // namespace iimg


#endif // !iimg_CMultiPageBitmapBase_included


