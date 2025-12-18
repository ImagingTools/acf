#ifndef iprm_CSelectionParam_included
#define iprm_CSelectionParam_included


// Qt includes
#include <QtCore/QMap>

// ACF includes 
#include <imod/TSingleModelObserverBase.h>
#include <iser/ISerializable.h>
#include <iprm/ISelectionParam.h>
#include <iprm/IOptionsList.h>


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
	virtual const IOptionsList* GetSelectionConstraints() const override;
	virtual int GetSelectedOptionIndex() const override;
	virtual bool SetSelectedOptionIndex(int index) override;
	virtual ISelectionParam* GetSubselection(int index) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

protected:
	/**
		Calculate optimal index based on current selected index and ID.
	*/
	int CalcIndexFromId(const QByteArray& optionId, int suggestedIndex = NO_SELECTION) const;
	/**
		Synchronize current index to optimal one.
	*/
	bool SyncIndexWithId();

	/**
		\internal
		Observes changes of contraints to reflect is in object state.
	*/
	class ConstraintsObserver: public imod::TSingleModelObserverBase<iprm::IOptionsList>
	{
	public:
		explicit ConstraintsObserver(CSelectionParam& parent);

		// reimplemented (imod::TSingleModelObserverBase<iprm::IOptionsList>)
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override;

	private:
		CSelectionParam& m_parent;
	};

private:
	int m_selectedOptionIndex;
	QByteArray m_selectedOptionId;

	typedef QMap<int, iprm::ISelectionParam*> SubselectionMap;
	SubselectionMap m_subselectionMap;

	const IOptionsList* m_constraintsPtr;
	ConstraintsObserver m_constraintsObserver;
};


} // namespace iprm


#endif // !iprm_CSelectionParam_included


