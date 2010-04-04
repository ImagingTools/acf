#ifndef icomp_CCompositeComponentStaticInfo_included
#define icomp_CCompositeComponentStaticInfo_included


// STL includes
#include <map>


// ACF includes
#include "istd/TDelPtr.h"

#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/IAttributeStaticInfo.h"
#include "icomp/IRegistry.h"
#include "icomp/CBaseComponentStaticInfo.h"


namespace icomp
{


class CCompositeComponentStaticInfo: public CBaseComponentStaticInfo
{
public:
	CCompositeComponentStaticInfo(
				const IRegistry& registry,
				const icomp::IComponentEnvironmentManager& manager);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual int GetComponentType() const;
	virtual IComponent* CreateComponent() const;
	virtual const istd::CString& GetDescription() const;
	virtual const istd::CString& GetKeywords() const;

protected:
	/**
		Get the element info for the given element ID. Method works recurive for the complex ID's.
	*/
	const IRegistry::ElementInfo* GetElementInfoFromRegistry(
				const IRegistry& registry,
				const std::string& elementId,
				const icomp::IComponentEnvironmentManager& manager) const;

	class AttrAsOptionalDelegator: virtual public IAttributeStaticInfo
	{
	public:
		AttrAsOptionalDelegator(
					const IAttributeStaticInfo* slavePtr,
					const iser::IObject* defaultValuePtr);

		virtual const std::string& GetAttributeDescription() const;
		virtual const iser::IObject* GetAttributeDefaultValue() const;
		virtual const std::string& GetAttributeTypeName() const;
		virtual const istd::CClassInfo& GetRelatedInterfaceType() const;
		virtual bool IsObligatory() const;

	private:
		const IAttributeStaticInfo& m_slave;
		const iser::IObject* m_defaultValuePtr;
	};

private:
	typedef istd::TDelPtr<IAttributeStaticInfo> AttrMetaInfoPtr;
	typedef std::map<const IAttributeStaticInfo*, AttrMetaInfoPtr> AttrReplacers;

	AttrReplacers m_attrReplacers;

	istd::CString m_description;
	istd::CString m_keywords;
	int m_category;
};


}//namespace icomp


#endif // !icomp_CCompositeComponentStaticInfo_included


