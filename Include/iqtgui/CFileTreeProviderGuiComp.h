#ifndef iqtgui_CFileTreeProviderGuiComp_included
#define iqtgui_CFileTreeProviderGuiComp_included


// ACF includes
#include "ifile/IFileNameParam.h"
#include "ibase/IQtItemModelProvider.h"
#include "imod/CMultiModelDispatcherBase.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtgui/Generated/ui_CFileTreeProviderGuiComp.h"


namespace iqtgui
{


class CFileTreeProviderGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CFileTreeProviderGuiComp, ibase::IQtItemModelProvider>,
			public imod::CMultiModelDispatcherBase
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CFileTreeProviderGuiComp, ibase::IQtItemModelProvider> BaseClass;

	I_BEGIN_COMPONENT(CFileTreeProviderGuiComp);
		I_ASSIGN(m_currentFileCompPtr, "CurrentFile", "Write name of the currently selected file to", false, "CurrentFile");
		I_ASSIGN_TO(m_currentFileModelCompPtr, m_currentFileCompPtr, false);
	I_END_COMPONENT;

	enum DataRoles{
		DR_PATH = Qt::UserRole + 1
	};

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

private Q_SLOTS:
	void OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
	void on_Refresh_clicked();

private:
	bool m_fileModelUpdateAllowed;

	I_REF(ifile::IFileNameParam, m_currentFileCompPtr);
	I_REF(imod::IModel, m_currentFileModelCompPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CFileTreeProviderGuiComp_included


