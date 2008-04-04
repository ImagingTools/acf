#ifndef istd_included
#define istd_included


#include <limits>


/**
	\defgroup istd Standard
	\ingroup ACF

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
	InfoLevel,
	WarningLevel,
	ErrorLevel,
	CriticalLevel
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
typedef unsigned long long I_QWORD;
typedef signed long long I_SQWORD;

static const double I_BIG_EPSILON = 1.0e-8;

/**
	Some very small number.
*/
#define I_EPSILON std::numeric_limits<double>::epsilon()


/**
	Infinite value.
*/
#define I_INFINITY std::numeric_limits<double>::infinity()


/**
	Automatic generation of variable names. AUTO_VARNAME2 and AUTO_VARNAME3 are helpers and are not for directly use!
*/
#define AUTO_VARNAME(var) AUTO_VARNAME2(var, __LINE__)
#define AUTO_VARNAME2(var, LINE_NUMBER) AUTO_VARNAME3(var, LINE_NUMBER)
#define AUTO_VARNAME3(var, LINE_NUMBER) var##LINE_NUMBER


#ifdef _DEBUG


#define I_IF_DEBUG(instructions) instructions
#define I_ASSERT(condition) if (!(condition)){::istd::OnSoftwareError(__FILE__, __LINE__);}
#define I_CRITICAL() istd::OnSoftwareError(__FILE__, __LINE__)


#else //_DEBUG


#define I_ASSERT(test)
#define I_CRITICAL()
#define I_IF_DEBUG(instructions)


#endif //_DEBUG


#if defined(_DEBUG) || defined(I_FORCE_TRACE)


#define I_TRACE(level, groupId, message) if (CheckTraceEnabled(level, groupId)){static CGroupRegistrator registrator(groupId); SendTraceMessage(level, groupId, message, __FILE__, __LINE__);}
#define I_TRACE_INFO(groupId, message) I_TRACE(istd::InfoLevel, groupId, message)
#define I_TRACE_WARNING(groupId, message) I_TRACE(istd::WarningLevel, groupId, message)
#define I_TRACE_ERROR(groupId, message) I_TRACE(istd::ErrorLevel, groupId, message)
#define I_TRACE_CRITICAL(groupId, message) I_TRACE(istd::CriticalLevel, groupId, message)


#else //defined(_DEBUG) || defined(I_FORCE_TRACE)


#define I_TRACE_INFO(groupId, message)
#define I_TRACE_WARNING(groupId, message)
#define I_TRACE_ERROR(groupId, message)
#define I_TRACE_CRITICAL(groupId, message)


#endif //defined(_DEBUG) || defined(I_FORCE_TRACE)


#endif // !istd_included

