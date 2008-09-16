#ifndef iser_CArchiveBase_included
#define iser_CArchiveBase_included


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iser
{


/**
	Base implementation of iser::IArchive interface.
	It provides standard implementation of some methods and standard tags.
*/
class CArchiveBase: virtual public IArchive
{
public:
	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& subTag, int& count);

protected:
	CArchiveBase(){}

	// list of standard tags used to serialize header
	static iser::CArchiveTag s_acfRootTag;
	static iser::CArchiveTag s_headerTag;
	static iser::CArchiveTag s_versionInfosTag;
	static iser::CArchiveTag s_versionInfoTag;
	static iser::CArchiveTag s_versionIdTag;
	static iser::CArchiveTag s_versionNumberTag;
	static iser::CArchiveTag s_versionDescriptionTag;

private:
	/**
		Block of copy contructor for all derrived classes.
	*/
	CArchiveBase(const CArchiveBase&){}
};


} // namespace iser


#endif // !iser_CArchiveBase_included


