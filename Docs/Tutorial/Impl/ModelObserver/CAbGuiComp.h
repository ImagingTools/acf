#ifndef CAbGuiComp_included
#define CAbGuiComp_included


// ACF includes
#include <iqtgui/TDesignerGuiObserverCompBase.h>


// Tutorial includes
#include "IAb.h"

#include <GeneratedFiles/ModelObserver/ui_CAbGuiComp.h>


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
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;
	virtual void OnGuiModelAttached() override;

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated() override;

private Q_SLOTS:
	/**
		The function will be triggered on changes of A.
	*/
	void OnAValueChanged(int value);

	/**
		The function will be triggered on changes of B.
	*/
	void OnBValueChanged(const QString& value);
};


#endif // !CAbGuiComp_included


