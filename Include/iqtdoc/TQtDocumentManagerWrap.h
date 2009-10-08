#ifndef iqtdoc_TQtDocumentManagerWrap_included
#define iqtdoc_TQtDocumentManagerWrap_included


// Qt includes
#include <QPrintDialog>


// ACF includes
#include "iqtdoc/IPrintable.h"



namespace iqtdoc
{


/**	
	Wrapper for general functionality of a Qt based document workspace.
*/
template <typename Base>
class TQtDocumentManagerWrap: public Base
{
public:
	typedef Base BaseClass;

	// pseudo-reimplemented (idoc::IDocumentManager)
	virtual int GetAllowedOperationFlags(const istd::IPolymorphic* viewPtr = NULL) const;
	virtual void FilePrint() const;
};



// reimplemented (idoc::IDocumentManager)

template <typename Base>
int TQtDocumentManagerWrap<Base>::GetAllowedOperationFlags(const istd::IPolymorphic* viewPtr) const
{
	int retVal = BaseClass::GetAllowedOperationFlags(viewPtr);

	if (viewPtr == NULL){
		viewPtr = GetActiveView();
	}

	if (dynamic_cast<const iqtdoc::IPrintable*>(viewPtr) != NULL){
		retVal |= OF_FILE_PRINT;
	}

	return retVal;
}


template <typename Base>
void TQtDocumentManagerWrap<Base>::FilePrint() const
{
	istd::IPolymorphic* activeViewPtr = GetActiveView();
	if (activeViewPtr != NULL){
		iqtdoc::IPrintable* printablePtr = dynamic_cast<iqtdoc::IPrintable*>(activeViewPtr);
		if (printablePtr != NULL){
			QPrinter printer(QPrinter::HighResolution);
			QPrintDialog printerDialog(&printer);

			if (printerDialog.exec() == QDialog::Accepted){
				printablePtr->Print(printerDialog.printer());
			}
		}
	}
}


} // namespace iqtdoc


#endif // !iqtdoc_TQtDocumentManagerWrap_included


