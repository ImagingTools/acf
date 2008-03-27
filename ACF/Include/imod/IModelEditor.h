#ifndef imod_IModelEditor_included
#define imod_IModelEditor_included


#include "imod/imod.h"


#include "imod/IObserver.h"


namespace imod
{

/**
	\ingroup imod

	Common interface for an model editor.
*/
class IModelEditor: virtual public imod::IObserver
{
public:
	/**
		Updates editor with model data.
	*/
	virtual void UpdateEditor() = 0;

	/**
		Updates model from editor.
	*/
	virtual void UpdateModel() = 0;

	/**
		Resets the editor view.
	*/
	virtual void ResetEditor() = 0;

	/**
		Returns \c true if the model data can be changed.
	*/
	virtual bool IsModelChangeable() const = 0;
};


} // namespace imod


#endif // !imod_IModelEditor_included

