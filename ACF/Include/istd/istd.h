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


// define own min function to avoid conflicts between STL and M$ macros
template <typename T>
inline typename const T& Min(const T& value1, const T& value2)
{
	if (value1 < value2){
		return value1;
	}
	else{
		return value2;
	}
}


// define own max function to avoid conflicts between STL and M$ macros
template <typename T>
inline typename const T& Max(const T& value1, const T& value2)
{
	if (value1 > value2){
		return value1;
	}
	else{
		return value2;
	}
}


class CGroupRegistrator
{
public:
	CGroupRegistrator(const char* filePath);
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


/**
	Send debug information to predefined output.
	\param	level	one of:
		\li	istd::InfoLevel	minimal message priority, info only.
		\li	istd::WarningLevel	warning message priority, should be used for normally but untipical state.
		\li	istd::ErrorLevel	error message priority, should not happened but e.g. wrong using of parameters can cause it.
		\li	istd::CriticalLevel	maximal message priority, for application states which should never occured.
*/
#define I_TRACE(level, groupId, message) if (istd::CheckTraceEnabled(level, groupId)){static istd::CGroupRegistrator registrator(groupId); istd::SendTraceMessage(level, groupId, message, __FILE__, __LINE__);}
#define I_TRACE_ONCE(level, groupId, message){\
	static active = true;\
	if (active){\
		I_TRACE(level, groupId, message);\
		active = false;\
	}\
}


#else //defined(_DEBUG) || defined(I_FORCE_TRACE)


#define I_TRACE(level, groupId, message)
#define I_TRACE_ONCE(level, groupId, message)


#endif //defined(_DEBUG) || defined(I_FORCE_TRACE)


#endif // !istd_included

