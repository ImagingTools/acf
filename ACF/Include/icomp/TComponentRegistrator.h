#ifndef icomp_TComponentRegistrator_included
#define icomp_TComponentRegistrator_included


#include <string>

#include "icomp/IComponentStaticInfo.h"
#include "icomp/TComponentStaticInfo.h"


namespace icomp
{


/**
	Simple helper class providing registration of template-specified component in package.
*/
template <class Component>
class TComponentRegistrator: public icomp::TComponentStaticInfo<Component>
{
public:
	typedef icomp::TComponentStaticInfo<Component> BaseClass;

	TComponentRegistrator(
				const std::string& componentId,
				icomp::IComponentStaticInfo& staticInfo,
				const istd::CString& description,
				const istd::CString& keywords);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual istd::CString GetDescription() const;
	virtual istd::CString GetKeywords() const;

private:
	istd::CString m_description;
	istd::CString m_keywords;
};


// public methods

template <class Component>
TComponentRegistrator<Component>::TComponentRegistrator(
			const std::string& componentId,
			icomp::IComponentStaticInfo& staticInfo,
			const istd::CString& description,
			const istd::CString& keywords)
:	BaseClass(&Component::InitStaticInfo(NULL)),
	m_description(description),
	m_keywords(keywords)
{
	staticInfo.RegisterSubcomponentInfo(componentId, this);
}


//	reimplemented (icomp::IComponentStaticInfo)

template <class Component>
istd::CString TComponentRegistrator<Component>::GetDescription() const
{
	return m_description;
}


template <class Component>
istd::CString TComponentRegistrator<Component>::GetKeywords() const
{
	return m_keywords;
}


}//namespace icomp


#endif // !icomp_TComponentRegistrator_included


