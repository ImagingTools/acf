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
	enum ChangeFlags
	{
		/**
			Internal ACF flag.
			Indicate that internal update is done.
		*/
		CF_ACF_INTERNAL = 1 << 0,
		/**
			Indicate that data model is changed.
		*/
		CF_MODEL = 1 << 1,
		/**
			Indicate, that update was aborted by user.
		*/
		CF_ABORTED = 1 << 2,
		/**
			Change flag indicate that this change was delegated (is indirect) for example from agregated object.
		*/
		CF_DELEGATED = 1 << 3,
		/**
			Used to avoid adding of changes to undo list.
		*/
		CF_NO_UNDO = 1 << 5
	};

	/**
		Flags for supported operations.
		This flags can be combined.
	*/
	enum SupportedOperations
	{
		/**
			No operations are supported.
		*/
		SO_NONE = 0,

		/**
			Observing of this object via observer-observable pattern.
		*/
		SO_OBSERVE = 1,
		
		/**
			Copying from other object.
		*/
		SO_COPY = 2,
		
		/**
			Creating of copy of this object.
		*/
		SO_CLONE = 4,

		/**
			Comparsion with another object.
		*/
		SO_COMPARE = 8
	};

	/**
		Control how relationship betweeen objects are interpreted.
		It is also used to define how objects are copied.
	*/
	enum CompatibilityMode
	{
		/**
			If external references are different this object are not compatible and cannot be compared or copied.
			Copy without references to external objects only if objects are compatible.
		*/
		CM_STRICT,
		/**
			External references are simple ignored.
			Copy without references to external objects.
		*/
		CM_WITHOUT_REFS,
		/**
			External references are part of object integrity.
			For comparation external references must be equal, for copy operations they will be copied.
		*/
		CM_WITH_REFS,
		/**
			Data of object are interpreted in context of external references.
			For comparation the meaning of object (e.g. after transformation using externals) will be compared.
			Copying will converting the data to be compatible according to existing references to external objects.
		*/
		CM_CONVERT
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

	/**
		Get set of flags for supported operations.
		\sa SupportedOperations
	*/
	virtual int GetSupportedOperations() const;

	/**
		Copy this object from another one.
		Default implementation in istd::IChangeable do nothing.
	*/
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

	/**
		Compare this object with another object.
		\param object	Object to be compared
		\return \c true if the objects are identical and \c false otherwise.
	*/
	virtual bool IsEqual(const IChangeable& object) const;

	/**
		Make a copy of this object.
		\return	new instance or NULL, if this operation is not supported.
	*/
	virtual IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

protected:
	/**
		Callback function for begin change event.
		Please note, that in some cases no following OnEndChanges can be called.
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
	if ((changeFlags & CF_ABORTED) == 0){
		OnEndChanges(changeFlags, changeParamsPtr);
	}
}


inline int IChangeable::GetSupportedOperations() const
{
	return SO_NONE;
}


inline bool IChangeable::CopyFrom(const IChangeable& /*object*/, CompatibilityMode /*mode*/)
{
	return false;
}


inline bool IChangeable::IsEqual(const IChangeable& /*object*/) const
{
	return false;
}


inline IChangeable* IChangeable::CloneMe(CompatibilityMode /*mode*/) const
{
	return NULL;
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

