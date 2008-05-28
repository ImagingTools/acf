#ifndef iqt_CBitmapLoaderComp_included
#define iqt_CBitmapLoaderComp_included


// Qt includes
#include <QDir>
#include <QStringList>

#include "iser/IFileLoader.h"

#include "iproc/TSyncProcessorWrap.h"

#include "icam/IBitmapAcquisition.h"
#include "icam/IFileBitmapAcquisitionParams.h"

#include "icomp/CComponentBase.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Bitmap loader component implementing interfaces \c iser::IFileLoader and \c icam::IBitmapAcquisition.
*/
class CBitmapLoaderComp: public icomp::CComponentBase,
			virtual public iser::IFileLoader,
			virtual public iproc::TSyncProcessorWrap<icam::IBitmapAcquisition, icam::IBitmapAcquisition::InputType, icam::IBitmapAcquisition::OutputType>
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CBitmapLoaderComp)
		I_REGISTER_INTERFACE(iser::IFileLoader)
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition)
		I_ASSIGN(m_defaultDirAttrPtr, "DefaultDir", "Directory will be used if no parameters are specified", true, ".")
		I_ASSIGN(m_parameterIdAttrPtr, "ParameterId", "Id used to get parameters from the parameter set", true, "FileBitmapAcquisition")
		I_ASSIGN(m_maxCachedDirectoriesAttrPtr, "MaxCachedDirs", "Maximum number of cached directories", true, 10)
	I_END_COMPONENT

	CBitmapLoaderComp();

	virtual void SetLastLoadFileName(const istd::CString& fileName);
	virtual void SetLastSaveFileName(const istd::CString& fileName);

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				bool forLoading = true,
				bool forSaving = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const;
	virtual const istd::CString& GetLastLoadFileName() const;
	virtual const istd::CString& GetLastSaveFileName() const;

	// reimplemented (iproc::TSyncProcessorWrap<isys::ITimer, iimg::IBitmap>)
	virtual int DoSyncProcess(const iprm::IParamsSet* paramsPtr, const isys::ITimer* inputPtr, iimg::IBitmap* outputPtr);

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

protected:
	struct ParamsInfo
	{
		ParamsInfo();

		QStringList files;
		QStringList::Iterator filesIter;
		I_DWORD idStamp;
	};

private:
	typedef ::std::map<istd::CString, ParamsInfo> DirInfos;
	DirInfos m_dirInfos;

	I_DWORD m_lastIdStamp;

	istd::CString m_lastLoadFileName;
	istd::CString m_lastSaveFileName;

	I_ATTR(istd::CString, m_defaultDirAttrPtr);
	I_ATTR(istd::CString, m_parameterIdAttrPtr);
	I_ATTR(int, m_maxCachedDirectoriesAttrPtr);
};


// inline methods

inline void CBitmapLoaderComp::SetLastLoadFileName(const istd::CString& fileName)
{
	m_lastLoadFileName = fileName;
}


inline void CBitmapLoaderComp::SetLastSaveFileName(const istd::CString& fileName)
{
	m_lastSaveFileName = fileName;
}


} // namespace iqt


#endif // !iqt_CBitmapLoaderComp_included


