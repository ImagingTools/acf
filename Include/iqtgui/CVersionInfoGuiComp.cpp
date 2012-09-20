#include "iqtgui/CVersionInfoGuiComp.h"


namespace iqtgui
{


// reimplemented (iqtgui::CGuiComponentBase)

void CVersionInfoGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	Tree->header()->setResizeMode(QHeaderView::ResizeToContents);

	for (int i = 0; i < m_versionInfosCompPtr.GetCount(); i++){
		iser::IVersionInfo* infoPtr = m_versionInfosCompPtr[i];
		I_ASSERT(infoPtr != NULL);

		iser::IVersionInfo::VersionIds ids = infoPtr->GetVersionIds();
		for (iser::IVersionInfo::VersionIds::const_iterator it = ids.begin(); it != ids.end(); it++){	
			QTreeWidgetItem* itemPtr = new QTreeWidgetItem(Tree);
			
			itemPtr->setText(0, infoPtr->GetVersionIdDescription(*it));
		
			quint32 number = 0;
			infoPtr->GetVersionNumber(*it, number);
			itemPtr->setText(1, infoPtr->GetEncodedVersionName(*it, number));		
		}
	}
}


} // namespace iqtgui
