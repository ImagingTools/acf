#ifndef icomp_CComponentMetaDescriptionEncoder_included
#define icomp_CComponentMetaDescriptionEncoder_included


// STL includes
#include <map>

// ACF includes
#include "istd/CString.h"


namespace icomp
{


/**
	Class to decode/encode the component category into human readable text.
*/
class CComponentMetaDescriptionEncoder
{
public:
	CComponentMetaDescriptionEncoder(const istd::CString& metaDescription);

	/**
		Get meta keys using in the meta-description.
	*/
	const istd::CStringList& GetMetaKeys() const;

	/**
		Get value list for the given key.
	*/
	const istd::CStringList& GetValues(const istd::CString& key = istd::CString()) const;

	/**
		Get the keyword list.
	*/
	const istd::CStringList& GetUnassignedKeywords() const;

private:
	typedef std::map<istd::CString, istd::CStringList> MetaValuesMap;

	MetaValuesMap m_metaValuesMap;
	istd::CStringList m_metaKeys;
	istd::CStringList m_keywords;
	istd::CStringList m_unassignedKeywords;
};


} // namespace icomp


#endif // !icomp_CComponentMetaDescriptionEncoder_included


