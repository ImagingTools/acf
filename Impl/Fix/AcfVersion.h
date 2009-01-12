#ifndef isvn_AcfVersion_included
#define isvn_AcfVersion_included


namespace isvn
{


enum RepositoryState{
	RS_ORIGINAL_VERSION =  752,
	RS_DIRTY_FLAG = 0,
	RS_USE_VERSION = RS_ORIGINAL_VERSION + RS_DIRTY_FLAG
};


} // namespace isvn


#endif // !isvn_AcfVersion_included


