#ifndef iqtgui_IMultiVisualStatusProvider_included
#define iqtgui_IMultiVisualStatusProvider_included


// ACF includes
#include <iqtgui/IVisualStatus.h>


namespace iqtgui
{


/**
	Common interface for a provider of the multiple visual statuses.
*/
class IMultiVisualStatusProvider: virtual public istd::IChangeable
{
public:
	/**
		Get the number of visual statuses.
	*/
	virtual int GetStatusesCount() const = 0;
	
	/**
		Get the visual status at a given index \c statusIndex.
	*/
	virtual const IVisualStatus* GetVisualStatus(int statusIndex) const = 0;
};


} // namespace iqtgui


#endif // !iqtgui_IMultiVisualStatusProvider_included


