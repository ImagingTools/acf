#pragma once


// ACF includes
#include <istd/IChangeable.h>


namespace ilog
{


class ITracingConfiguration: virtual public istd::IChangeable
{
public:
	// -1 tracing off, 0 tracing all
	virtual int GetTracingLevel() const = 0;
	virtual void SetTracingLevel(int tracingLevel) = 0;
};


} // namespace ilog

