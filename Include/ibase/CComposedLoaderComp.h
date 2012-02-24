#ifndef ibase_CComposedLoaderComp_included
#define ibase_CComposedLoaderComp_included


#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"


namespace ibase
{


/**
	Loader component, which delegates the persistence functionality to its slave loaders.
*/
class CComposedLoaderComp:
			public icomp::CComponentBase,
			virtual public iser::IFileLoader
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CComposedLoaderComp);
		I_REGISTER_INTERFACE(iser::IFileTypeInfo);
		I_REGISTER_INTERFACE(iser::IFileLoader);
		I_ASSIGN_MULTI_0(m_slaveLoadersCompPtr, "SlaveLoaders", "List of slave loaders", true);
		I_ASSIGN(m_commonDescriptionAttrPtr, "CommonDescription", "Optional common description of this file type", false, "Composed");
	I_END_COMPONENT;

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const QString& filePath = QString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const QString& filePath = QString()) const;

	// reimplemented (iser::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

private:
	I_MULTIREF(iser::IFileLoader, m_slaveLoadersCompPtr);
	I_ATTR(QString, m_commonDescriptionAttrPtr);
};


} // namespace ibase


#endif // !ibase_CComposedLoaderComp_included


