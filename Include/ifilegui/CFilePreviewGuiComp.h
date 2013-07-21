#ifndef ifilegui_CFilePreviewGuiComp_included
#define ifilegui_CFilePreviewGuiComp_included


// Qt includes
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QDateTime>

// ACF includes
#include "ifile/IFilePersistence.h"
#include "ifile/IFileNameParam.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "ifilegui/Generated/ui_CFilePreviewGuiComp.h"


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
		I_ASSIGN(m_objectCompPtr, "Object", "Object", true, "Object");
		I_ASSIGN_TO(m_objectModelCompPtr, m_objectCompPtr, true);
		I_ASSIGN(m_objectObserverCompPtr, "ObjectView", "View component for the object", true, "ObjectView");
		I_ASSIGN_TO(m_objectGuiCompPtr, m_objectObserverCompPtr, true);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private Q_SLOTS:
	void OnFileChanged(const QString& filePath);

private:
	void UpdateObjectFromFile();

private:
	I_REF(ifile::IFilePersistence, m_fileLoaderCompPtr);
	I_REF(istd::IChangeable, m_objectCompPtr);
	I_REF(imod::IModel, m_objectModelCompPtr);
	I_REF(imod::IObserver, m_objectObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_objectGuiCompPtr);

	QFileSystemWatcher m_fileSystemObserver;

	QDateTime m_lastModificationTimeStamp;
	QString m_lastFilePath;
};


} // namespace ifilegui


#endif // !ifilegui_CFilePreviewGuiComp_included


