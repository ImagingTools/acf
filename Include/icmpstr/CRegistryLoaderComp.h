#ifndef icmpstr_CRegistryLoaderComp_included
#define icmpstr_CRegistryLoaderComp_included


// Qt includes
#include <QObject>


// ACF includes
#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"

#include "ibase/TFileSerializerComp.h"


namespace icmpstr
{


/**
	Special registry loader supporting of loading layout data.
*/
class CRegistryLoaderComp:
	public QObject,
	public ibase::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive>
{
public:
	typedef ibase::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> BaseClass;

	I_BEGIN_COMPONENT(CRegistryLoaderComp);
	I_END_COMPONENT;

	enum MessageId
	{
		MI_LOAD_ERROR = 680,
		MI_CANNOT_READ_LAYOUT
	};

	// reimplemented (iser::IFileLoader)
	virtual int LoadFromFile(istd::IChangeable& data, const QString& filePath) const;
	virtual int SaveToFile(const istd::IChangeable& data, const QString& filePath) const;

	// reimplemented (iser::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

protected:
	QString GetLayoutPath(const QString& registryPath) const;

	// reimplemented (ibase::TFileSerializerComp)
	virtual void OnReadError(
				const iser::CXmlFileReadArchive& archive,
				const istd::IChangeable& data,
				const QString& filePath) const;

	using QObject::tr;
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryLoaderComp_included


