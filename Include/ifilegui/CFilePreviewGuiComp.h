#ifndef ifilegui_CFilePreviewGuiComp_included
#define ifilegui_CFilePreviewGuiComp_included


// Qt includes
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include <QtCore/QFutureWatcher>
#include <QtWidgets/QGraphicsView>

// ACF includes
#include <ifile/IFilePersistence.h>
#include <ifile/IFileNameParam.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <GeneratedFiles/ifilegui/ui_CFilePreviewGuiComp.h>


namespace ifilegui
{


/**
	Component for the preview of the file contents.

	The component also observers the file system changes and updates the file preview automatically, if the file was changed.
*/
class CFilePreviewGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CFilePreviewGuiComp, ifile::IFileNameParam>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CFilePreviewGuiComp, ifile::IFileNameParam> BaseClass;

	I_BEGIN_COMPONENT(CFilePreviewGuiComp);
		I_ASSIGN(m_fileLoaderCompPtr, "FileLoader", "Object loader", true, "FileLoader");
		I_ASSIGN(m_objectFactoryCompPtr, "ObjectFactory", "Factory for creation of data object to be previewed", true, "ObjectFactory");
		I_ASSIGN(m_currentPreviewObjectCompPtr, "CurrentPreviewObject", "Current object will be copied here if set", false, "CurrentPreviewObject");
		I_ASSIGN(m_objectObserverCompPtr, "ObjectView", "View component for the object", true, "ObjectView");
		I_ASSIGN_TO(m_objectGuiCompPtr, m_objectObserverCompPtr, true);
		I_ASSIGN(m_isAsynchronPreviewGenerationEnabledAttrPtr, "AsynchronPreviewGenerationEnabled", "If enabled, the preview generation will be done in a separate thread", true, true);
		I_ASSIGN(m_noAvailableLabelAttrPtr, "NoAvailableLabel", "Text used for no available preview state", false, "");
		I_ASSIGN(m_noAvailableIconPathAttrPtr, "NoAvailableIconPath", "Path to the icon used for no available preview state", false, "");
	I_END_COMPONENT;

	CFilePreviewGuiComp();

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiRetranslate();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private Q_SLOTS:
	void UpdateFilePreview();
	void OnPreviewGenerationFinished();

private:
	void UpdateObjectFromFile();
	void ResetPreview();
	
protected:
	/**
		Helper class to watch after QGraphicsView Resize
	*/
	class PreviewWidget : public QGraphicsView
	{
	public:
		PreviewWidget(QWidget *parent = NULL);

	protected:
		// reimplemented (QWidget)
		void resizeEvent(QResizeEvent* eventPtr);
	};

protected:
	I_REF(ifile::IFilePersistence, m_fileLoaderCompPtr);
	I_FACT(istd::IChangeable, m_objectFactoryCompPtr);
	I_REF(imod::IObserver, m_objectObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_objectGuiCompPtr);	
	I_REF(istd::IChangeable, m_currentPreviewObjectCompPtr);
	I_ATTR(bool, m_isAsynchronPreviewGenerationEnabledAttrPtr);
	I_TEXTATTR(m_noAvailableLabelAttrPtr);
	I_ATTR(QByteArray, m_noAvailableIconPathAttrPtr);

	QFileSystemWatcher m_fileSystemObserver;

	QDateTime m_lastModificationTimeStamp;
	QString m_lastFilePath;

	QFutureWatcher<void> m_previewGenerationWatcher;

	istd::TDelPtr<istd::IChangeable> m_previewObjectPtr;
	istd::TDelPtr<istd::IChangeable> m_workingObjectPtr;

	QMutex m_mutex;

	bool m_previewWasGenerated;
	QTimer m_timer;
};


} // namespace ifilegui


#endif // !ifilegui_CFilePreviewGuiComp_included


