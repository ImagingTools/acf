#ifndef istd_IChangeable_included
#define istd_IChangeable_included


#include "istd/istd.h"


#include "istd/IPolymorphic.h"


namespace istd
{


/**
	Common interface for model objects, which can be changed.
	Model changes are executed in two steps: begin and end.
	The caller can also put some flags and additional data to the update functions, to control the update logic.
*/
class IChangeable: virtual public IPolymorphic  
{
public:
	enum ChangeFlags{
		/**
			Internal ACF flag.
			Indicate that internal update is done.
		*/
		CF_ACF_INTERNAL = 0x1,
		/**
			Indicate that data model is changed.
		*/
		CF_MODEL = 0x2
	};
	
	/**
		Starts the change transaction. 
		\note Please note that the \c changeFlags and \c changeParamsPtr must not be identical with the
		corresponded parameters in EndChanges().
		\sa EndChanges()
	*/
	virtual void BeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr = NULL);

	/**
		Ends the change transaction. 
		\note Please note that the \c changeFlags and \c changeParamsPtr must not be identical with the
		corresponded parameters in BeginChanges().
		\sa BeginChanges()
	*/
	virtual void EndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr = NULL);

protected:
	/**
		Callback function for begin change event.
	*/
	virtual void OnBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

	/**
		Callback function for end change event.
	*/
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);
};


// public methods

inline void IChangeable::BeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	OnBeginChanges(changeFlags, changeParamsPtr);
}


inline void IChangeable::EndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	OnEndChanges(changeFlags, changeParamsPtr);
}


// protected methods

inline void IChangeable::OnBeginChanges(int /*changeFlags*/, istd::IPolymorphic* /*changeParamsPtr*/)
{
}


inline void IChangeable::OnEndChanges(int /*changeFlags*/, istd::IPolymorphic* /*changeParamsPtr*/)
{
}


} // namespace istd


#endif // !istd_IChangeable_included

