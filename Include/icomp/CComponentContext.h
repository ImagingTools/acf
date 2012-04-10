#ifndef icomp_CComponentContext_included
#define icomp_CComponentContext_included


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QMap>

// ACF includes
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
	CComponentContext(
				const IRegistryElement* elementPtr,
				const IComponentStaticInfo* staticInfoPtr,
				const IComponentContext* parentPtr = NULL,
				const QByteArray& contextId = "");

	const QByteArray& GetContextId() const;

	// reimplemented (icomp::IComponentContext)
	virtual const IRegistryElement& GetRegistryElement() const;
	virtual const IComponentStaticInfo& GetStaticInfo() const;
	virtual const IComponentContext* GetParentContext() const;
	virtual const iser::IObject* GetAttribute(const QByteArray& attributeId, int* definitionLevelPtr = NULL) const;

private:
	const IRegistryElement& m_registryElement;
	const IComponentStaticInfo& m_staticInfo;

	const IComponentContext* m_parentPtr;

	typedef QMap<QByteArray, const iser::IObject*> AttributeMap;
	AttributeMap m_attributeMap;
	QByteArray m_contextId;
};


// inline methods

inline const QByteArray& CComponentContext::GetContextId() const
{
	return m_contextId;
}


} // namespace icomp


#endif // !icomp_CComponentContext_included


