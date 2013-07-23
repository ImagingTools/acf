#include "iqtdoc/CDocumentMetaInfoEditorComp.h"


// Qt includes
#include <QtGui/QFormLayout>


namespace iqtdoc
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CDocumentMetaInfoEditorComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ResetItems();

	QWidget* containerWidgetPtr = GetWidget();
	Q_ASSERT(containerWidgetPtr != NULL);

	QFormLayout* formLayoutPtr = dynamic_cast<QFormLayout*>(containerWidgetPtr->layout());
	Q_ASSERT(formLayoutPtr != NULL);

	typedef idoc::IDocumentMetaInfo::MetaInfoTypes MetaInfoTypes;

	idoc::IDocumentMetaInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		MetaInfoTypes types = objectPtr->GetSupportedMetaInfoTypes();

		for (		MetaInfoTypes::ConstIterator index = types.constBegin();
					index != types.constEnd();
					++index){
			int metaInfoType = *index;

			MetaInfoItem metaInfoItem;

			QString metaInfoLabel = objectPtr->GetMetaInfoName(metaInfoType);
			QString metaInfo = objectPtr->GetDocumentMetaInfo(metaInfoType).toString();

			if (!metaInfoLabel.isEmpty()){
				metaInfoItem.labelPtr = new QLabel(metaInfoLabel, containerWidgetPtr);
				metaInfoItem.editPtr = new QLineEdit(metaInfo, containerWidgetPtr);

				m_metaInfoItemsMap[metaInfoType] = metaInfoItem;

				formLayoutPtr->addRow(metaInfoItem.labelPtr, metaInfoItem.editPtr);
			}
		}
	}
}


void CDocumentMetaInfoEditorComp::OnGuiModelDetached()
{
	ResetItems();

	BaseClass::OnGuiModelDetached();
}


void CDocumentMetaInfoEditorComp::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());
}


// reimplemented (iqtgui::CComponentBase)

void CDocumentMetaInfoEditorComp::OnGuiCreated()
{
	QWidget* containerWidgetPtr = GetWidget();
	Q_ASSERT(containerWidgetPtr != NULL);

	QFormLayout* formLayoutPtr = new QFormLayout(containerWidgetPtr);

	containerWidgetPtr->setLayout(formLayoutPtr);

	BaseClass::OnGuiCreated();
}


// private methods

void CDocumentMetaInfoEditorComp::ResetItems()
{
	for (		MetaInfoItemsMap::Iterator index = m_metaInfoItemsMap.begin();
				index != m_metaInfoItemsMap.end();
				++index){
		MetaInfoItem& item = index.value();

		delete item.labelPtr;
		delete item.editPtr;
	}

	m_metaInfoItemsMap.clear();
}


} // namespace iqtdoc


