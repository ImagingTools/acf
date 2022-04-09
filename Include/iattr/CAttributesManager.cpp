#include <iattr/CAttributesManager.h>


// Qt includes
#include <QtCore/QMap>
#include <QtCore/QObject>

// ACF includes
#include <istd/CChangeGroup.h>
#include <imod/IModel.h>
#include <iattr/TAttribute.h>


namespace iattr
{


// static constants
static const istd::IChangeable::ChangeSet s_removeAllAttributesChange(CAttributesManager::CF_RESET, CAttributesManager::CF_ATTR_REMOVED, QObject::tr("Remove all attributes"));
static const istd::IChangeable::ChangeSet s_removeAttributeChange(CAttributesManager::CF_ATTR_REMOVED, QObject::tr("Remove attribute"));
static const istd::IChangeable::ChangeSet s_addAttributeChange(CAttributesManager::CF_ATTR_ADDED, QObject::tr("Add attribute"));
static const istd::IChangeable::ChangeSet s_insertGroupChange(QObject::tr("Insert attribute"));
static const iser::CArchiveTag s_attributesTag("Properties", "List of object properties", iser::CArchiveTag::TT_MULTIPLE);
static const iser::CArchiveTag s_attributeTag("Property", "Object property", iser::CArchiveTag::TT_GROUP, &s_attributesTag);
static const iser::CArchiveTag s_typeIdTag("PropertyTypeId", "ID of the property object", iser::CArchiveTag::TT_LEAF, &s_attributeTag);
static const iser::CArchiveTag s_idTag("PropertyId", "Name of the property object", iser::CArchiveTag::TT_LEAF, &s_attributeTag);
static const iser::CArchiveTag s_objectTag("PropertyObject", "Property object", iser::CArchiveTag::TT_GROUP, &s_attributeTag);



// public methods

CAttributesManager::CAttributesManager(const iser::IObjectFactory* factoryPtr)
:	m_attributesFactoryPtr(factoryPtr),
	m_attributesUpdateBridge(this)
{
}


void CAttributesManager::SetAttributesFactory(const iser::IObjectFactory* factoryPtr)
{
	m_attributesFactoryPtr = factoryPtr;
}


// reimplemented (iattr::IAttributesManager)

void CAttributesManager::RemoveAllAttributes()
{
	if (!m_attributesMap.isEmpty()){
		istd::CChangeNotifier notifier(this, &s_removeAllAttributesChange);
		Q_UNUSED(notifier);

		for (		AttributesMap::Iterator iter = m_attributesMap.begin();
					iter != m_attributesMap.end();
					++iter){
			AttributePtr& attributePtr = iter.value();
			imod::IModel* oldAttrModelPtr = dynamic_cast<imod::IModel*>(attributePtr.GetPtr());
			if ((oldAttrModelPtr != NULL) && m_attributesUpdateBridge.IsModelAttached(oldAttrModelPtr)){
				oldAttrModelPtr->DetachObserver(&m_attributesUpdateBridge);
			}
		}

		m_attributesMap.clear();
	}
}


bool CAttributesManager::InsertAttribute(
			const QByteArray& attributeId,
			iser::IObject* attributePtr,
			bool releaseFlag)
{
	istd::CChangeGroup changeGroup(this, &s_insertGroupChange);
	Q_UNUSED(changeGroup);

	AttributePtr& newAttributePtr = m_attributesMap[attributeId];

	if (newAttributePtr.IsValid()){
		istd::CChangeNotifier notifier(this, &s_removeAttributeChange);
		Q_UNUSED(notifier);

		imod::IModel* oldAttrModelPtr = dynamic_cast<imod::IModel*>(newAttributePtr.GetPtr());
		if ((oldAttrModelPtr != NULL) && m_attributesUpdateBridge.IsModelAttached(oldAttrModelPtr)){
			oldAttrModelPtr->DetachObserver(&m_attributesUpdateBridge);
		}
	}

	if (attributePtr != NULL){
		istd::CChangeNotifier notifier(this, &s_addAttributeChange);
		Q_UNUSED(notifier);

		newAttributePtr.SetPtr(attributePtr, releaseFlag);

		imod::IModel* attrModelPtr = dynamic_cast<imod::IModel*>(attributePtr);
		if (attrModelPtr != NULL){
			attrModelPtr->AttachObserver(&m_attributesUpdateBridge);
		}
	}
	else{
		m_attributesMap.remove(attributeId);
	}

	return true;
}


bool CAttributesManager::RemoveAttribute(const QByteArray& attributeId)
{
	if (m_attributesMap.contains(attributeId)){
		istd::CChangeNotifier notifier(this, &s_removeAttributeChange);
		Q_UNUSED(notifier);

		m_attributesMap.remove(attributeId);
	}

	return true;
}


// reimplemented (iattr::IAttributesProvider)

IAttributesProvider::AttributeIds CAttributesManager::GetAttributeIds() const
{
#if QT_VERSION >= 0x060000
	QList<int> keys = m_attributesMap.keys();

	return QSet<int>(keys.begin(), keys.end());
#else
	return m_attributesMap.keys().toSet();
#endif
}


iser::IObject* CAttributesManager::GetAttribute(const QByteArray& attributeId) const
{
	AttributesMap::ConstIterator findIter = m_attributesMap.constFind(attributeId);
	if (findIter != m_attributesMap.constEnd()){
		return findIter.value().GetPtr();
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CAttributesManager::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	bool isStoring = archive.IsStoring();

	int attributesCount = m_attributesMap.count();
	retVal = retVal && archive.BeginMultiTag(s_attributesTag, s_attributeTag, attributesCount);

	if (isStoring){
		for (		AttributesMap::ConstIterator iter = m_attributesMap.constBegin();
					iter != m_attributesMap.constEnd();
					++iter){
			QByteArray attributeId = iter.key();
			AttributePtr attributePtr = iter.value().GetPtr();

			if (attributePtr.IsValid()){
				QByteArray attributeTypeId = attributePtr->GetFactoryId();

				retVal = retVal && archive.BeginTag(s_attributeTag);

				retVal = retVal && archive.BeginTag(s_typeIdTag);
				retVal = retVal && archive.Process(attributeTypeId);
				retVal = retVal && archive.EndTag(s_typeIdTag);

				retVal = retVal && archive.BeginTag(s_idTag);
				retVal = retVal && archive.Process(attributeId);
				retVal = retVal && archive.EndTag(s_idTag);

				retVal = retVal && archive.BeginTag(s_objectTag);
				retVal = retVal && attributePtr->Serialize(archive);
				retVal = retVal && archive.EndTag(s_objectTag);

				retVal = retVal && archive.EndTag(s_attributeTag);
			}
		}

		retVal = retVal && archive.EndTag(s_attributesTag);
	}
	else{
		istd::CChangeNotifier notifier(this, &GetAllChanges());
		Q_UNUSED(notifier);

		if (m_attributesFactoryPtr == NULL){
			return false;
		}

		m_attributesMap.clear();

		for (int propertyIndex = 0; propertyIndex < attributesCount; ++propertyIndex){
			retVal = retVal && archive.BeginTag(s_attributeTag);

			QByteArray attributeId;
			QByteArray attributeTypeId;

			retVal = retVal && archive.BeginTag(s_typeIdTag);
			retVal = retVal && archive.Process(attributeTypeId);
			retVal = retVal && archive.EndTag(s_typeIdTag);

			retVal = retVal && archive.BeginTag(s_idTag);
			retVal = retVal && archive.Process(attributeId);
			retVal = retVal && archive.EndTag(s_idTag);

			if (!retVal){
				return false;
			}

			AttributePtr& attributePtr = m_attributesMap[attributeId];
			if (!attributePtr.IsValid()){
				attributePtr.SetPtr(m_attributesFactoryPtr->CreateInstance(attributeTypeId), true);

				if (!attributePtr.IsValid()){
					return false;
				}
			}

			retVal = retVal && archive.BeginTag(s_objectTag);
			retVal = retVal && attributePtr->Serialize(archive);
			retVal = retVal && archive.EndTag(s_objectTag);

			retVal = retVal && archive.EndTag(s_attributeTag);
		}

		retVal = retVal && archive.EndTag(s_attributesTag);
	}

	return retVal;
}


} // namespace iattr


