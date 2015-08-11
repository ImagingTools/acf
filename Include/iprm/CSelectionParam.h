#ifndef iprm_CSelectionParam_included
#define iprm_CSelectionParam_included


// Qt includes
#include <QtCore/QMap>

// ACF includes 
#include "imod/TSingleModelObserverBase.h"
#include "iser/ISerializable.h"
#include "iprm/ISelectionParam.h"
#include "iprm/IOptionsList.h"


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
		Get index of an option by ID.
	*/
	int GetOptionIndexById(const QByteArray& optionId) const;

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
	/**
		\internal
		Observes changes of contraints to reflect is in object state.
	*/
	class ConstraintsObserver: public imod::TSingleModelObserverBase<iprm::IOptionsList>
	{
	public:
		ConstraintsObserver(CSelectionParam& parent);

		// reimplemented (imod::TSingleModelObserverBase<iprm::IOptionsList>)
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	private:
		CSelectionParam& m_parent;
	};

private:
	typedef QMap<int, iprm::ISelectionParam*> SubselectionMap;

	int m_selectedOptionIndex;

	const IOptionsList* m_constraintsPtr;

	SubselectionMap m_subselectionMap;

	QByteArray m_selectedOptionId;

	ConstraintsObserver m_constraintsObserver;
};


} // namespace iprm


#endif // !iprm_CSelectionParam_included


