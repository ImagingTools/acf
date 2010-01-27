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
	CComponentContext(const IRegistryElement* elementPtr, const IComponentContext* parentPtr = NULL, const std::string& contextId = "");

	const std::string& GetContextId() const;

	// reimplemented (icomp::IComponentContext)
	virtual const IRegistryElement& GetRegistryElement() const;
	virtual const IComponentContext* GetParentContext() const;
	virtual const iser::IObject* GetAttribute(const std::string& attributeId, int* definitionLevelPtr = NULL) const;

private:
	const IRegistryElement& m_registryElement;

	const IComponentContext* m_parentPtr;

	typedef std::map<std::string, const iser::IObject*> AttributeMap;
	AttributeMap m_attributeMap;
	std::string m_contextId;
};


// inline methods

inline const std::string& CComponentContext::GetContextId() const
{
	return m_contextId;
}


}//namespace icomp


#endif // !icomp_CComponentContext_included


