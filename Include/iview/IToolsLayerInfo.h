#ifndef iview_IToolsLayerInfo_included
#define iview_IToolsLayerInfo_included


// ACF includes
#include <istd/IChangeable.h>


namespace iview
{


/**
	Interface for accessing informations about the tool layer in the shape view.
*/
class IToolsLayerInfo: virtual public istd::IChangeable
{
public:
	/**
		Check if the distance measuring is active.
	*/
	virtual bool IsDistanceMeasureToolActive() const = 0;

	/**
		Check if the point measuring is active.
	*/
	virtual bool IsPointMeasureToolActive() const = 0;
};


} // namespace iview



#endif // !iview_IToolsLayerInfo_included
