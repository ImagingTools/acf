#ifndef iqtprm_COptionsListGuiComp_included
#define iqtprm_COptionsListGuiComp_included


// Qt includes
#include <QComboBox>
#include <QRadioButton>


// ACF includes
#include "istd/TPointerVector.h"

#include "iprm/ISelectionParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtprm/Generated/ui_COptionsListGuiComp.h"


namespace iqtprm
{


/**
	An editor for an exclusive option selection implemented as a list of options.
*/
class COptionsListGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::COptionsListGuiComp, iprm::ISelectionParam>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::COptionsListGuiComp, iprm::ISelectionParam> BaseClass;

	enum DataRole
	{
		DR_SELECTION_INDEX = Qt::UserRole + 1,
		DR_SELECTION_PARAM
	};

	I_BEGIN_COMPONENT(COptionsListGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

protected Q_SLOTS:
	void OnSelectionChanged(int index);

private:
	void CreateOptionsTree(const iprm::ISelectionParam* selectionParamPtr, QTreeWidgetItem* parentItemPtr = NULL);
};


} // namespace iqtprm


#endif // !iqtprm_COptionsListGuiComp_included


