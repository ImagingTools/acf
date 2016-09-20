#include <icomp/CComponentContext.h>


#include <icomp/IRegistryElement.h>
#include <icomp/IComponentStaticInfo.h>
#include <icomp/IAttributeStaticInfo.h>


namespace icomp
{


CComponentContext::CComponentContext(
			const IRegistryElement* elementPtr,
			const IComponentStaticInfo* staticInfoPtr,
			const IComponentContext* parentPtr,
			const QByteArray& contextId)
:	m_registryElement(*elementPtr),
	m_staticInfo(*staticInfoPtr),
	m_parentPtr(parentPtr),
	m_contextId(contextId)
{
	Q_ASSERT(elementPtr != NULL);
	Q_ASSERT(staticInfoPtr != NULL);
}


QByteArray CComponentContext::GetCompleteContextId() const
{
	QByteArray completeIdPath = m_contextId;
	for (		const IComponentContext* partContextPtr = m_parentPtr;
				partContextPtr != NULL;
				partContextPtr = partContextPtr->GetParentContext()){
		completeIdPath = partContextPtr->GetContextId() + "/" + completeIdPath;
	}

	return completeIdPath;
}


// reimplemented (icomp::IComponentContext)

const QByteArray& CComponentContext::GetContextId() const
{
	return m_contextId;
}


const IRegistryElement& CComponentContext::GetRegistryElement() const
{
	return m_registryElement;
}


const IComponentStaticInfo& CComponentContext::GetStaticInfo() const
{
	return m_staticInfo;
}


const IComponentContext* CComponentContext::GetParentContext() const
{
	return m_parentPtr;
}


const iser::IObject* CComponentContext::GetAttribute(const QByteArray& attributeId, int* definitionLevelPtr) const
{
	AttributeMap::ConstIterator findIter = m_attributeMap.constFind(attributeId);
	if (findIter == m_attributeMap.constEnd()){
		AttributeInfo data;

		CalcAttributeInfo(attributeId, data);

		m_attributeMap[attributeId] = data;

		if (definitionLevelPtr != NULL){
			*definitionLevelPtr = data.definitionLevel;
		}

		return data.attributePtr;
	}
	else{
		const AttributeInfo& data = findIter.value();

		if (definitionLevelPtr != NULL){
			*definitionLevelPtr = data.definitionLevel;
		}

		return data.attributePtr;
	}
}


// static methods

QByteArray CComponentContext::GetHierarchyAddress(const IComponentContext* contextPtr)
{
	QByteArray retVal;

	while (contextPtr != NULL){
		retVal = contextPtr->GetContextId()  + "/" + retVal;

		contextPtr = contextPtr->GetParentContext();
	}

	return retVal;
}


// protected methods

bool CComponentContext::CalcAttributeInfo(const QByteArray& attributeId, AttributeInfo& result) const
{
	const IRegistryElement::AttributeInfo* infoPtr = m_registryElement.GetAttributeInfo(attributeId);

	if (infoPtr != NULL){
		const QByteArray& exportId = infoPtr->exportId;
		if (!exportId.isEmpty() && (m_parentPtr != NULL)){
			int parentLevel = -1;

			const iser::IObject* parentAttributePtr = m_parentPtr->GetAttribute(exportId, &parentLevel);
			Q_ASSERT((parentAttributePtr == NULL) || (parentLevel >= 0));	// if attribute is retured parent level must be set

			if (parentAttributePtr != NULL){
				result.definitionLevel = parentLevel + 1;
				result.attributePtr = parentAttributePtr;

				return true;
			}
		}

		if (infoPtr->attributePtr.IsValid()){
			result.definitionLevel = 0;
			result.attributePtr = infoPtr->attributePtr.GetPtr();

			return true;
		}
	}

	const IAttributeStaticInfo* attributeInfoPtr = m_staticInfo.GetAttributeInfo(attributeId);
	if (attributeInfoPtr != NULL){
		int attributeFlags = attributeInfoPtr->GetAttributeFlags();

		static const int UseDefaultFlags = IAttributeStaticInfo::AF_OBLIGATORY | IAttributeStaticInfo::AF_VALUE;
		if ((attributeFlags & UseDefaultFlags) == UseDefaultFlags){
			const iser::IObject* defaultAttributePtr = attributeInfoPtr->GetAttributeDefaultValue();
			if (defaultAttributePtr != NULL){
				result.definitionLevel = 0;
				result.attributePtr = defaultAttributePtr;

				return true;
			}
		}

		if ((attributeFlags & IAttributeStaticInfo::AF_NULLABLE) == 0){
			qCritical(	"Component %s: Attribute %s was not set!",
						GetCompleteContextId().constData(),
						attributeId.constData());
		}
	}
	else{
		qCritical(	"Component %s: Internal attribute logic for %s failed!",
					GetCompleteContextId().constData(),
					attributeId.constData());
	}

	result.definitionLevel = -1;
	result.attributePtr = NULL;

	return false;
}


} // namespace icomp


