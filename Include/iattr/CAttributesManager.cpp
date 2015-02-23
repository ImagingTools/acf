#include "iattr/CAttributesManager.h"


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "imod/IModel.h"

// ACF-Solutions includes
#include "imod/IModel.h"
#include "iattr/TAttribute.h"
#include "iattr/TMultiAttribute.h"


namespace iattr
{


// public methods

CAttributesManager::CAttributesManager(const iser::IObjectFactory* factoryPtr)
:	m_attributesFactoryPtr(factoryPtr)
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
		ChangeSet changeSet(CF_RESET, CF_ATTR_REMOVED);
		istd::CChangeNotifier notifier(this, changeSet);
		Q_UNUSED(notifier);

		for (		AttributesMap::Iterator iter = m_attributesMap.begin();
					iter != m_attributesMap.end();
					++iter){
			AttributePtr& attributePtr = iter.value();
			imod::IModel* oldAttrModelPtr = dynamic_cast<imod::IModel*>(attributePtr.GetPtr());
			if ((oldAttrModelPtr != NULL) && IsModelAttached(oldAttrModelPtr)){
				oldAttrModelPtr->DetachObserver(this);
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
	ChangeSet changeSet(CF_ATTR_ADDED);
	istd::CChangeNotifier notifier(this, changeSet);
	Q_UNUSED(notifier);

	AttributePtr& newAttributePtr = m_attributesMap[attributeId];
	imod::IModel* oldAttrModelPtr = dynamic_cast<imod::IModel*>(newAttributePtr.GetPtr());
	if ((oldAttrModelPtr != NULL) && IsModelAttached(oldAttrModelPtr)){
		oldAttrModelPtr->DetachObserver(this);
	}

	newAttributePtr.SetPtr(attributePtr, releaseFlag);

	imod::IModel* attrModelPtr = dynamic_cast<imod::IModel*>(attributePtr);
	if (attrModelPtr != NULL){
		attrModelPtr->AttachObserver(this);
	}

	return true;
}


// reimplemented (iattr::IAttributesProvider)

IAttributesProvider::AttributeIds CAttributesManager::GetAttributeIds() const
{
	return m_attributesMap.keys().toSet();
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
	static iser::CArchiveTag attributesTag("Properties", "List of object properties", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag attributeTag("Property", "Object property", iser::CArchiveTag::TT_GROUP, &attributesTag);
	static iser::CArchiveTag typeIdTag("PropertyTypeId", "ID of the property object", iser::CArchiveTag::TT_LEAF, &attributeTag);
	static iser::CArchiveTag idTag("PropertyId", "Name of the property object", iser::CArchiveTag::TT_LEAF, &attributeTag);
	static iser::CArchiveTag objectTag("PropertyObject", "Property object", iser::CArchiveTag::TT_GROUP, &attributeTag);

	bool retVal = true;

	bool isStoring = archive.IsStoring();

	int attributesCount = m_attributesMap.count();
	retVal = retVal && archive.BeginMultiTag(attributesTag, attributeTag, attributesCount);

	if (isStoring){
		for (		AttributesMap::ConstIterator iter = m_attributesMap.constBegin();
					iter != m_attributesMap.constEnd();
					++iter){
			QByteArray attributeId = iter.key();
			AttributePtr attributePtr = iter.value().GetPtr();

			if (attributePtr.IsValid()){
				QByteArray attributeTypeId = attributePtr->GetFactoryId();

				retVal = retVal && archive.BeginTag(attributeTag);

				retVal = retVal && archive.BeginTag(typeIdTag);
				retVal = retVal && archive.Process(attributeTypeId);
				retVal = retVal && archive.EndTag(typeIdTag);

				retVal = retVal && archive.BeginTag(idTag);
				retVal = retVal && archive.Process(attributeId);
				retVal = retVal && archive.EndTag(idTag);

				retVal = retVal && archive.BeginTag(objectTag);
				retVal = retVal && attributePtr->Serialize(archive);
				retVal = retVal && archive.EndTag(objectTag);

				retVal = retVal && archive.EndTag(attributeTag);
			}
		}

		retVal = retVal && archive.EndTag(attributesTag);
	}
	else{
		istd::CChangeNotifier notifier(this, GetAllChanges());
		Q_UNUSED(notifier);

		if (m_attributesFactoryPtr == NULL){
			return false;
		}

		m_attributesMap.clear();

		for (int propertyIndex = 0; propertyIndex < attributesCount; ++propertyIndex){
			retVal = retVal && archive.BeginTag(attributeTag);

			QByteArray attributeId;
			QByteArray attributeTypeId;

			retVal = retVal && archive.BeginTag(typeIdTag);
			retVal = retVal && archive.Process(attributeTypeId);
			retVal = retVal && archive.EndTag(typeIdTag);

			retVal = retVal && archive.BeginTag(idTag);
			retVal = retVal && archive.Process(attributeId);
			retVal = retVal && archive.EndTag(idTag);

			if (!retVal){
				return false;
			}

			AttributePtr& attributePtr = m_attributesMap[attributeTypeId];
			if (!attributePtr.IsValid()){
				attributePtr.SetPtr(m_attributesFactoryPtr->CreateInstance(attributeTypeId), true);

				if (!attributePtr.IsValid()){
					return false;
				}
			}

			retVal = retVal && attributePtr->Serialize(archive);

			retVal = retVal && archive.EndTag(attributeTag);
		}

		retVal = retVal && archive.EndTag(attributesTag);
	}

	return retVal;
}


// protected methods

// reimplemented (imod::IObserver)

void CAttributesManager::BeforeUpdate(imod::IModel* /*modelPtr*/)
{
	BeginChanges(GetDelegatedChanges());
}


void CAttributesManager::AfterUpdate(imod::IModel* /*modelPtr*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	EndChanges(GetDelegatedChanges());
}


} // namespace iattr


