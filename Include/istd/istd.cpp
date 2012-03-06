#include "istd/istd.h"


// STL includes
#include <assert.h>
#include <iostream>

// ACF includes
#include "istd/ITrace.h"
#include "istd/CStaticServicesProvider.h"


namespace istd
{


// public methods of class CGroupRegistrator

CGroupRegistrator::CGroupRegistrator(const char* groupId)
{
	ITrace* tracePtr = GetService<ITrace>();

	if (tracePtr != NULL){
		tracePtr->RegisterGroupId(groupId);
	}
}


// public functions

void OnSoftwareError(const char* fileName, int line)
{
	ITrace* tracePtr = GetService<ITrace>();

	if (tracePtr != NULL){
		static const char* groupId = "assert";

		if (tracePtr->IsTraceEnabled(CriticalLevel, groupId)){
			tracePtr->LogMessage(CriticalLevel, groupId, "assertion error", fileName, line);
		}
	}

	assert(false);
}



bool CheckTraceEnabled(TraceLevel level, const char* groupId)
{
	ITrace* tracePtr = GetService<ITrace>();

	if (tracePtr != NULL){
		return tracePtr->IsTraceEnabled(level, groupId);
	}
	else{
		return false;
	}
}


void SendTraceMessage(TraceLevel level, const char* groupId, const char* message, const char* fileName, int line)
{
	ITrace* tracePtr = GetService<ITrace>();

	if (tracePtr != NULL){
		tracePtr->LogMessage(level, groupId, message, fileName, line);
	}
	else{
		std::cerr << level << "\t" << groupId << "\t" << message << " [fileName: " << line << "]" << std::endl;
	}
}


} // namespace istd


