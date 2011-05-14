#ifndef imod_IModelEditor_included
#define imod_IModelEditor_included


#include "istd/IPolymorphic.h"

#include "imod/imod.h"


namespace imod
{

/**
	Common interface for an model editor.

	\ingroup ModelObserver
*/
class IModelEditor: virtual public istd::IPolymorphic
{
public:
	enum ChangeFlags
	{
		/**
			Called at begin of editor initialization.
		*/
		CF_INIT_EDITOR = 1 << 4
	};

	/**
		Updates editor with model data.
		This method will be called only if GUI is connected.
		\param	updateFlags	the same as \c changeFlag in istd::IChangeable.
	*/
	virtual void UpdateEditor(int updateFlags = 0) = 0;

	/**
		Updates model from editor.
		This method will be called only if GUI is connected and model is attached.
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

