#ifndef idoc_CSingleDocumentManagerBase_included
#define idoc_CSingleDocumentManagerBase_included


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QString>

// ACF includes
#include "istd/TDelPtr.h"
#include "iser/IArchive.h"
#include "imod/CSingleModelObserverBase.h"
#include "idoc/CTmplBasedDocumentManagerBase.h"


namespace idoc
{


/**
	Basic implementation of a template-based single document manager.
*/
class CSingleDocumentManagerBase:
			public idoc::CTmplBasedDocumentManagerBase,
			protected imod::CSingleModelObserverBase
{
public:
	// reimplemented (idoc::IDocumentManager)
	virtual idoc::IUndoManager* GetUndoManagerForDocument(const istd::IChangeable* documentPtr) const;
	virtual int GetDocumentsCount() const;
	virtual istd::IChangeable& GetDocumentFromIndex(int index, DocumentInfo* documentInfoPtr = NULL) const;
	virtual int GetViewsCount(int documentIndex) const;
	virtual istd::IPolymorphic* GetViewFromIndex(int documentIndex, int viewIndex) const;
	virtual istd::IPolymorphic* GetActiveView() const;
	virtual istd::IChangeable* GetDocumentFromView(const istd::IPolymorphic& view, DocumentInfo* documentInfoPtr = NULL) const;
	virtual istd::IPolymorphic* AddViewToDocument(const istd::IChangeable& document, const QByteArray& viewTypeId = QByteArray());
	virtual QByteArray GetDocumentTypeId(const istd::IChangeable& document) const;
	virtual bool InsertNewDocument(
				const QByteArray& documentTypeId,
				bool createView = true,
				const QByteArray& viewTypeId = "",
				istd::IChangeable** newDocumentPtr = NULL);
	virtual bool OpenDocument(
				const QByteArray* documentTypeIdPtr,
				const QString* fileNamePtr = NULL,
				bool createView = true,
				const QByteArray& viewTypeId = "",
				istd::IChangeable** documentPtr = NULL,
				FileToTypeMap* loadedMapPtr = NULL);
	virtual bool SaveDocument(
				int documentIndex = -1,
				bool requestFileName = false,
				FileToTypeMap* savedMapPtr = NULL);
	virtual bool SaveDirtyDocuments(bool beQuiet = false, bool* ignoredPtr = NULL);
	virtual void CloseDocument(int documentIndex = -1, bool beQuiet = false, bool* ignoredPtr = NULL);
	virtual void CloseCurrentView(bool beQuiet = false, bool* ignoredPtr = NULL);

protected:
	/**
		Open single document using its file path.
		\param	filePath		file path.
		\param	createView		if true, instance of view will be created.
		\param	viewTypeId		optional view type ID should be created.
		\param	documentTypeId	output parameter returning loaded ducument type ID.
		\return	instance of created document or NULL if error is occured.
	*/
	virtual bool OpenDocument(
				const QString& filePath,
				bool createView,
				const QByteArray& viewTypeId,
				QByteArray& documentTypeId);

	/**
		Create instance of specified document without attaching to this manager.
	*/
	bool NewDocument(
				const QByteArray& documentTypeId,
				bool createView,
				const QByteArray& viewTypeId,
				bool initialize);
	void EnsureViewRemoved();

	QString GetCurrentDocumentFilePath() const;
	bool HasDocumentPendingChanges() const;

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// abstract methods

	/**
		Called after view is registered.
	*/
	virtual void OnViewRegistered(istd::IPolymorphic* viewPtr) = 0;

	/**
		Called before view is removed.
	*/
	virtual void OnViewRemoved(istd::IPolymorphic* viewPtr) = 0;

	/**
		Gets open file names.
	*/
	virtual QString GetOpenFilePath(const QByteArray* documentTypeIdPtr = NULL) const  = 0;

	/**
		Gets save file name.
	*/
	virtual QString GetSaveFilePath(const QByteArray& documentTypeId, const QString& currentFilePath) const = 0;

	/**
		Query user if this document can be closed.
		\param	ignoredPtr	optional return flag indicating that user ignored this close operation.
	*/
	virtual bool QueryDocumentSave(bool* ignoredPtr) = 0;

	/**
		Serializes open documents information
	*/
	bool SerializeOpenDocument(iser::IArchive& archive);

private:
	QString m_filePath;
	QByteArray m_documentTypeId;
	QByteArray m_viewTypeId;
	istd::TDelPtr<istd::IChangeable> m_documentPtr;
	istd::TDelPtr<idoc::IUndoManager> m_undoManagerPtr;
	istd::TDelPtr<istd::IPolymorphic> m_viewPtr;

	bool m_isDirty;
};


} // namespace idoc


#endif // idoc_CSingleDocumentManagerBase_included


