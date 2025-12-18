#pragma once


// ACF includes
#include <istd/IChangeable.h>


namespace istd
{


template <class ModelImpl>
class TCloneableWrap: public ModelImpl
{
public:
	// pseudo-reimplemented (istd::IChangeable)
	virtual istd::IChangeableUniquePtr CloneMe(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const override;
};


// pseudo-reimplemented (istd::IChangeable)

template <class ModelImpl>
istd::IChangeableUniquePtr TCloneableWrap<ModelImpl>::CloneMe(istd::IChangeable::CompatibilityMode mode) const
{
	istd::IChangeableUniquePtr clonePtr(new ModelImpl);
	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr;
	}

	return istd::IChangeableUniquePtr();
}


} // namespace istd


