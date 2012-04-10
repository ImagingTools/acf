#ifndef icmpstr_CMultiAttributeEditor_included
#define icmpstr_CMultiAttributeEditor_included


// Qt includes
#include <QtGui/QTreeWidget>


// ACF includes
#include "iqtgui/CTreeWidgetFilter.h"
#include "iqtgui/CItemDelegate.h"

#include "icmpstr/CElementSelectionInfoManagerBase.h"
#include "icmpstr/Generated/ui_CMultiAttributeEditor.h"


namespace icmpstr
{


class CMultiAttributeEditor: public QDialog
{
	Q_OBJECT
public:
	CMultiAttributeEditor(
				const CElementSelectionInfoManagerBase& elementSelectionInfoManager,
				const QByteArray& attributeId,
				int attributeFlags);

	/**
		Set the list of the attributes as a text object
	*/
	void SetEditorText(const QString& attributeValuesText);		

	/**
		Get attribute values
	*/
	QStringList GetAttributeValues() const;

protected Q_SLOTS:
	void OnItemAdd();
	void OnItemRemove();
	void OnItemMoveUp();
	void OnItemMoveDown();
	void UpdateButtonStates();

protected:
	void CreateValuesTree(const QStringList& values);

private:
	class ValueItemDelegate: public iqtgui::CItemDelegate
	{
	public:
		typedef iqtgui::CItemDelegate BaseClass;

		ValueItemDelegate(const CMultiAttributeEditor& parent);
		
		// reimplemented (QItemDelegate)
		virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		virtual void setEditorData(QWidget* editor, const QModelIndex& index ) const;
		virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

	private:
		const CMultiAttributeEditor& m_parent;
	};


	const CElementSelectionInfoManagerBase& m_elementSelectionInfoManager;
	QByteArray m_attributeId;
	int m_attributeFlags;

	Ui::CMultiAttributeEditor m_dialog;

	ValueItemDelegate m_valueItemDelegate;
};


} // namespace icmpstr


#endif // !icmpstr_CMultiAttributeEditor_included

