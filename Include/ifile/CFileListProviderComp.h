#ifndef ifile_CFileListProviderComp_included
#define ifile_CFileListProviderComp_included


// Qt includes
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QStringList>

// ACF includes
#include "iser/IFileLoader.h"

#include "imod/IModel.h"
#include "imod/CSingleModelObserverBase.h"

#include "icomp/CComponentBase.h"

#include "ifile/IFileListProvider.h"

#include "ifile/IFileNameParam.h"

#include "ifile/ifile.h"


namespace ifile
{


/**
	Provide list of file pathes using recursive search of some file directory.
*/
class CFileListProviderComp:
			public QObject,
			public icomp::CComponentBase,
			virtual public ifile::IFileListProvider,
			protected imod::CSingleModelObserverBase
{
	Q_OBJECT
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileListProviderComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(ifile::IFileListProvider);

		I_ASSIGN(m_dirParamCompPtr, "DirParam", "Parameter storing root directory", true, "DirParam");
		I_ASSIGN(m_dirParamModelCompPtr, "DirParam", "Parameter storing root directory", false, "DirParam");
		I_ASSIGN(m_fileLoaderCompPtr, "FileLoader", "File loader used to create file filters", false, "FileLoader");
		I_ASSIGN_MULTI_0(m_filtersAttrPtr, "Filters", "List of filters if no loader is specified", false);
		I_ASSIGN(m_minRecurDepthAttrPtr, "MinRecurDepth", "Minimal recursion depth for file search", true, 0);
		I_ASSIGN(m_maxRecurDepthAttrPtr, "MaxRecurDepth", "Maximal recursion depth for file search, negative value means no depth limit", true, 0);
	I_END_COMPONENT;

	// reimplemented (ifile::IFileListProvider)
	virtual QStringList GetFileList() const;

protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private Q_SLOTS:
	void OnDirectoryContentChanged(const QString& directoryPath);

private:
	I_REF(ifile::IFileNameParam, m_dirParamCompPtr);
	I_REF(imod::IModel, m_dirParamModelCompPtr);
	I_REF(iser::IFileLoader, m_fileLoaderCompPtr);
	I_MULTIATTR(QString, m_filtersAttrPtr);
	I_ATTR(int, m_minRecurDepthAttrPtr);
	I_ATTR(int, m_maxRecurDepthAttrPtr);

	QStringList m_fileList;

	QFileSystemWatcher m_directoryWatcher;

};


} // namespace ifile


#endif //!ifile_CFileListProviderComp_included


