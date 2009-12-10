#ifndef ACF_included
#define ACF_included


/**
\defgroup ACF ACF

\mainpage
\section Introduction
ACF (Application Component Framework) is a powerfull C++ based component framework that was especially designed 
for creation of very complex applications. ACF contains many standard implementations of most usefull design patterns, 
such Serialization, Model/Observer, Factory and so on. ACF implements a powerfull component concept 
and includes many well designed components, which you can directly use in your application.

\section Example

\section Tutorials
See \c Docs/Tutorial for tutorials.
*/


/**
	\defgroup ModelObserver Model/Observer concept
	The main use-case of this concept is to get information on the observer side about changes of model.
	It can be used e.g. to realize GUI update if releted model changes.

	\ingroup ACF
*/


/**
	\defgroup Persistence Persistence concept.

	\ingroup ACF
*/


/**
	\defgroup Main Main concepts and implementations
	If you can learn how to use ACF you should begin with elements defined in this module.

	\ingroup ACF
*/


/**
	\defgroup System Defines access to operating system dependent implementation
	Many of system dependent interfaces will be registered on global level as services.
	This interfaces can be accessed using global template functions istd::GetService() and istd::CreateService().

	\ingroup ACF
*/

/**
	\defgroup ComponentConcept Component concept
	This concept allows to divide application into smaller modules.
	At the implementation level you don't need to care on the application topology.
	The topology will be described separatelly using \em .arx files and can be started using
	runtime environment over ACF tool or compiled to C++ code using ARXC compiler.
	Additionally you can use component simulation to use components as "normal" classes.

	\ingroup ACF
*/


#endif // !ACF_included
