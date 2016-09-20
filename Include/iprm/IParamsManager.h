#ifndef iprm_IParamsManager_included
#define iprm_IParamsManager_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QSet>
#include <QtCore/QByteArray>

// ACF includes
#include <iprm/ISelectionParam.h>


namespace iprm
{


class IParamsSet;


/**
	Manager of parameters set.
*/
class IParamsManager: virtual public iprm::ISelectionParam
{
public:
	typedef QSet<QByteArray> TypeIds;
	/**
		Bitwise coded flags used to find out supported features.
	*/
	enum ParamsOperationFlags
	{
		/**
			Active if number of parameters is fixed.
		*/
		MF_COUNT_FIXED = 1,

		/**
			Active if rename of parameters is supported.
		*/
		MF_SUPPORT_RENAME = 2,

		/**
			Active if insert of parameters is possible.
		*/
		MF_SUPPORT_INSERT = 4,

		/**
			Active if delete of parameters is possible.
		*/
		MF_SUPPORT_DELETE = 8,

		/**
			Active if swap of parameters with the other one is possible.
		*/
		MF_SUPPORT_SWAP = 16,

		/**
			Active if it is possible to get the disabled option.
		*/
		MF_DISABLE_ALLOWED = 32,

		/**
			Active if it is possible to edit parameter values.
		*/
		MF_SUPPORT_EDIT = 64,

		/**
			Option is inactive (i.e. cannot be activated or selected) 
		*/
		MF_INACTIVE = 128
	};

	/**
		Possible changes of the manager's data model.
	*/
	enum ChangeFlags
	{
		CF_SET_INSERTED = 0x5af60,
		CF_SET_REMOVED,
		CF_SET_NAME_CHANGED,
		CF_SET_ENABLE_CHANGED
	};

	/**
		Get operation control flags of some paramter set or whole manager.
		\sa ManagerFlags.
		\param	index	index addressing position in parameter set list.
						For negative value general flags are returned.
						For example if you want to check if there is any renameable element
						you should call it with negative index value.
						To check if first element can be renamed call it with index value 0.
						To check if you can insert at the last position please use index value equal to current number of parameter set.
	*/
	virtual int GetIndexOperationFlags(int index = -1) const = 0;

	/**
		Get number of managed set.
	*/
	virtual int GetParamsSetsCount() const = 0;

	/**
		Get selection contraints describing each supported type.
		\return	NULL, if only one annonymed type is supported.
	*/
	virtual const IOptionsList* GetParamsTypeConstraints() const = 0;

	/**
		Insert new parameters set at selected position.
		\param	typeIndex	Index of parameter type.
							This index is context specified and is used to indicate type of parameters should be added.
							If equals negative value the type will be automatically selected.
		\param	index		Position in list of parameters or negative value to use default position.
		\return				Position of inserted new entry or negative value if no insert was possible.
							For specified index positions (positive \c index value) it will return the same index or negative value.
	*/
	virtual int InsertParamsSet(int typeIndex = -1, int index = -1) = 0;

	/**
		Remove parameters set at selected position.
	*/
	virtual bool RemoveParamsSet(int index) = 0;

	/**
		Swap two parameter sets.
	*/
	virtual bool SwapParamsSet(int index1, int index2) = 0;

	/**
		Get selected parameter set.
	*/
	virtual IParamsSet* GetParamsSet(int index) const = 0;

	/**
		\param	typeIndex	Index of parameter type.
							This index is context specified and is used to indicate type of parameters should be created.
							If equals negative value, the type will be automatically selected.
		\param	index		If \c index is not negative, the parameter set data at the given position will be copied to the created instance.
		\note The caller is responsible for the memory mangement of the created parameter set.
	*/
	virtual IParamsSet* CreateParameterSet(int typeIndex = -1, int index = -1) const = 0;

	/**
		Get name of specified parameter set.
	*/
	virtual QString GetParamsSetName(int index) const = 0;

	/**
		Set name of specified parameter set.
	*/
	virtual bool SetParamsSetName(int index, const QString& name) = 0;

	/**
		Get the description of the specified parameter set.
	*/
	virtual QString GetParamsSetDescription(int index) const = 0;

	/**
		Set the description of the specified parameter set.
	*/
	virtual void SetParamsSetDescription(int index, const QString& description) = 0;
};


} // namespace iprm


#endif // !iprm_IParamsManager_included


