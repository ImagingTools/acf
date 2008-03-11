#ifndef iqt_CGuiComponentBase_included
#define iqt_CGuiComponentBase_included


#include "icomp/CComponentBase.h"

#include "iqt/IGuiObject.h"


namespace iqt
{


class CGuiComponentBase:
			public QObject, 
			public icomp::CComponentBase,
			public IGuiObject
{
public:
	typedef icomp::CComponentBase BaseClass;

	CGuiComponentBase();

	// reimplemented (iqt::IGuiObject)
	virtual bool IsGuiCreated() const;
	virtual bool CreateGui(QWidget* parentPtr);
	virtual bool DestroyGui();
	virtual QWidget* GetWidget() const;
	virtual void OnTryClose(bool* ignoredPtr = NULL);

protected:
	/**
		Called from widget event filter when key is pressed.
		\return	if event was consumed returns true, elsewhere false.
	 */
	virtual bool OnKeyPressed(QKeyEvent* event);

	/**
		Called from widget event filter when slave widget is shown.
	*/
	virtual void OnGuiShown();

	/**
		Called from widget event filter when slave widget is hidden.
	*/
	virtual void OnGuiHidden();

	/**
		Called from widget event filter when GUI should be retranslated.
	 */
	virtual void OnRetranslate();

	/**
		Called just after GUI is initialized.
	 */
	virtual void OnGuiCreated();

	/**
		Called just before GUI is released.
	 */
	virtual void OnGuiDestroyed();

	// reimplemented (QObject)
	bool eventFilter(QObject* obj, QEvent* event);

	// abstract methods
	/**
		Create slave widget object.
	 */
	virtual QWidget* InitWidgetToParent(QWidget* parentPtr) = 0;

private:
	QWidget* m_widgetPtr;
};


// inline methods

inline QWidget* CGuiComponentBase::GetWidget() const
{
	return m_widgetPtr;
}


} // namespace iqt


#endif //!iqt_CGuiComponentBase_included


