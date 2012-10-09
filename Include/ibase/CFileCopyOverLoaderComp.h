#ifndef ibase_CFileCopyOverLoaderComp_included
#define ibase_CFileCopyOverLoaderComp_included


#include "istd/IChangeable.h"

#include "iser/IFileLoader.h"

#include "ibase/IFileConvertCopy.h"
#include "ibase/TLoggerCompWrap.h"


namespace ibase
{


/**
	General file copy or transformation.
	This implementation uses general file loader, saver and any object instance.
*/
class CFileCopyOverLoaderComp:
			public ibase::CLoggerComponentBase,
			virtual public IFileConvertCopy
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileCopyOverLoaderComp);
		I_REGISTER_INTERFACE(IFileConvertCopy);

		I_ASSIGN(m_objectCompPtr, "Object", "Object used as representation of copied data", true, "Object");
		I_ASSIGN(m_inputLoaderCompPtr, "InputLoader", "input file loader", true, "InputLoader");
		I_ASSIGN(m_outputLoaderCompPtr, "OutputLoader", "output file loader", true, "OutputLoader");
	I_END_COMPONENT;

	// reimplemented (ibase::IFileConvertCopy)
	virtual bool ConvertFiles(
				const QString& inputPath,
				const QString& outputPath,
				const iprm::IParamsSet* paramsPtr = NULL) const;

private:
	I_REF(istd::IChangeable, m_objectCompPtr);
	I_REF(iser::IFileLoader, m_inputLoaderCompPtr);
	I_REF(iser::IFileLoader, m_outputLoaderCompPtr);
};


} // namespace ibase


#endif // !ibase_CFileCopyOverLoaderComp_included


