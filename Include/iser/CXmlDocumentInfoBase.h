#ifndef iser_CXmlDocumentInfoBase_included
#define iser_CXmlDocumentInfoBase_included


// STL includes
#include <map>


// ACF includes
#include "istd/CString.h"

#include "iser/iser.h"


namespace iser
{


/**
	Base class for all XML implementations for ACF documents.
	This helper class can be used e.g. to implement XML archives.

	\ingroup Persistence

*/
class CXmlDocumentInfoBase
{
public:
	/**
		Check if comments in XML document are enabled.
		It can be used e.g. to store tag descriptions as comments.
	*/
	bool IsCommentEnabled() const;
	/**
		Allows comments in XML document.
		\sa IsCommentEnabled()
	*/
	void SetCommentEnabled(bool state = true);

	// static methods
	static void EncodeXml(const std::string& text, std::string& xmlText);
	static void DecodeXml(const std::string& xmlText, std::string& text);
	static const istd::CString& GetElementSeparator();

protected:
	class CharToEntityMap: public std::map< char, std::string >
	{
	public:
		CharToEntityMap();
	};

	class EntityToChartMap: public std::map< std::string, char >
	{
	public:
		EntityToChartMap();
	};

private:
	bool m_isCommentEnabled;

	static CharToEntityMap s_charToEntityMap;
	static EntityToChartMap s_entityToChartMap;
	static istd::CString s_elementSeparator;
};


// inline methods

inline bool CXmlDocumentInfoBase::IsCommentEnabled() const
{
	return m_isCommentEnabled;
}


inline void CXmlDocumentInfoBase::SetCommentEnabled(bool state)
{
	m_isCommentEnabled = state;
}


} // namespace iser


#endif // !iser_CXmlDocumentInfoBase_included


