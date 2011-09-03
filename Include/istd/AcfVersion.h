#ifndef istd_AcfVersion_included
#define istd_AcfVersion_included


namespace istd
{


enum RepositoryState
{
	RS_ORIGINAL_VERSION =  1871,
	RS_DIRTY_FLAG = 1,
	RS_USE_VERSION = RS_ORIGINAL_VERSION + RS_DIRTY_FLAG
};


} // namespace istd


#endif // !istd_AcfVersion_included


