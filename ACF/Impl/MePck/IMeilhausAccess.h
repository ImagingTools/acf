#ifndef IMeilhausAccess_included
#define IMeilhausAccess_included


#include "istd/IPolymorphic.h"
#include "istd/CRange.h"


namespace imebase
{


class CChannelSelectionNode;


class IMeilhausAccess: public istd::IPolymorphic
{
public:
	virtual bool CreateSelectionTree(CChannelSelectionNode& result) const = 0;
	virtual istd::CRange GetIntervalRange() const = 0;
	virtual bool IsSamplingModeSupported(int mode) const = 0;
};


} // namespace imebase


#endif // !IMeilhausAccess_included


