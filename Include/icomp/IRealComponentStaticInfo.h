#ifndef icomp_IRealComponentStaticInfo_included
#define icomp_IRealComponentStaticInfo_included


#include "icomp/IComponentStaticInfo.h"


namespace icomp
{


/**
	Interface adding to component static info functionality existing only for real components.
*/
class IRealComponentStaticInfo: virtual public IComponentStaticInfo
{
public:
	/**
		Map from class type to interface extractor implementation.
	*/
	typedef istd::TIMap<std::string, InterfaceExtractorPtr> InterfaceExtractors;

	/**
		Get list of physical interface extractors.
	*/
	virtual const InterfaceExtractors& GetInterfaceExtractors() const = 0;
};


}//namespace icomp


#endif // !icomp_IRealComponentStaticInfo_included


