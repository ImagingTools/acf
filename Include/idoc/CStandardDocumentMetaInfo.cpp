#include "idoc/CStandardDocumentMetaInfo.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace idoc
{


// public methods

// reimplemented (IDocumentMetaInfo)

CStandardDocumentMetaInfo::MetaInfoTypes CStandardDocumentMetaInfo::GetSupportedMetaInfoTypes() const
{
	MetaInfoTypes retVal;

	retVal.insert(MIT_AUTHOR);
	retVal.insert(MIT_CREATOR);
	retVal.insert(MIT_DESCRIPTION);
	retVal.insert(MIT_CREATION_TIME);
	retVal.insert(MIT_MODIFICATION_TIME);

	return retVal;
}


QVariant CStandardDocumentMetaInfo::GetDocumentMetaInfo(int metaInfoType) const
{
	static QVariant emptyValue;

	if (m_infosMap.contains(metaInfoType)){
		return m_infosMap[metaInfoType];
	}

	return emptyValue;
}


bool CStandardDocumentMetaInfo::SetDocumentMetaInfo(int metaInfoType, const QVariant& metaInfo)
{
	MetaInfoTypes registeredTypes = GetSupportedMetaInfoTypes();

	if (registeredTypes.contains(metaInfoType)){
		
		if (m_infosMap[metaInfoType] != metaInfo){
			istd::CChangeNotifier changePtr(this);

			m_infosMap[metaInfoType] = metaInfo;
		}

		return true;
	}

	qDebug("Meta info type is not supported by this implementation. You should re-implement GetSupportedMetaInfoTypes method for adding user specific meta info types");

	return false;
}


QString CStandardDocumentMetaInfo::GetMetaInfoName(int metaInfoType) const
{
	static QString emptyName;

	switch (metaInfoType){
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
	}

	return emptyName;
}


QString CStandardDocumentMetaInfo::GetMetaInfoDescription(int metaInfoType) const
{
	static QString emptyName;

	switch (metaInfoType){
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
	}

	return emptyName;
}


// reimplemented (iser::ISerializable)

bool CStandardDocumentMetaInfo::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag metaInfoMapTag("MetaInfoMap", "Mapping between meta info and its type");
	static iser::CArchiveTag metaInfoTag("MetaInfo", "Single meta information");
	static iser::CArchiveTag typeTag("Type", "Type of the meta information");
	static iser::CArchiveTag valueTag("Value", "Value of the meta information");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

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
	if (infoPtr != NULL)
	{
		return (m_infosMap == infoPtr->m_infosMap);
	}

	return false;
}


} // namespace idoc


