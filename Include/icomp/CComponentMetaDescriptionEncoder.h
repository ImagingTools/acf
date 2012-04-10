#ifndef icomp_CComponentMetaDescriptionEncoder_included
#define icomp_CComponentMetaDescriptionEncoder_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMap>


namespace icomp
{


/**
	Class to decode/encode the component category into human readable text.
*/
class CComponentMetaDescriptionEncoder
{
public:
	CComponentMetaDescriptionEncoder(const QString& metaDescription);

	/**
		Get meta keys using in the meta-description.
	*/
	const QStringList& GetMetaKeys() const;

	/**
		Get value list for the given key.
	*/
	const QStringList& GetValues(const QString& key = QString()) const;

	/**
		Get the keyword list.
	*/
	const QStringList& GetUnassignedKeywords() const;

private:
	typedef QMap<QString, QStringList> MetaValuesMap;

	MetaValuesMap m_metaValuesMap;
	QStringList m_metaKeys;
	QStringList m_keywords;
	QStringList m_unassignedKeywords;
};


} // namespace icomp


#endif // !icomp_CComponentMetaDescriptionEncoder_included


