#ifndef iproc_included
#define iproc_included


#include "ibase/ibase.h"


/**
	This namespace containes interfaces and implementation of data processing concepts.
	There are 2 main processing concepts:
	\li	asynchrone processing with <em>"push data"</em> model, where the caller controls life cycle of processed objects. It is represented by class iproc::IProcessor,
	\li	asynchrone processing with <em>"pull data" model</em>, where processing objects are global, undepend on processing session. It is represented by class iproc::ISupplier.

	This package is system independent.
	This package use following ACF packages:
	\li	istd
	\li	iser
	\li	ibase
	\li	iprm
*/
namespace iproc
{


} // namespace iproc


#endif // !iproc_included

