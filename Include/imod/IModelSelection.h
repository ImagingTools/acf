#ifndef imod_IModelSelection_included
#define imod_IModelSelection_included


#include "istd/IChangeable.h"

#include "imod/IModel.h"


namespace imod
{


/**
	Interface for model selection.	

	\ingroup ModelObserver
*/
class IModelSelection: virtual public istd::IChangeable
{
public:
	/**
		Get selected model.
	*/
	virtual imod::IModel* GetSelectedModel() const = 0;

	/**
		Set selected model.
	*/
	virtual void SetSelectedModel(imod::IModel* modelPtr) = 0;
};


} // namespace imod


#endif // !imod_IModelSelection_included


