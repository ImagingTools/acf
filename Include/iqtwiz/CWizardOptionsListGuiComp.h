#ifndef iqtwiz_CWizardOptionsListGuiComp_included
#define iqtwiz_CWizardOptionsListGuiComp_included


// Qt includes
#include <QComboBox>
#include <QRadioButton>


// ACF includes
#include "istd/TPointerVector.h"

#include "iprm/IParamsManager.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtwiz/Generated/ui_CWizardOptionsListGuiComp.h"


namespace iqtwiz
{


/**
	An editor for an exclusive option selection implemented as a list of options.
*/
class CWizardOptionsListGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CWizardOptionsListGuiComp, iprm::IParamsManager>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CWizardOptionsListGuiComp, iprm::IParamsManager> BaseClass;

	enum DataRole
	{
		DR_SELECTION_INDEX = Qt::UserRole + 1,
		DR_OBJECT_PTR
	};

	I_BEGIN_COMPONENT(CWizardOptionsListGuiComp);
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
	void OnSelectionChanged();

private:
	void CreateOptionsTree(const iprm::IParamsManager* paramsManagerPtr, QTreeWidgetItem* parentItemPtr = NULL);
};


} // namespace iqtwiz


#endif // !iqtwiz_CWizardOptionsListGuiComp_included


