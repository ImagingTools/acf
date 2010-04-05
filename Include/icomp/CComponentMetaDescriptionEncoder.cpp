#include "icomp/CComponentMetaDescriptionEncoder.h"


#include "istd/CRange.h"


namespace icomp
{


// public methods

CComponentMetaDescriptionEncoder::CComponentMetaDescriptionEncoder(const istd::CString& metaDescription)
{
	m_metaValuesMap.clear();
	m_keywords.clear();

	istd::CString currentKey;
	int wordBegin = 0;
	int quoteCount = 0;
	int apostrophCount = 0;

	int descriptionLength = int(metaDescription.length());
	for (int i = 0; i <= descriptionLength; ++i){
		if (i < descriptionLength){
			istd::CString::value_type ch = metaDescription[i];

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
					currentKey = metaDescription.substr(wordBegin, i - wordBegin);
				}

				wordBegin = i + 1;
				continue;
			}
			else if ((ch != ' ') || (quoteCount > 0)){
				continue;
			}
		}

		if (i > wordBegin){
			istd::CString keyword = metaDescription.substr(wordBegin, i - wordBegin);
			m_keywords.push_back(keyword);
			if (!currentKey.IsEmpty()){
				if (m_metaValuesMap.find(currentKey) == m_metaValuesMap.end()){
					m_metaKeys.push_back(currentKey);
				}

				m_metaValuesMap[currentKey].push_back(keyword);
			}

			if (apostrophCount <= 0){
				currentKey = "";
			}
		}

		wordBegin = i + 1;
	}
}


const istd::CStringList& CComponentMetaDescriptionEncoder::GetMetaKeys() const
{
	return m_metaKeys;
}


const istd::CStringList& CComponentMetaDescriptionEncoder::GetValues(const istd::CString& key) const
{
	if (!key.IsEmpty()){
		MetaValuesMap::const_iterator foundKeyIter = m_metaValuesMap.find(key);
		if (foundKeyIter != m_metaValuesMap.end()){
			return foundKeyIter->second;
		}
		else{
			static istd::CStringList emptyList;

			return emptyList;
		}
	}
	else{
		return m_keywords;
	}
}


} // namespace icomp


