#ifndef icomp_TSubelementStaticInfo_included
#define icomp_TSubelementStaticInfo_included


// STL incldues
#include <map>
#include <string>


// ACF includes
#include "icomp/IElementStaticInfo.h"
#include "icomp/CBaseComponentStaticInfo.h"


namespace icomp
{


template <class ComponentType>
class TSubelementStaticInfo:
			virtual public IElementStaticInfo,
			virtual public IComponentInterfaceExtractor
{
public:
	typedef void* (*InterfaceExtractorPtr)(ComponentType& component);

	/**
		Constructor attaching this subelement info to the component.
	*/
	TSubelementStaticInfo(const std::string& name, CBaseComponentStaticInfo& owner);

	/**
		Register interface ID for this static component info.
		This interface ID is used for static check
		if this component can be used to resolve reference dependecy of second one.
	*/
	virtual void RegisterInterfaceExtractor(const std::string& interfaceName, InterfaceExtractorPtr extractorPtr);

	//	reimplemented (icomp::IElementStaticInfo)
	virtual const IElementStaticInfo* GetSubelementInfo(const std::string& subcomponentId) const;
	virtual Ids GetMetaIds(int metaGroupId) const;

	//	reimplemented (icomp::IComponentInterfaceExtractor)
	virtual void* GetComponentInterface(
				const istd::CClassInfo& interfaceType,
				IComponent& component,
				const std::string& subId) const;

	template <class InterfaceType>
	class Registrator
	{
	public:
		typedef InterfaceType* (*ExtractorPtr)(ComponentType& component);

		Registrator(TSubelementStaticInfo& staticInfo,
					const std::string& interfaceName,
					ExtractorPtr extractorPtr)
		{
			staticInfo.RegisterInterfaceExtractor(interfaceName, reinterpret_cast<InterfaceExtractorPtr>(extractorPtr));
		}
	};

private:
	typedef std::map<std::string, InterfaceExtractorPtr> InterfaceExtractors;
	InterfaceExtractors m_interfaceExtractors;
};


// public methods

template <class ComponentType>
TSubelementStaticInfo<ComponentType>::TSubelementStaticInfo(const std::string& name, CBaseComponentStaticInfo& owner)
{
	owner.RegisterSubelementInfo(name, this);
}


template <class ComponentType>
void TSubelementStaticInfo<ComponentType>::RegisterInterfaceExtractor(const std::string& interfaceName, InterfaceExtractorPtr extractorPtr)
{
	m_interfaceExtractors[interfaceName] = extractorPtr;
}


//	reimplemented (icomp::IElementStaticInfo)

template <class ComponentType>
const IElementStaticInfo* TSubelementStaticInfo<ComponentType>::GetSubelementInfo(const std::string& /*subcomponentId*/) const
{
	return NULL;
}


template <class ComponentType>
IElementStaticInfo::Ids TSubelementStaticInfo<ComponentType>::GetMetaIds(int metaGroupId) const
{
	Ids retVal;

	if (metaGroupId == MGI_INTERFACES){
		for (		InterfaceExtractors::const_iterator iter = m_interfaceExtractors.begin();
					iter != m_interfaceExtractors.end();
					++iter){
			retVal.insert(iter->first);
		}
	}

	return retVal;
}


//	reimplemented (icomp::IComponentInterfaceExtractor)

template <class ComponentType>
void* TSubelementStaticInfo<ComponentType>::GetComponentInterface(
			const istd::CClassInfo& interfaceType,
			IComponent& component,
			const std::string& subId) const
{
	ComponentType* nativeTypePtr = dynamic_cast<ComponentType*>(&component);
	if ((nativeTypePtr != NULL) && subId.empty()){
		static istd::CClassInfo compInterfaceType(typeid(icomp::IComponent));

		if (!interfaceType.IsValid() || interfaceType.IsVoid() || (interfaceType == compInterfaceType)){
			return &component;
		}

		InterfaceExtractors::const_iterator foundIter = m_interfaceExtractors.find(interfaceType.GetName());
		if (foundIter != m_interfaceExtractors.end()){
			InterfaceExtractorPtr extractorPtr = foundIter->second;

			return extractorPtr(*nativeTypePtr);
		}

		if (interfaceType.IsConst()){
			istd::CClassInfo nonConstType = interfaceType.GetConstCasted(false);

			InterfaceExtractors::const_iterator foundIter = m_interfaceExtractors.find(nonConstType.GetName());
			if (foundIter != m_interfaceExtractors.end()){
				InterfaceExtractorPtr extractorPtr = foundIter->second;

				return extractorPtr(*nativeTypePtr);
			}
		}
	}

	return NULL;
}


} // namespace icomp


#endif // !icomp_TSubelementStaticInfo_included


