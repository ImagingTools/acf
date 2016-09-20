#ifndef ilog_CTextFileLogStreamerComp_included
#define ilog_CTextFileLogStreamerComp_included


// Qt includes
#include <QtCore/QFile>
#include <QtCore/QTextStream>

// ACF includes
#include <ifile/IFileNameParam.h>
#include <ifile/IFilePersistence.h>
#include <ilog/CStreamLogCompBase.h>


namespace ilog
{


/**
	Message container displaying messages as log list.
*/
class CTextFileLogStreamerComp: 
			public CStreamLogCompBase,
			public ifile::IFilePersistence
{
public:
	typedef CStreamLogCompBase BaseClass;

	I_BEGIN_COMPONENT(CTextFileLogStreamerComp);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_ASSIGN(m_isAppendAttrPtr, "AppendToExisting", "Don't overwrite existing log file", true, false);
	I_END_COMPONENT;

	CTextFileLogStreamerComp();

	// reimplemented (ifile::IFilePersistence)
	virtual bool IsOperationSupported(
		const istd::IChangeable* dataObjectPtr,
		const QString* filePathPtr = NULL,
		int flags = -1,
		bool beQuiet = true) const;
	virtual int LoadFromFile(
				istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;
	virtual int SaveToFile(
				const istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr = NULL, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

protected:
	// reimplemented (CStreamLogCompBase)
	virtual void WriteText(const QString& text, istd::IInformationProvider::InformationCategory category);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	bool OpenFileStream(const QString& filePath);
	void CloseFileStream();

private:
	I_ATTR(bool, m_isAppendAttrPtr);

	mutable QFile m_outputFile;
	mutable QTextStream m_outputFileStream;
};


} // namespace ilog


#endif // !ilog_CTextFileLogStreamerComp_included


