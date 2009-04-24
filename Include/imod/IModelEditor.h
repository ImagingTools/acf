#ifndef imod_IModelEditor_included
#define imod_IModelEditor_included


#include "istd/IPolymorphic.h"

#include "imod/imod.h"


namespace imod
{

/**
	\ingroup imod

	Common interface for an model editor.
*/
class IModelEditor: virtual public istd::IPolymorphic
{
public:
	/**
		Updates editor with model data.
	*/
	virtual void UpdateEditor(int updateFlags = 0) = 0;

	/**
		Updates model from editor.
	*/
	virtual void UpdateModel() const = 0;

	/**
		Returns \c true if the model data can be changed.
	*/
	virtual bool IsReadOnly() const = 0;

	/**
		Set flag that the model data can be changed.
		\return	true, if this feature is supported.
	*/
	virtual void SetReadOnly(bool state) = 0;
};


} // namespace imod


#endif // !imod_IModelEditor_included

