#ifndef iqtgui_CObjectPreviewGuiComp_included
#define iqtgui_CObjectPreviewGuiComp_included


// Qt includes
#include <QFileSystemWatcher>
#include <QDateTime>


// ACF includes
#include "iser/IFileLoader.h"

#include "iprm/IFileNameParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/Generated/ui_CObjectPreviewGuiComp.h"


namespace iqtgui
{


class CObjectPreviewGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CObjectPreviewGuiComp, iprm::IFileNameParam>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CObjectPreviewGuiComp, iprm::IFileNameParam> BaseClass;

	I_BEGIN_COMPONENT(CObjectPreviewGuiComp);
		I_ASSIGN(m_fileLoaderCompPtr, "FileLoader", "Object loader", true, "FileLoader");
		I_ASSIGN(m_objectCompPtr, "Object", "Object", true, "Object");
		I_ASSIGN_TO(m_objectModelCompPtr, m_objectCompPtr, true);
		I_ASSIGN(m_objectObserverCompPtr, "ObjectView", "View component for the object", true, "ObjectView");
		I_ASSIGN_TO(m_objectGuiCompPtr, m_objectObserverCompPtr, true);
	I_END_COMPONENT;

protected:
	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor(int updateFlags = 0);
	virtual void UpdateModel() const;

	// reimplemented (imod::TGuiObserverWrap)
	virtual void OnGuiModelDetached();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private Q_SLOTS:
	void OnFileChanged(const QString& filePath);

private:
	void UpdateObjectFromFile();

private:
	I_REF(iser::IFileLoader, m_fileLoaderCompPtr);
	I_REF(istd::IChangeable, m_objectCompPtr);
	I_REF(imod::IModel, m_objectModelCompPtr);
	I_REF(imod::IObserver, m_objectObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_objectGuiCompPtr);

	QFileSystemWatcher m_fileSystemObserver;

	QDateTime m_lastModificationTimeStamp;
	QString m_lastFilePath;
};


} // namespace iqtgui


#endif // !iqtgui_CObjectPreviewGuiComp_included


