#ifndef iprm_CSelectionParam_included
#define iprm_CSelectionParam_included


// Qt includes
#include <QtCore/QMap>

// ACF includes 
#include "iser/ISerializable.h"

#include "iprm/ISelectionParam.h"


namespace iprm
{


/**
	Basic implementation of selection parameter.
*/
class CSelectionParam: virtual public ISelectionParam
{
public:
	CSelectionParam();

	/**
		Set selection constraints for this selection object. 
	*/
	void SetSelectionConstraints(const IOptionsList* constraintsPtr);

	/**
		Set sub-selection for a given option index. 
	*/
	void SetSubselection(int selectionIndex, ISelectionParam* selectionPtr);

	/**
		Set selection index according to a given option ID.
	*/
	bool SetSelectedOptionById(const QByteArray& selectedOptionId);

	/**
		Get sub-selection for the currently selected option.
	*/
	ISelectionParam* GetActiveSubselection() const;

	// reimplemented (iprm::ISelectionParam)
	virtual const IOptionsList* GetSelectionConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual ISelectionParam* GetSubselection(int index) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	int m_selectedOptionIndex;

private:
	typedef QMap<int, iprm::ISelectionParam*> SubselectionMap;

	const IOptionsList* m_constraintsPtr;

	SubselectionMap m_subselectionMap;
};


} // namespace iprm


#endif // !iprm_CSelectionParam_included


