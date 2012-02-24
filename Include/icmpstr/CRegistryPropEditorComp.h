#ifndef icmpstr_CRegistryPropEditorComp_included
#define icmpstr_CRegistryPropEditorComp_included


// ACF includes
#include "icomp/IRegistry.h"

#include "ibase/IMessageConsumer.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CCheckableComboBox.h"

#include "icmpstr/IRegistryConsistInfo.h"

#include "icmpstr/Generated/ui_CRegistryPropEditorComp.h"


namespace icmpstr
{


class CRegistryPropEditorComp: public iqtgui::TDesignerGuiObserverCompBase<Ui::CRegistryPropEditorComp, icomp::IRegistry>
{
    Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CRegistryPropEditorComp, icomp::IRegistry> BaseClass;

	I_BEGIN_COMPONENT(CRegistryPropEditorComp);
		I_ASSIGN(m_consistInfoCompPtr, "ConsistencyInfo", "Allows to check consistency of registries and attributes", false, "ConsistencyInfo");
	I_END_COMPONENT;

protected:
	class TextLog:
				public QString,
				virtual public ibase::IMessageConsumer
	{
	public:
		// reimplemented (ibase::IMessageConsumer)
		virtual bool IsMessageSupported(
					int messageCategory = -1,
					int messageId = -1,
					const ibase::IMessage* messagePtr = NULL) const;
		virtual void AddMessage(const MessagePtr& messagePtr);
	};

	void CreateOverview();

	// reimplemented (imod::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

protected Q_SLOTS:
	void on_DescriptionEdit_editingFinished();
	void OnUpdateKeywords();

private:
	static QString ConvertToKeyword(const QString& input, const QString& key = QString());

private:
	I_REF(IRegistryConsistInfo, m_consistInfoCompPtr);

	iqtgui::CCheckableComboBox* m_categoryComboBox;
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryPropEditorComp_included

