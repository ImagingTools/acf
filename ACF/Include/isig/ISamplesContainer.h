#ifndef isig_ISamplesContainer_included
#define isig_ISamplesContainer_included


#include "iser/ISerializable.h"

#include "isig/isig.h"


namespace isig
{


class ISamplesContainer: virtual public iser::ISerializable
{
public:
	/**
		Set number of stored samples to 0.
	*/
	virtual void ResetContainer() = 0;

	/**
		Get number of samples stored in this container.
	*/
	virtual int GetSamplesCount() const = 0;

	/**
		Set number of samples stored in this container.
		\return	true, if sucess - correct number of elements was allocated.
	*/
	virtual bool SetSamplesCount(int count) = 0;

	/**
		Get sample value at specified index.
	*/
	virtual double GetSample(int index) const = 0;

	/**
		Set sample value at specified index.
	*/
	virtual void SetSample(int index, double value) = 0;
};


} // namespace isig


#endif // !isig_ISamplesContainer_included


