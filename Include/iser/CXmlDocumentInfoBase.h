#ifndef iser_CXmlDocumentInfoBase_included
#define iser_CXmlDocumentInfoBase_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QMap>


namespace iser
{


/**
	Base class for all XML implementations for ACF documents.
	This helper class can be used e.g. to implement XML archives.
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
	static void EncodeXml(const QByteArray& text, QByteArray& xmlText);
	static void DecodeXml(const QByteArray& xmlText, QByteArray& text);
	static void EncodeXml(const QString& text, QByteArray& xmlText);
	static void DecodeXml(const QByteArray& xmlText, QString& text);
	static const QString& GetElementSeparator();

protected:
	class CharToEntityMap: public QMap<char, QByteArray>
	{
	public:
		CharToEntityMap();
	};

	class EntityToChartMap: public QMap<QByteArray, char>
	{
	public:
		EntityToChartMap();
	};

	class WideCharToEntityMap: public QMap<QChar, QByteArray >
	{
	public:
		WideCharToEntityMap();
	};

	class EntityToWideChartMap: public QMap<QByteArray, QChar>
	{
	public:
		EntityToWideChartMap();
	};

private:
	bool m_isCommentEnabled;

	static CharToEntityMap s_charToEntityMap;
	static EntityToChartMap s_entityToChartMap;
	static WideCharToEntityMap s_wideCharToEntityMap;
	static EntityToWideChartMap s_entityToWideChartMap;

	static QString s_elementSeparator;
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


