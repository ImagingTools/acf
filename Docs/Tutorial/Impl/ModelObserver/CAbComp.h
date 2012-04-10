#ifndef CAbComp_included
#define CAbComp_included

// ACF includes
#include "iser/ISerializable.h"

#include "icomp/CComponentBase.h"


// AB includes
#include "IAb.h"
#include "IAConstraints.h"


/**
	Implementation of AB-component.
*/
class CAbComp:
	public icomp::CComponentBase,	// Basic class for all ACF-components
	virtual public IAb,				// Interface to be implemented
	virtual public IAConstraints	// Contraints interface can be also directly implemented hier, but it is only an option.
{
public:
	typedef icomp::CComponentBase BaseClass; // CComponentBase or any from CComponentBase direct or indirect derived class must be declared as BaseClass. It is necessary for I_BEGIN_COMPONENT macro.

	// Component initialization block:
	I_BEGIN_COMPONENT(CAbComp); 
		// Register interfaces (It makes these interface accessable for other components):
		I_REGISTER_INTERFACE(IAb);
		I_REGISTER_INTERFACE(IAConstraints);
		I_REGISTER_INTERFACE(iser::ISerializable);

		// Assign meta-informations for declared attributes: (ID of the attribute, short description, obligatory/optional flag and default value)
		I_ASSIGN(m_defaultAValueAttrPtr, "DefaultAValue", "Default value of A", false, 50);
		I_ASSIGN(m_defaultBValueAttrPtr, "DefaultBValue", "Default value of B", false, "SomeText");
	I_END_COMPONENT;

	CAbComp();

	// reimplemented (IAb)
	virtual const IAConstraints& GetAConstraints() const;
	virtual int GetA() const;
	virtual bool SetA(int a);
	virtual QByteArray GetB() const;
	virtual void SetB(const QByteArray& b);

	// reimplemented (IAConstraints)
	virtual const istd::CRange& GetARange() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:	
	// Data model definition

	/**
		A
	*/
	int m_a;

	/**
		B
	*/
	QByteArray m_b;

	/**
		Constraints of A
	*/
	istd::CRange m_aRange;

	/**
		Attribute for default value of A.
	*/
	I_ATTR(int, m_defaultAValueAttrPtr);

	/**
		Attribute for default value of B.
	*/
	I_ATTR(QByteArray, m_defaultBValueAttrPtr);
};


#endif // !CAbComp_included

