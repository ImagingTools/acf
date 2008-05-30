	#ifndef iqt_CParamsManagerGuiComp_included
	#define iqt_CParamsManagerGuiComp_included


	#include "iprm/IParamsManager.h"
	#include "iprm/ISelectionParam.h"

	#include "iqt/TDesignerGuiObserverCompBase.h"

	#include "iqt/Generated/ui_CParamsManagerGuiComp.h"


	namespace iqt
	{


	class CParamsManagerGuiComp: public TDesignerGuiObserverCompBase<
				Ui::CParamsManagerGuiComp,
				iprm::IParamsManager>
	{
		Q_OBJECT

	public:
		typedef TDesignerGuiObserverCompBase<
					Ui::CParamsManagerGuiComp,
					iprm::IParamsManager> BaseClass;

		I_BEGIN_COMPONENT(CParamsManagerGuiComp)
			I_ASSIGN(m_paramsGuiCompPtr, "ParamsGui", "GUI object used to display parameters", false, "ParamsGui")
			I_ASSIGN(m_paramsObserverCompPtr, "ParamsGui", "GUI object used to display parameters", false, "ParamsGui")
			I_ASSIGN(m_selectionParamCompPtr, "SelectionParam", "Selection parameter where actual selected index will be set", false, "SelectionParam")
			I_ASSIGN(m_allowAddRemoveAttrPtr, "AllowAddRemove", "If it is false, 'Add' and 'Remove' buttons will be always hidden", true, true)
		I_END_COMPONENT

		enum ChangeFlags
		{
			CF_PARAMS_ADD = 0x40,
			CF_PARAMS_REMOVE = 0x80,
		};

		CParamsManagerGuiComp();

		// reimplemented (imod::IModelEditor)
		virtual void UpdateModel() const;
		virtual void UpdateEditor();

	protected slots:
		void on_AddButton_clicked();
		void on_RemoveButton_clicked();
		void on_ParamsTree_itemSelectionChanged();

	protected:
		void UpdateActions();
		void UpdateTree();
		int GetSelectedIndex() const;
		void EnsureParamsGuiDetached();

		// reimplemented (iqt::TGuiObserverWrap)
		virtual void OnGuiModelShown();
		virtual void OnGuiModelAttached();
		virtual void OnGuiModelDetached();

	private:
		I_REF(iqt::IGuiObject, m_paramsGuiCompPtr);
		I_REF(imod::IObserver, m_paramsObserverCompPtr);
		I_REF(iprm::ISelectionParam, m_selectionParamCompPtr);
		I_ATTR(bool, m_allowAddRemoveAttrPtr);

		imod::IModel* m_lastConnectedModelPtr;
	};


	} // namespace iqt


	#endif // !iqt_CParamsManagerGuiComp_included


