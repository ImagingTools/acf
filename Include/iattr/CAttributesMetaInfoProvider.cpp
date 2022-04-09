#include <iattr/CAttributesMetaInfoProvider.h>


namespace iattr
{


void CAttributesMetaInfoProvider::RemoveAllAttributeMetaInfos()
{
	m_attributesMetaInfoMap.clear();
}

bool CAttributesMetaInfoProvider::InsertAttributeMetaInfo(
			const QByteArray& attributeId,
			IAttributeMetaInfo* attributeMetaInfoPtr,
			bool releaseFlag)
{
	m_attributesMetaInfoMap[attributeId].SetPtr(attributeMetaInfoPtr, releaseFlag);

	return true;
}


bool CAttributesMetaInfoProvider::HasAttributeMetaId(const QByteArray& attributeId) const
{
	return m_attributesMetaInfoMap.contains(attributeId);
}


// reimplemented (iattr::IAttributesMetaInfoProvider)

IAttributesProvider::AttributeIds CAttributesMetaInfoProvider::GetAttributeMetaIds() const
{
#if QT_VERSION >= 0x060000
	QList<int> keys = m_attributesMetaInfoMap.keys();

	return QSet<int>(keys.begin(), keys.end());
#else
	return m_attributesMetaInfoMap.keys().toSet();
#endif
}



const IAttributeMetaInfo* CAttributesMetaInfoProvider::GetAttributeMetaInfo(const QByteArray& attributeId) const
{
	AttributesMetaInfoMap::ConstIterator findIter = m_attributesMetaInfoMap.constFind(attributeId);
	if (findIter != m_attributesMetaInfoMap.constEnd()){
		return findIter.value().GetPtr();
	}

	return NULL;
}


} // namespace iattr


