#ifndef iqtgui_CGuiComponentDialog_included
#define iqtgui_CGuiComponentDialog_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#else
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#endif

// ACF includes
#include "icomp/CComponentBase.h"

#include "iqtgui/IGuiObject.h"


namespace iqtgui
{


/**
	Class to display an ACF GUI component in a dialog.
*/
class CGuiComponentDialog: public QDialog
{
public:
	typedef QDialog BaseClass;

	/**
		Construct the component dialog.
		\param guiObjectPtr Pointer to the gui object.
		\param buttons You can define some standard buttons for this dialog.
		Default no buttons are set and the dialog starts only with the "Close" button.
		\sa QDialogButtonBox::StandardButtons
		\param isModal controls the modality of this dialog.
		\param parentWidgetPtr - parent widget for this dialog. Default is NULL.
	*/
	CGuiComponentDialog(
				iqtgui::IGuiObject* guiObjectPtr,
				int buttons = 0,
				bool isModal = true,
				QWidget* parentWidgetPtr = NULL);

	virtual ~CGuiComponentDialog();

	/**
		Set initial size and position of the dialog.
		\param screenFactor	Size factor related to the screen size. If the value is smaller or equal 0.0 the default size will be used.
		\param positionPtr	Position of the dialog on the screen. If not set, the dialog will be placed at the center of the screen.
	*/
	void SetDialogGeometry(double screenFactor = 0.0, const QPoint* positionPtr = NULL);

	/**
		Returns the internal button box.
		You can use this function to create your own connections to the signals of the button group.
	*/
	const QDialogButtonBox* GetButtonBoxPtr() const;

	/**
		Returns pointer to the connected GUI object.
	*/
	const iqtgui::IGuiObject* GetGuiPtr() const;

protected:
	// reimplemented (QWidget)
	virtual void closeEvent(QCloseEvent* eventPtr);
	virtual void keyPressEvent(QKeyEvent* eventPtr);
	virtual void showEvent(QShowEvent* eventPtr);

private:
	QDialogButtonBox* m_buttonsBox;
	iqtgui::IGuiObject* m_guiObjectPtr;

	double m_screenFactor;
	const QPoint* m_screenPositionPtr;
};


// public inline methods

inline const QDialogButtonBox* CGuiComponentDialog::GetButtonBoxPtr() const
{
	return m_buttonsBox;
}


inline const iqtgui::IGuiObject* CGuiComponentDialog::GetGuiPtr() const
{
	return m_guiObjectPtr;
}


} // namespace iqtgui


#endif // !iqtgui_CGuiComponentDialog_included


