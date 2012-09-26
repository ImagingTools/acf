#ifndef ibase_CExternalFileConverterComp_included
#define ibase_CExternalFileConverterComp_included


// Qt includes
#include <QtCore/QProcess>

// ACF includes
#include "ibase/IFileConvertCopy.h"
#include "ibase/TLoggerCompWrap.h"

#include "iprm/IFileNameParam.h"


namespace ibase
{


/**
	File converter, which uses an external programm to perform the convert action. 
*/
class CExternalFileConverterComp:
			public QObject,
			public ibase::CLoggerComponentBase,
			virtual public IFileConvertCopy
{
	Q_OBJECT

public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CExternalFileConverterComp);
		I_REGISTER_INTERFACE(IFileConvertCopy);
		I_ASSIGN(m_executablePathCompPtr, "ExecutablePath", "Path to the application's binary", true, "ExecutablePath");
		I_ASSIGN(m_defaultProcessArgumentsAttrPtr, "DefaultProcessArguments", "Application conversion arguments.\nUse $(Input) to specify the input and $(Output) for output file name", false, "$(Input) $(Output)");
		I_ASSIGN(m_processArgumentsParamsIdAttrPtr, "ProcessArgumentsParamsId", "ID of the command line parameter (given as a INameParam object) in the parameter set", true, "ProcessArgumentsParamsId");
	I_END_COMPONENT;

	// reimplemented (ibase::IFileConvertCopy)
	virtual bool ConvertFile(
				const QString& inputFilePath,
				const QString& outputFilePath,
				const iprm::IParamsSet* paramsPtr = NULL) const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private Q_SLOTS:
	void OnReadyReadStandardError();
	void OnReadyReadStandardOutput();

private:
	I_REF(iprm::IFileNameParam, m_executablePathCompPtr);
	I_ATTR(QString, m_defaultProcessArgumentsAttrPtr);
	I_ATTR(QByteArray, m_processArgumentsParamsIdAttrPtr);

	mutable QProcess m_conversionProcess;
};


} // namespace ibase


#endif // !ibase_CExternalFileConverterComp_included


