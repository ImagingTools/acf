#include "idoc/CStandardDocumentMetaInfo.h"


// Qt includes
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>

// ACF includes
#include "istd/CChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace idoc
{


// public methods

// reimplemented (idoc::IDocumentMetaInfo)

CStandardDocumentMetaInfo::MetaInfoTypes CStandardDocumentMetaInfo::GetMetaInfoTypes(bool /*allowReadOnly*/) const
{
	return m_infosMap.keys().toSet();
}


QVariant CStandardDocumentMetaInfo::GetMetaInfo(int metaInfoType) const
{
	static QVariant emptyValue;

	if (m_infosMap.contains(metaInfoType)){
		return m_infosMap[metaInfoType];
	}

	return emptyValue;
}


bool CStandardDocumentMetaInfo::SetMetaInfo(int metaInfoType, const QVariant& metaInfo)
{
	if (m_infosMap[metaInfoType] != metaInfo){
		static ChangeSet changeSet(CF_METAINFO);
		istd::CChangeNotifier changePtr(this, changeSet);

		m_infosMap[metaInfoType] = metaInfo;
	}

	return true;
}


QString CStandardDocumentMetaInfo::GetMetaInfoName(int metaInfoType) const
{
	static QString emptyName;

	switch (metaInfoType){
	case MIT_TITLE:
		return QObject::tr("Title");

	case MIT_AUTHOR:
		return QObject::tr("Author");

	case MIT_CREATOR:
		return QObject::tr("Creator");

	case MIT_DESCRIPTION:
		return QObject::tr("Description");

	case MIT_CREATION_TIME:
		return QObject::tr("Creation Time");

	case MIT_MODIFICATION_TIME:
		return QObject::tr("Modification Time");

	default:
		return emptyName;
	}
}


QString CStandardDocumentMetaInfo::GetMetaInfoDescription(int metaInfoType) const
{
	static QString emptyName;

	switch (metaInfoType){
	case MIT_TITLE:
		return QObject::tr("Title of the document");

	case MIT_AUTHOR:
		return QObject::tr("Author of the document");

	case MIT_CREATOR:
		return QObject::tr("Creator of the document");

	case MIT_DESCRIPTION:
		return QObject::tr("Document description");

	case MIT_CREATION_TIME:
		return QObject::tr("Time of document's creation");

	case MIT_MODIFICATION_TIME:
		return QObject::tr("Time of last document's modification");

	default:
		return emptyName;
	}
}


bool CStandardDocumentMetaInfo::IsMetaInfoWritable(int metaInfoType) const
{
	switch (metaInfoType){
		case MIT_CREATION_TIME:
		case MIT_MODIFICATION_TIME:
			return false;
		default:
			return true;
	}
}


// reimplemented (iser::ISerializable)

bool CStandardDocumentMetaInfo::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag metaInfoMapTag("MetaInfoMap", "Mapping between meta info and its type", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag metaInfoTag("MetaInfo", "Single meta information", iser::CArchiveTag::TT_GROUP, &metaInfoMapTag);
	static iser::CArchiveTag typeTag("Type", "Type of the meta information", iser::CArchiveTag::TT_LEAF, &metaInfoTag);
	static iser::CArchiveTag valueTag("Value", "Value of the meta information", iser::CArchiveTag::TT_LEAF, &metaInfoTag);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	int metaInfosCount = m_infosMap.size();
	retVal = retVal && archive.BeginMultiTag(metaInfoMapTag, metaInfoTag, metaInfosCount);

	if (archive.IsStoring()){
		for (		MetaInfoMap::ConstIterator index = m_infosMap.constBegin();
					index != m_infosMap.constEnd();
					++index){
			retVal = retVal && archive.BeginTag(metaInfoTag);

			int type = index.key();
			retVal = retVal && archive.BeginTag(typeTag);
			retVal = retVal && archive.Process(type);
			retVal = retVal && archive.EndTag(typeTag);


			QByteArray variantData;
			QDataStream variantStream(&variantData, QIODevice::ReadWrite);
			QVariant value = index.value();
			variantStream << value;

			retVal = retVal && archive.BeginTag(valueTag);
			retVal = retVal && archive.Process(variantData);
			retVal = retVal && archive.EndTag(valueTag);

			retVal = retVal && archive.EndTag(metaInfoTag);
		}
	}
	else{
		static ChangeSet changeSet(CF_ALL_DATA, CF_METAINFO);
		istd::CChangeNotifier changePtr(this, changeSet);

		m_infosMap.clear();

		for (int itemIndex = 0; itemIndex < metaInfosCount; ++itemIndex){
			retVal = retVal && archive.BeginTag(metaInfoTag);

			int type;
			retVal = retVal && archive.BeginTag(typeTag);
			retVal = retVal && archive.Process(type);
			retVal = retVal && archive.EndTag(typeTag);

			QByteArray variantData;
			retVal = retVal && archive.BeginTag(valueTag);
			retVal = retVal && archive.Process(variantData);
			retVal = retVal && archive.EndTag(valueTag);

			QVariant variantValue;
			QDataStream variantStream(variantData);
			variantStream >> variantValue;

			if (retVal){
				m_infosMap[type] = variantValue;
			}

			retVal = retVal && archive.EndTag(metaInfoTag);
		}
	}

	retVal = retVal && archive.EndTag(metaInfoMapTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CStandardDocumentMetaInfo::IsEqual(const IChangeable& object) const
{
	const CStandardDocumentMetaInfo* infoPtr = dynamic_cast<const CStandardDocumentMetaInfo*>(&object);
	if (infoPtr != NULL){
		return (m_infosMap == infoPtr->m_infosMap);
	}

	return false;
}


bool CStandardDocumentMetaInfo::CopyFrom(const IChangeable& object, CompatibilityMode /*mode = CM_WITHOUT_REFS*/)
{
	const CStandardDocumentMetaInfo* infoPtr = dynamic_cast<const CStandardDocumentMetaInfo*>(&object);
	if (infoPtr != NULL){
		m_infosMap = infoPtr->m_infosMap;
		return true;
	}

	return false;
}


} // namespace idoc


