#pragma once


// ACF includes
#include <istd/IChangeable.h>
#include <icmm/icmm.h>


namespace icmm
{


/**
	Common interface for a colorant list used in a substractive color model.
*/
class IColorantList : virtual public istd::IChangeable
{
public:
	/**
		Get the list of colorant-IDs.
	*/
	virtual QByteArrayList GetColorantIds() const = 0;

	/**
		Get display name of the colorant with the given ID.
	*/
	virtual QString GetColorantName(const QByteArray& colorantId) const = 0;

	/**
		Get type of the colorant with the given ID.
	*/
	virtual ColorantType GetColorantType(const QByteArray& colorantId) const = 0;

	/**
		Get type of the colorant with the given ID. 
	*/
	virtual ProcessColorantUsage GetProcessColorantUsage(const QByteArray& colorantId) const = 0;
};


} // namespace icmm


