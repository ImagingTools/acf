#include "ibase/CCopyAppComp.h"


namespace ibase
{


// reimplemented (ibase::IApplication)

bool CCopyAppComp::InitializeApplication(int /*argc*/, char** /*argv*/)
{
	return true;
}


int CCopyAppComp::Execute(int argc, char** argv)
{
	if (!m_fileCopyCompPtr.IsValid()){
		SendErrorMessage(0, "File copy component not defined", "CopyApp");

		return 1;
	}

	QString inputFilePath;
	QString outputFilePath;

	for (int index = 1; index < argc; ++index){
		QByteArray argument = argv[index];
		if (!argument.isEmpty() && (argument[0] == '-')){
			QByteArray option = argument.mid(1);

			if (index < argc - 1){
				if (option == "o"){
					outputFilePath = QString::fromLocal8Bit(argv[index + 1]);
					++index;
				}
				else if ((option == "input") && *m_needExplicitInputAttrPtr){
					inputFilePath = QString::fromLocal8Bit(argv[index + 1]);
					++index;
				}
			}
		}
		else if (!*m_needExplicitInputAttrPtr && (index == 1)){
			inputFilePath = QString::fromLocal8Bit(argument);
		}
	}

	if (!m_fileCopyCompPtr->ConvertFile(inputFilePath, outputFilePath)){
		SendErrorMessage(0, QString("Copy of ") + inputFilePath + QString(" to ") + outputFilePath + " failed", "CopyApp");

		return 20;
	}

	return 0;
}


QString CCopyAppComp::GetHelpText() const
{
	return "General ACF copy application. Usage: <application> InputFilePath [-o OutputFilePath]";
}


} // namespace ibase


