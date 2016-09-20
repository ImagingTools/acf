#ifndef ifile_CTimeStampFileNameParamComp_included
#define ifile_CTimeStampFileNameParamComp_included


// ACF includes
#include <ifile/CRelativeFileNameParamComp.h>


namespace ifile
{


/**
	File path generation using time stamp.
*/
class CTimeStampFileNameParamComp: public ifile::CRelativeFileNameParamComp
{
public:
	typedef ifile::CRelativeFileNameParamComp BaseClass;

	I_BEGIN_COMPONENT(CTimeStampFileNameParamComp);
	I_END_COMPONENT;

	// reimplemented (ifile::IFileNameParam)
	virtual const QString& GetPath() const;

private:
	mutable QString m_generatedPath;
};


} // namespace ifile


#endif // !ifile_CTimeStampFileNameParamComp_included


