#ifndef istd_h_included
#define istd_h_included


/**
\defgroup istd
\ingroup ACF



/**
	Contains the standard, system indenendent primitives and basic implementations.
*/
namespace istd
{


class CGroupRegistrator
{
public:
	CGroupRegistrator(const char* fileName);
};


enum TraceLevel
{
	TL_INFO,
	TL_WARNING,
	TL_ERROR,
	TL_CRITICAL
};


extern void OnSoftwareError(const char* fileName, int line);
extern bool CheckTraceEnabled(TraceLevel level, const char* groupId);
extern void SendTraceMessage(TraceLevel level, const char* groupId, const char* message, const char* fileName, int line);


} // namespace istd


typedef unsigned char I_BYTE;
typedef signed char I_SBYTE;
typedef unsigned short I_WORD;
typedef signed short I_SWORD;
typedef unsigned int I_DWORD;
typedef signed int I_SDWORD;
typedef long long I_QWORD;


#ifdef _DEBUG


#define I_IF_DEBUG(instructions) instructions
#define I_ASSERT(condition) if (!(condition)){::istd::OnSoftwareError(__FILE__, __LINE__);}
#define I_ERROR() istd::OnSoftwareError(__FILE__, __LINE__)


#else //_DEBUG


#define I_ASSERT(test)
#define I_ERROR()
#define I_IF_DEBUG(instructions)


#endif //_DEBUG


#if defined(_DEBUG) || defined(I_FORCE_TRACE)


#define I_TRACE(level, groupId, message) if (CheckTraceEnabled(level, groupId)){static CGroupRegistrator registrator(groupId); SendTraceMessage(level, groupId, message, __FILE__, __LINE__);}
#define I_TRACE_INFO(groupId, message) I_TRACE(istd::TL_INFO, groupId, message)
#define I_TRACE_WARNING(groupId, message) I_TRACE(istd::TL_WARNING, groupId, message)
#define I_TRACE_ERROR(groupId, message) I_TRACE(istd::TL_ERROR, groupId, message)
#define I_TRACE_CRITICAL(groupId, message) I_TRACE(istd::TL_CRITICAL, groupId, message)


#else //defined(_DEBUG) || defined(I_FORCE_TRACE)


#define I_TRACE_INFO(groupId, message)
#define I_TRACE_WARNING(groupId, message)
#define I_TRACE_ERROR(groupId, message)
#define I_TRACE_CRITICAL(groupId, message)


#endif //defined(_DEBUG) || defined(I_FORCE_TRACE)


#endif // !istd_h_included

