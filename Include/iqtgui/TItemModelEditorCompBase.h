#ifndef iqtgui_TItemModelEditorCompBase_included
#define iqtgui_TItemModelEditorCompBase_included


// Qt includes
#include <QtCore/QAbstractItemModel>

// ACF includes
#include <ibase/IQtItemModelProvider.h>

#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <iqtgui/TGuiObserverWrap.h>


namespace iqtgui
{


/**
	Basic generic implementation of a data model editor for components which provide access to Qt's item model (QAbstractItemModel).
	\sa ibase::IQtItemModelProvider
*/
template <class ItemWidgetClass>
class TItemModelEditorCompBase:
				public ibase::TModelObserverCompWrap<
							TGuiObserverWrap<
										TGuiComponentBase<ItemWidgetClass>,
										imod::TSingleModelObserverBase<ibase::IQtItemModelProvider> > >
{
public:
	typedef ibase::TModelObserverCompWrap<
				TGuiObserverWrap<
							TGuiComponentBase<ItemWidgetClass>,
							imod::TSingleModelObserverBase<ibase::IQtItemModelProvider> > > BaseClass;

	I_BEGIN_COMPONENT(TItemModelEditorCompBase);
		I_ASSIGN(m_sortingEnabledAttrPtr, "SortingEnabled", "If set, sorting of table data is enabled", true, true);
	I_END_COMPONENT;

protected:
	using BaseClass::GetObservedObject;
	using BaseClass::GetQtWidget;

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (TGuiObserverWrap)
	virtual void OnGuiModelAttached();

private:
	I_ATTR(bool, m_sortingEnabledAttrPtr);
};


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

template <class ItemWidgetClass>
void TItemModelEditorCompBase<ItemWidgetClass>::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ibase::IQtItemModelProvider* objectPtr = BaseClass::GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	QAbstractItemModel* itemModelPtr = const_cast<QAbstractItemModel*>(objectPtr->GetItemModel());
	if (itemModelPtr != NULL){
		ItemWidgetClass* viewPtr = BaseClass::GetQtWidget();
		Q_ASSERT(viewPtr != NULL);

		viewPtr->setModel(itemModelPtr);
	}
}


// reimplemented (CGuiComponentBase)

template <class ItemWidgetClass>
void TItemModelEditorCompBase<ItemWidgetClass>::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	ItemWidgetClass* viewPtr = BaseClass::GetQtWidget();

	Q_ASSERT(viewPtr != NULL);
	Q_ASSERT(m_sortingEnabledAttrPtr.IsValid());

	viewPtr->setSortingEnabled(*m_sortingEnabledAttrPtr);
}


} // namespace iqtgui


#endif // !iqtgui_TItemModelEditorCompBase_included


