#ifndef idoc_CMultiDocumentManagerBase_included
#define idoc_CMultiDocumentManagerBase_included


// Qt includes
#include <QtCore/QList>

// ACF includes
#include "istd/TPointerVector.h"
#include "istd/TDelPtr.h"
#include "imod/CMultiModelObserverBase.h"
#include "idoc/IUndoManager.h"
#include "idoc/CTmplBasedDocumentManagerBase.h"
#include "iser/IArchive.h"


namespace idoc
{


class CMultiDocumentManagerBase: public idoc::CTmplBasedDocumentManagerBase
{
public:
	CMultiDocumentManagerBase();

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
	typedef istd::TDelPtr<istd::IChangeable> DocumentPtr;
	typedef istd::TDelPtr<idoc::IUndoManager> UndoManagerPtr;
	typedef istd::TDelPtr<istd::IPolymorphic> ViewPtr;
	struct ViewInfo
	{
		ViewPtr viewPtr;
		QByteArray viewTypeId;
	};
	typedef QList<ViewInfo> Views;

	struct SingleDocumentData: public DocumentInfo, public imod::CMultiModelObserverBase
	{
		SingleDocumentData(
					CMultiDocumentManagerBase* parentPtr,
					const QByteArray& documentTypeId,
					istd::IChangeable* documentPtr,
					idoc::IUndoManager* undoManagerPtr);

		CMultiDocumentManagerBase* parentPtr;
		DocumentPtr documentPtr;
		UndoManagerPtr undoManagerPtr;
		Views views;

	protected:
		// reimplemented (imod::CMultiModelObserverBase)
		virtual void OnUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	};

	/**
		Indicate that some view is active now.
	*/
	virtual void SetActiveView(istd::IPolymorphic* viewPtr);

	/**
		Open single document using its file path.
		\param	filePath		file path.
		\param	createView		if true, instance of view will be created.
		\param	viewTypeId		optional view type ID should be created.
		\param	documentTypeId	output parameter returning loaded ducument type ID.
		\return	instance of created document or NULL if error is occured.
	*/
	virtual istd::IChangeable* OpenDocument(
				const QString& filePath,
				bool createView,
				const QByteArray& viewTypeId,
				QByteArray& documentTypeId);

	virtual void CloseAllDocuments();

	SingleDocumentData& GetSingleDocumentData(int index) const;

	/**
		Get document info assigned to active view.
	*/
	SingleDocumentData* GetActiveDocumentInfo() const;

	/**
		Get document info assigned to specified view.
	*/
	SingleDocumentData* GetDocumentInfoFromView(const istd::IPolymorphic& view) const;

	/**
		Get document info assigned to specified file.
	*/
	SingleDocumentData* GetDocumentInfoFromPath(const QString& filePath) const;

	/**
		Get position index of the given document in the document list.
	*/
	int GetDocumentIndex(const SingleDocumentData& document) const;

	/**
		Create instance of specified document without attaching to this manager.
	*/
	virtual SingleDocumentData* CreateDocument(
				const QByteArray& documentTypeId,
				bool createView,
				const QByteArray& viewTypeId,
				bool initialize) const;

	/**
		Register (attach) created document as new working document.
	*/
	bool RegisterDocument(SingleDocumentData* documentPtr);

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
	virtual QStringList GetOpenFilePaths(const QByteArray* documentTypeIdPtr = NULL) const  = 0;

	/**
		Gets save file name.
	*/
	virtual QString GetSaveFilePath(const QByteArray& documentTypeId, const QString& currentFilePath) const = 0;

	/**
		Query user if this document should be saved.
		\param	info		document info of document will be closed.
		\param	ignoredPtr	optional return flag indicating that user ignored this close operation.
	*/
	virtual bool QueryDocumentSave(const SingleDocumentData& info, bool* ignoredPtr) = 0;

	/**
		Serializes open documents information
	*/
	bool SerializeOpenDocumentList(iser::IArchive& archive);

private:
	typedef istd::TPointerVector<SingleDocumentData> DocumentInfos;

	DocumentInfos m_documentInfos;

	istd::IPolymorphic* m_activeViewPtr;
};


} // namespace idoc


#endif // idoc_CMultiDocumentManagerBase_included


