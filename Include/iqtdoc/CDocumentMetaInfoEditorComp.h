#ifndef iqtdoc_CDocumentMetaInfoEditorComp_included
#define iqtdoc_CDocumentMetaInfoEditorComp_included


// Qt includes
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

// ACF includes
#include "idoc/IDocumentMetaInfo.h"
#include "imod/TSingleModelObserverBase.h"
#include "ibase/TModelObserverCompWrap.h"
#include "iqtgui/TGuiObserverWrap.h"
#include "iqtgui/TGuiComponentBase.h"


namespace iqtdoc
{


class CDocumentMetaInfoEditorComp:
			public ibase::TModelObserverCompWrap<
						iqtgui::TGuiObserverWrap<
									iqtgui::TGuiComponentBase<QWidget>,
									imod::TSingleModelObserverBase<idoc::IDocumentMetaInfo> > >
{
	Q_OBJECT
public:
	typedef ibase::TModelObserverCompWrap<
		iqtgui::TGuiObserverWrap<
		iqtgui::TGuiComponentBase<QWidget>,
		imod::TSingleModelObserverBase<idoc::IDocumentMetaInfo> > > BaseClass;

	I_BEGIN_COMPONENT(CDocumentMetaInfoEditorComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CComponentBase)
	virtual void OnGuiCreated();

private:
	void ResetItems();

private:
	struct MetaInfoItem
	{
		MetaInfoItem()
			:labelPtr(NULL),
			editPtr(NULL)
		{
		}

		QLabel* labelPtr;
		QLineEdit* editPtr;
	};

	typedef QMap<int, MetaInfoItem> MetaInfoItemsMap;

	MetaInfoItemsMap m_metaInfoItemsMap;
};


} // namespace iqtdoc


#endif // !iqtdoc_CDocumentMetaInfoEditorComp_included


