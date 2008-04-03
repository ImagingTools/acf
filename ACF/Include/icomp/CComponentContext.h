#ifndef icomp_CComponentContext_included
#define icomp_CComponentContext_included


#include <map>

#include "istd/TDelPtr.h"

#include "icomp/IComponentContext.h"


namespace icomp
{


class IRegistryElement;


/**
	Base implementation of component session context.
	This implementation uses icomp::IRegistryElement to generate lilst of attributes.
	Please note that resolving of exported attribute is done.
*/
class CComponentContext: virtual public IComponentContext
{
public:
	CComponentContext(const IRegistryElement* elementPtr, const IComponentContext* parentPtr = NULL);

	// reimplemented (icomp::IComponentContext)
	virtual const IRegistryElement& GetRegistryElement() const;
	virtual const IComponentContext* GetParentContext() const;
	virtual const iser::ISerializable* GetAttribute(const std::string& attributeId, const IComponentContext** realContextPtr = NULL) const;
	virtual IComponent* GetSubcomponent(const std::string& componentId) const;

private:
	const IComponentContext* m_parentPtr;

	const IRegistryElement& m_registryElement;

	typedef std::map< std::string, const iser::ISerializable* > AttributeMap;
	AttributeMap m_attributeMap;
};


}//namespace icomp


#endif // !icomp_CComponentContext_included


