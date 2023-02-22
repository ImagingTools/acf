#include <iser/CJsonReadArchiveBase.h>


namespace iser
{


// public methods

CJsonReadArchiveBase::CJsonReadArchiveBase()
			: CTextReadArchiveBase(),
			m_rootTag("", "", iser::CArchiveTag::TT_GROUP),
			m_rootTagEnabled(false)
{

}


// reimplemented (iser::IArchive)

bool CJsonReadArchiveBase::BeginTag(const iser::CArchiveTag& tag)
{
	bool retVal = true;
	QString tagId(tag.GetId());
	int tagType = tag.GetTagType();

	if (m_iterators.isEmpty() && !tagId.isEmpty()){
		retVal = retVal && BeginTag(m_rootTag);
		m_rootTagEnabled = true;
	}

	if (m_iterators.isEmpty()){
		HelperIterator newHelperIterator;
		bool isObject = m_document.isObject();
		QJsonObject jsonObject = m_document.object();
		newHelperIterator.SetValue(jsonObject);
		newHelperIterator.SetKey(tagId);
		QString value = newHelperIterator.GetValue();
		m_iterators.push_back(newHelperIterator);

		return retVal;
	}

	HelperIterator helperIterator = m_iterators.last();

	if (helperIterator.isArray()){
		if (tagType == iser::CArchiveTag::TT_LEAF){

				return false;
		}
		QJsonObject jsonObject = helperIterator.GetObject();
		HelperIterator newHelperIterator;
		newHelperIterator.SetValue(jsonObject);
		newHelperIterator.SetKey(tagId);
		m_iterators.push_back(newHelperIterator);
	}
	else {
		QJsonObject jsonObject = helperIterator.GetObject();
		if (jsonObject.contains(tagId)){
			HelperIterator newHelperIterator;
			newHelperIterator.SetValue(jsonObject.value(tagId));
			newHelperIterator.SetKey(tagId);
			m_iterators.push_back(newHelperIterator);
		}
		else{
			if (IsLogConsumed()){
				SendLogMessage(
							istd::IInformationProvider::IC_ERROR,
							MI_TAG_ERROR,
							QString("Tag '%1' not found!").arg(QString(tagId)),
							"CJsonStringReadArchive",
							istd::IInformationProvider::ITF_SYSTEM);
			}

			return false;
		}
	}

	return retVal;
}


bool CJsonReadArchiveBase::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count)
{
	QString tagId(tag.GetId());
	HelperIterator helperIterator = m_iterators.last();
	QJsonObject jsonObject;

	if (helperIterator.isObject()){
		jsonObject = helperIterator.GetObject();
	}
	else{

		return false;
	}

	if (jsonObject.contains(tagId)){
		QJsonValue jsonValue = jsonObject.value(tagId);
		if (jsonValue.isArray()){
			HelperIterator newHelperIterator;
			newHelperIterator.SetValue(jsonValue);
			newHelperIterator.SetKey(tagId);
			m_iterators.push_back(newHelperIterator);
			count = jsonValue.toArray().count();
		}
	}
	else{
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Tag '%1' not found!").arg(QString(tagId)),
						"CJsonStringReadArchive",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	return true;
}


bool CJsonReadArchiveBase::EndTag(const iser::CArchiveTag& tag)
{

	if (m_iterators.isEmpty()){

		return false;
	}

	HelperIterator helperIterator = m_iterators.last();
	if (helperIterator.GetKey() == tag.GetId()){
		m_iterators.pop_back();
		if (!m_iterators.isEmpty() && m_iterators.last().isArray()){
			m_iterators.last().NextElementArray();
		}
	}
	else{
		SendLogMessage(
					istd::IInformationProvider::IC_ERROR,
					MI_TAG_ERROR,
					QString("Closing tag '%1' was not opened!").arg(QString(tag.GetId())),
					"JSON Reader",
					istd::IInformationProvider::ITF_SYSTEM);

		return false;
	}

	return true;
}


bool CJsonReadArchiveBase::Process(QString& value)
{
	return ReadStringNode(value);
}


// protected methods

bool CJsonReadArchiveBase::InitArchive(const QByteArray &inputString, bool serializeHeader)
{
	QJsonParseError error;
	m_document = QJsonDocument::fromJson(inputString, &error);

	if (error.error != QJsonParseError::NoError && IsLogConsumed()){
		SendLogMessage(
					istd::IInformationProvider::IC_ERROR,
					MI_TAG_ERROR,
					error.errorString(),
					"CJsonStringReadArchive",
					istd::IInformationProvider::ITF_SYSTEM);

		return false;
	}

//	BeginTag(m_rootTag);

	if (serializeHeader){
		SerializeAcfHeader();
	}

	return true;
}


bool CJsonReadArchiveBase::ReadStringNode(QString &text)
{
	HelperIterator helperIterator = m_iterators.last();
	QJsonObject jsonObject;

	if (!helperIterator.isObject() && !helperIterator.isArray()){
		text = helperIterator.GetValue();
	}
	else{
		return false;
	}

	return true;
}


bool CJsonReadArchiveBase::ReadTextNode(QByteArray &text)
{
	QString stringText;

	if (ReadStringNode(stringText)){
		text = stringText.toLocal8Bit();

		return true;
	}

	return false;
}

CJsonReadArchiveBase::HelperIterator::HelperIterator()
			: activeArrayIndex(0)
{

}

void CJsonReadArchiveBase::HelperIterator::SetKey(const QString &key)
{
	m_key = key;
}


const QString CJsonReadArchiveBase::HelperIterator::GetKey()
{
	return m_key;
}


QString CJsonReadArchiveBase::HelperIterator::GetValue()
{
	QString retVal;
	if (m_value.isBool()){
		retVal = m_value.toBool() ? "true" : "false";
	}
	else{
		if (m_value.isDouble()){
			retVal = QString::number(m_value.toDouble());
		}
		else{
			retVal = m_value.toString();
		}
	}

	return retVal;
}


QJsonObject CJsonReadArchiveBase::HelperIterator::GetObject()
{
	if (m_value.isObject()){

		return m_value.toObject();
	}

	if (m_value.isArray() && activeArrayIndex > -1 && activeArrayIndex < m_array.count()){

		return m_array[activeArrayIndex].toObject();
	}

	return QJsonObject();
}


bool CJsonReadArchiveBase::HelperIterator::NextElementArray()
{
	if (isArray()){
		activeArrayIndex++;

		return activeArrayIndex < m_array.count();
	}

	return false;
}


void CJsonReadArchiveBase::HelperIterator::SetValue(const QJsonValue value)
{
	m_value = value;

	if (m_value.isArray()){
		m_array = m_value.toArray();
		activeArrayIndex = 0;
	}
}


bool CJsonReadArchiveBase::HelperIterator::isObject()
{
	return m_value.isObject();
}


bool CJsonReadArchiveBase::HelperIterator::isArray()
{
	return m_value.isArray();
}


} // namespace iser


