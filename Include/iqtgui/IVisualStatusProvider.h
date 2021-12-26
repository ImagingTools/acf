#ifndef iqtgui_IVisualStatusProvider_included
#define iqtgui_IVisualStatusProvider_included


// ACF includes
#include <iqtgui/IVisualStatus.h>


namespace iqtgui
{


class IVisualStatusProvider: virtual public istd::IChangeable
{
public:
	virtual const IVisualStatus* GetVisualStatus() const = 0;
};


} // namespace iqtgui


#endif // !iqtgui_IVisualStatusProvider_included


