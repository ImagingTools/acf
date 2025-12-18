#ifndef iview_IScreenTransformationProvider_included
#define iview_IScreenTransformationProvider_included


// ACF includes
#include <istd/IPolymorphic.h>
#include <istd/TSmartPtr.h>
#include <iview/CScreenTransform.h>


namespace iview
{

class IScreenTransformationProvider: virtual public istd::IPolymorphic
{
public:
	virtual istd::TSmartPtr<iview::CScreenTransform> GetTransformation() const = 0;
};


} // namespace iview


#endif // !iview_IScreenTransformationProvider_included


