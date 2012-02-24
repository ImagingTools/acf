#include "icomp/CComponentMetaDescriptionEncoder.h"


namespace icomp
{


// public methods

CComponentMetaDescriptionEncoder::CComponentMetaDescriptionEncoder(const QString& metaDescription)
{
	m_metaValuesMap.clear();
	m_keywords.clear();

	QString currentKey;
	int wordBegin = 0;
	int quoteCount = 0;
	int apostrophCount = 0;

	int descriptionLength = int(metaDescription.length());
	for (int i = 0; i <= descriptionLength; ++i){
		if (i < descriptionLength){
			QChar ch = metaDescription[i];

			if (ch == '\''){
				if (apostrophCount <= 0){
					++apostrophCount;
					wordBegin = i + 1;
					continue;
				}
				else{
					--apostrophCount;
				}
			}
			else if (ch == '\"'){
				if (quoteCount <= 0){
					++quoteCount;
					wordBegin = i + 1;
					continue;
				}
				else{
					--quoteCount;
				}
			}
			else if (ch == '='){
				if (i > wordBegin){
					currentKey = metaDescription.mid(wordBegin, i - wordBegin);
				}

				wordBegin = i + 1;
				continue;
			}
			else if ((ch != ' ') || (quoteCount > 0)){
				continue;
			}
		}

		if (i > wordBegin){
			QString keyword = metaDescription.mid(wordBegin, i - wordBegin);
			m_keywords.push_back(keyword);
			if (!currentKey.isEmpty()){
				if (m_metaValuesMap.find(currentKey) == m_metaValuesMap.end()){
					m_metaKeys.push_back(currentKey);
				}

				m_metaValuesMap[currentKey].push_back(keyword);
			}
			else{
				m_unassignedKeywords.push_back(keyword);
			}

			if (apostrophCount <= 0){
				currentKey = "";
			}
		}

		wordBegin = i + 1;
	}
}


const QStringList& CComponentMetaDescriptionEncoder::GetMetaKeys() const
{
	return m_metaKeys;
}


const QStringList& CComponentMetaDescriptionEncoder::GetValues(const QString& key) const
{
	if (!key.isEmpty()){
		MetaValuesMap::const_iterator foundKeyIter = m_metaValuesMap.find(key);
		if (foundKeyIter != m_metaValuesMap.end()){
			return foundKeyIter->second;
		}
		else{
			static QStringList emptyList;

			return emptyList;
		}
	}
	else{
		return m_keywords;
	}
}


const QStringList& CComponentMetaDescriptionEncoder::GetUnassignedKeywords() const
{
	return m_unassignedKeywords;
}


} // namespace icomp


