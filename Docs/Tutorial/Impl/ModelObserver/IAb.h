#ifndef IAb_included
#define IAb_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <iser/ISerializable.h>


class IAConstraints;


/**
	Interface for AB-object
*/
class IAb: virtual public iser::ISerializable
{
public:
	/**
		Flags for changes indentification
	*/
	enum ChangeFlags
	{
		/**
			State of A was changed
		*/
		CF_A_CHANGED = 0xba3530,	// some unique random value

		/**
			State of B was changed
		*/
		CF_B_CHANGED
	};

	/**
		Get constraints of A.
		Constraints describes some additional information about allowed parameter ranges and related informations.
	*/
	virtual const IAConstraints& GetAConstraints() const = 0;

	/**
		Get the values of A.
	*/
	virtual int GetA() const = 0;

	/**
		Set the value of A
	*/
	virtual bool SetA(int a) = 0;

	/**
		Get the value of B
	*/
	virtual QByteArray GetB() const = 0;

	/**
		Set the value of B
	*/
	virtual void SetB(const QByteArray& b) = 0;
};


#endif // !IAb_included


