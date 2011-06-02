#ifndef iqtgui_CFileDialogSerializerComp_included
#define iqtgui_CFileDialogSerializerComp_included


// Qt includes
#include <QFileInfo>


// ACF includes
#include "istd/CString.h"

#include "iser/IFileLoader.h"
#include "iser/IFileLoaderInfo.h"

#include "ibase/TLoggerCompWrap.h"

#include "icomp/CComponentBase.h"

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

	I_BEGIN_COMPONENT(CFileDialogLoaderComp)
		I_REGISTER_INTERFACE(iser::IFileTypeInfo);
		I_REGISTER_INTERFACE(iser::IFileLoader)
		I_REGISTER_INTERFACE(iser::IFileLoaderInfo)
		I_ASSIGN_MULTI_0(m_loadersCompPtr, "Loaders", "List of file serializers will be used as slaves", true)
	I_END_COMPONENT

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				int flags = 0,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;

	// reimplemented (iser::IFileTypeInfo)
	virtual bool GetFileExtensions(istd::CStringList& result, int flags = 0, bool doAppend = false) const;
	virtual istd::CString GetTypeDescription(const istd::CString* extensionPtr = NULL) const;

	// reimplemented (iser::IFileLoaderInfo)
	virtual istd::CString GetLastFilePath(OperationType operationType = OT_UNKNOWN, PathType pathType = PT_COMPLETE) const;

	// static methods
	/**
		Append filters extracted from file loader to Qt filter list accepted by \c QFileDialog.
		\param	loader	file loader.
		\param	allExt	output list of all filters from extensions separated using semicolon (for example '*.bmp;*.png').
		\param	result	input/output list of filters with description separated with next line character.
		\return			number of filters.
	*/
	static int AppendLoaderFilterList(const iser::IFileTypeInfo& fileTypeInfo, int flags, QString& allExt, QString& result);

protected:
	istd::CString GetPathForType(const QFileInfo& fileInfo, PathType pathType) const;
	virtual QString GetFileName(const istd::CString& filePath, bool isSaving, int& selectionIndex) const;

	iser::IFileLoader* GetLoaderFor(const QString& filePath, int selectionIndex, int flags, bool beQuiet) const;

private:
	I_MULTIREF(iser::IFileLoader, m_loadersCompPtr);

	mutable QFileInfo m_lastOpenInfo;
	mutable QFileInfo m_lastSaveInfo;
};
	  

} // namespace iqtgui


#endif // !iqtgui_CFileDialogSerializerComp_included

