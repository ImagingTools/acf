#ifndef iqtdoc_CMultiDocumentWorkspaceGuiComp_included
#define iqtdoc_CMultiDocumentWorkspaceGuiComp_included


#include <QWorkspace>


#include "idoc/CDocumentManagerBase.h"

#include "iqt/TGuiComponentBase.h"


namespace iqmain
{


/**	
	This class is a Qt-based workspace implementation of a document manager.
*/
class CMultiDocumentWorkspaceGuiComp:
			public iqt::TGuiComponentBase<QWorkspace>, 
			public idoc::CDocumentManagerBase
{
	Q_OBJECT

public:
	typedef iqt::TGuiComponentBase<QWorkspace> BaseClass;
	typedef idoc::CDocumentManagerBase BaseClass2;

	I_BEGIN_COMPONENT(CMultiDocumentWorkspaceGuiComp)
		I_REGISTER_INTERFACE(idoc::IDocumentManager);
		I_ASSIGN(m_scrollingEnabledAttrPtr, "ScrollingWorkspace", "Enable scrolling of workspace area", true, false)
		I_ASSIGN(m_documentTemplateCompPtr, "DocumentTemplate", "Document template", true, "DocumentTemplate")
	I_END_COMPONENT

	// reimplemented (iqt::IGuiObject)
	virtual void OnTryClose(bool* ignoredPtr = NULL);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

protected:
	virtual bool eventFilter(QObject* obj, QEvent* event);

	/**
		Update titles of views or all views of specified document.
		\param	optional document object, if you want to update only views of single document.
	*/
	void UpdateAllTitles();

	/**
		Find view object associated with specified Qt widget.
		\param	widget	Qt widget representing view.
		\return	associated view or NULL if nothing is found.
	*/
	iqt::IGuiObject* GetViewFromWidget(const QWidget& widget) const;

	/**
		Creates signal/slot connnections for the implementation.
	*/
	virtual void CreateConnections();

	/**
		Creates the filter for the file selection dialog.
		\param	documentTypeIdPtr	optional ID of document type if only filter for single document type should be created.
	*/
	QString CreateFileDialogFilter(const std::string* documentTypeIdPtr = NULL) const;

	// reimplemented (idoc::CDocumentManagerBase)
	virtual istd::CStringList GetOpenFileNames(const std::string* documentTypeIdPtr = NULL) const;
	virtual istd::CString GetSaveFileName(const std::string& documentTypeId) const;
	virtual void OnViewRegistered(istd::IPolymorphic* viewPtr);

	// reimplemented (imod::CMultiModelObserverBase)
	void OnUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (iqt:CGuiComponentBase)
	virtual void OnGuiCreated();	
	virtual void OnGuiDestroyed();

	// reimplemented (istd:IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

protected slots:
	void OnWindowActivated(QWidget* window);

private:
	I_ATTR(bool, m_scrollingEnabledAttrPtr);
	I_REF(idoc::IDocumentTemplate, m_documentTemplateCompPtr);
};


} // namespace iqmain


#endif // iqtdoc_CMultiDocumentWorkspaceGuiComp_included

