#ifndef CAbGuiComp_included
#define CAbGuiComp_included


// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"


// Tutorial includes
#include "IAb.h"

#include "Generated/ui_CAbGuiComp.h"


/**
	UI-observer component of IAb interface.
*/
class CAbGuiComp: public iqtgui::TDesignerGuiObserverCompBase<Ui::CAbGuiComp, IAb>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CAbGuiComp, IAb> BaseClass;
	
	I_BEGIN_COMPONENT(CAbGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);


	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

private Q_SLOTS:
	void OnAValueChanged(int value);
	void OnBValueChanged(const QString& value);
};


#endif // !CAbGuiComp_included


