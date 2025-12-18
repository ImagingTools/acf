#ifndef iqtgui_CTableBasedModelEditorComp_included
#define iqtgui_CTableBasedModelEditorComp_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QTableView>
#else
#include <QtGui/QTableView>
#endif

// ACF includes
#include <iqtgui/TItemModelEditorCompBase.h>


namespace iqtgui
{


/**
	Standard table based editor for data model components which provide access to Qt's item model (QAbstractItemModel).
	\sa ibase::IQtItemModelProvider
*/
class CTableBasedModelEditorComp: public TItemModelEditorCompBase<QTableView>
{
public:
	typedef TItemModelEditorCompBase<QTableView> BaseClass;

	I_BEGIN_COMPONENT(CTableBasedModelEditorComp);
		I_ASSIGN(m_showHorizontalHeaderAttrPtr, "ShowHorizontalHeader", "If set, horizontal table header is shown", true, true);
		I_ASSIGN(m_showVerticalHeaderAttrPtr, "ShowVerticalHeader", "If set, vertical table header is shown", true, true);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;

private:
	I_ATTR(bool, m_showHorizontalHeaderAttrPtr);
	I_ATTR(bool, m_showVerticalHeaderAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CTableBasedModelEditorComp_included


