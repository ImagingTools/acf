#ifndef idoc_CMultiDocumentManagerBase_included
#define idoc_CMultiDocumentManagerBase_included


// STL includes
#include <list>


// ACF includes
#include "istd/TPointerVector.h"
#include "istd/TDelPtr.h"

#include "imod/IUndoManager.h"
#include "imod/CSingleModelObserverBase.h"

#include "idoc/CTmplBasedDocumentManagerBase.h"
#include "idoc/IDocumentStateComparator.h"


namespace idoc
{


class CMultiDocumentManagerBase: public idoc::CTmplBasedDocumentManagerBase
{
public:
	CMultiDocumentManagerBase();

	// reimplemented (idoc::IDocumentManager)
	virtual imod::IUndoManager* GetUndoManagerForDocument(const istd::IChangeable* documentPtr) const;
	virtual int GetDocumentsCount() const;
	virtual istd::IChangeable& GetDocumentFromIndex(int index, DocumentInfo* documentInfoPtr = NULL) const;
	virtual int GetViewsCount(int documentIndex) const;
	virtual istd::IPolymorphic* GetViewFromIndex(int documentIndex, int viewIndex) const;
	virtual istd::IPolymorphic* GetActiveView() const;
	virtual istd::IChangeable* GetDocumentFromView(const istd::IPolymorphic& view, DocumentInfo* documentInfoPtr = NULL) const;
	virtual istd::IPolymorphic* AddViewToDocument(const istd::IChangeable& document, const std::string& viewTypeId = std::string());
	virtual std::string GetDocumentTypeId(const istd::IChangeable& document) const;
	virtual bool FileNew(
				const std::string& documentTypeId, 
				bool createView = true, 
				const std::string& viewTypeId = "",
				istd::IChangeable** newDocumentPtr = NULL);
	virtual bool FileOpen(
				const std::string* documentTypeIdPtr,
				const QString* fileNamePtr = NULL,
				bool createView = true,
				const std::string& viewTypeId = "",
				FileToTypeMap* loadedMapPtr = NULL);
	virtual bool FileSave(
				int documentIndex = -1,
				bool requestFileName = false,
				FileToTypeMap* savedMapPtr = NULL);
	virtual void FileClose(int documentIndex = -1, bool* ignoredPtr = NULL);

protected:
	typedef istd::TDelPtr<istd::IChangeable> DocumentPtr;
	typedef istd::TDelPtr<imod::IUndoManager> UndoManagerPtr;
	typedef istd::TDelPtr<IDocumentStateComparator> StateComparatorPtr;
	typedef istd::TDelPtr<istd::IPolymorphic> ViewPtr;
	typedef std::list<ViewPtr> Views;

	struct SingleDocumentData: public DocumentInfo, public imod::CSingleModelObserverBase
	{
		SingleDocumentData(
					CMultiDocumentManagerBase* parentPtr,
					const std::string& documentTypeId,
					istd::IChangeable* documentPtr,
					imod::IUndoManager* undoManagerPtr,
					IDocumentStateComparator* stateComparatorPtr)
		{
			this->parentPtr = parentPtr;
			this->documentTypeId = documentTypeId;
			this->documentPtr.SetPtr(documentPtr);
			this->undoManagerPtr.SetPtr(undoManagerPtr);
			this->stateComparatorPtr.SetPtr(stateComparatorPtr);
			isDirty = false;

			if ((documentPtr != NULL) && (stateComparatorPtr != NULL)){
				stateComparatorPtr->StoreState(*documentPtr);
			}
		}

		CMultiDocumentManagerBase* parentPtr;
		DocumentPtr documentPtr;
		UndoManagerPtr undoManagerPtr;
		StateComparatorPtr stateComparatorPtr;
		Views views;

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);
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
				const std::string& viewTypeId,
				std::string& documentTypeId);

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
		Create instance of specified document without attaching to this manager.
	*/
	SingleDocumentData* CreateDocument(const std::string& documentTypeId, bool createView, const std::string& viewTypeId) const;

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
	virtual QStringList GetOpenFilePaths(const std::string* documentTypeIdPtr = NULL) const  = 0;

	/**
		Gets save file name.
	*/
	virtual QString GetSaveFilePath(const std::string& documentTypeId) const = 0;

	/**
		Query user if this document can be closed.
		\param	info		document info of document will be closed.
		\param	ignoredPtr	optional return flag indicating that user ignored this close operation.
	*/
	virtual void QueryDocumentClose(const SingleDocumentData& info, bool* ignoredPtr) = 0;

private:
	typedef istd::TPointerVector<SingleDocumentData> DocumentInfos;

	DocumentInfos m_documentInfos;

	istd::IPolymorphic* m_activeViewPtr;
};


} // namespace idoc


#endif // idoc_CMultiDocumentManagerBase_included


