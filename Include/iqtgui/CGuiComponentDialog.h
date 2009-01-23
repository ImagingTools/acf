#ifndef iqtgui_CGuiComponentDialog_included
#define iqtgui_CGuiComponentDialog_included


// Qt includes
#include <QDialog>
#include <QDialogButtonBox>


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
		\sa QDialogButtonBox::StandardButtons. Default no buttons are set and the dialog starts only with close button

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
		Returns the internal button box. 
		You can use this function to create your own connections to the signals of the button group.
	*/
	const QDialogButtonBox* GetButtonBoxPtr() const;

private:
	QDialogButtonBox* m_buttonsBox;
	iqtgui::IGuiObject* m_guiObjectPtr;
};


// public inline methods

inline const QDialogButtonBox* CGuiComponentDialog::GetButtonBoxPtr() const
{
	return m_buttonsBox;
}


} // namespace iqtgui


#endif // !iqtgui_CGuiComponentDialog_included

