#pragma once


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QMap>
#include <QtCore/QMutex>

// ACF includes
#include <icomp/IComponentContext.h>


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
				const IComponentContext* parentPtr,
				const QByteArray& contextId);

	QByteArray GetCompleteContextId() const;

	// reimplemented (icomp::IComponentContext)
	virtual const QByteArray& GetContextId() const override;
	virtual const IRegistryElement& GetRegistryElement() const override;
	virtual const IComponentStaticInfo& GetStaticInfo() const override;
	virtual const IComponentContext* GetParentContext() const override;
	virtual const iser::IObject* GetAttribute(const QByteArray& attributeId, int* definitionLevelPtr = NULL) const override;

	// static methods
	/**
		Get address of this component identifying it in component topology hierarchy.
	*/
	static QByteArray GetHierarchyAddress(const IComponentContext* contextPtr);

protected:
	struct AttributeInfo
	{
		const iser::IObject* attributePtr;
		int definitionLevel;
	};

	bool CalcAttributeInfo(const QByteArray& attributeId, AttributeInfo& result) const;

private:
	const IRegistryElement& m_registryElement;
	const IComponentStaticInfo& m_staticInfo;

	const IComponentContext* m_parentPtr;

	typedef QMap<QByteArray, AttributeInfo> AttributeMap;
	mutable AttributeMap m_attributeMap;

	QByteArray m_contextId;

#if QT_VERSION >= 0x060000
	mutable QRecursiveMutex m_attributeMapMutex;
#else
	mutable QMutex m_attributeMapMutex;
#endif
};


} // namespace icomp


