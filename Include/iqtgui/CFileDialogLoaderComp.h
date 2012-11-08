#ifndef iqtgui_CFileDialogSerializerComp_included
#define iqtgui_CFileDialogSerializerComp_included


// Qt includes
#include <QtCore/QFileInfo>


// ACF includes
#include <QtCore/QString>

#include "iser/IFileLoader.h"
#include "iser/IFileLoaderInfo.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "ifile/IFileNameParam.h"

#include "iqtgui/iqtgui.h"


namespace iqtgui
{

/**
	File dialog component. 
	
	File dialog will be created according to set of the slave file loaders.
*/
class CFileDialogLoaderComp: 
			public QObject,
			public ibase::CLoggerComponentBase, 
			virtual public iser::IFileLoader,
			virtual public iser::IFileLoaderInfo
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_NONE_SUPPORTS = 0xf8e0
	};

	I_BEGIN_COMPONENT(CFileDialogLoaderComp);
		I_REGISTER_INTERFACE(iser::IFileTypeInfo);
		I_REGISTER_INTERFACE(iser::IFileLoader);
		I_REGISTER_INTERFACE(iser::IFileLoaderInfo);
		I_ASSIGN(m_useNativeAttrPtr, "UseNative", "Use native system file dialog", true, true);
		I_ASSIGN_MULTI_0(m_loadersCompPtr, "Loaders", "List of file serializers will be used as slaves", true);
		I_ASSIGN(m_statupDirectoryCompPtr, "StartupDirectory", "Initial directory path for the file loader dialog", false, "StartupDirectory");
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

	// reimplemented (iser::IFileLoaderInfo)
	virtual QString GetLastFilePath(OperationType operationType = OT_UNKNOWN, PathType pathType = PT_COMPLETE) const;

	// static methods
	/**
		Append filters extracted from file loader to Qt filter list accepted by \c QFileDialog.
		\param	loader	file loader.
		\param	allExt	output list of all filters from extensions separated using semicolon (for example '*.bmp;*.png').
		\param	result	input/output list of filters with description separated with next line character.
		\return			number of filters.
	*/
	static void AppendLoaderFilterList(const iser::IFileTypeInfo& fileTypeInfo, int flags, QString& allExt, QStringList& result);

protected:
	QString GetPathForType(const QFileInfo& fileInfo, PathType pathType) const;
	virtual QString GetFileName(const QString& filePath, bool isSaving, int& selectionIndex) const;

	iser::IFileLoader* GetLoaderFor(const QString& filePath, int selectionIndex, int flags, bool beQuiet) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(bool, m_useNativeAttrPtr);
	I_MULTIREF(iser::IFileLoader, m_loadersCompPtr);
	I_REF(ifile::IFileNameParam, m_statupDirectoryCompPtr);

	mutable QFileInfo m_lastOpenInfo;
	mutable QFileInfo m_lastSaveInfo;
};
	  

} // namespace iqtgui


#endif // !iqtgui_CFileDialogSerializerComp_included

