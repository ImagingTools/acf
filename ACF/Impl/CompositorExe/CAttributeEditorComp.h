#ifndef CAttributeEditorComp_included
#define CAttributeEditorComp_included

#include <QtGui/QWidget>
#include <QtGui/QComboBox>
#include <QItemDelegate>

#include "iqt/TDesignerGuiObserverCompBase.h"

#include "icomp/IRegistryElement.h"

#include "Generated/ui_CAttributeEditorComp.h"

#include "IAttributeSelectionObserver.h"


class CAttributeEditorComp: public iqt::TDesignerGuiObserverCompBase<Ui::CAttributeEditorComp, icomp::IRegistryElement>
{
    Q_OBJECT

public:
	typedef iqt::TDesignerGuiObserverCompBase<Ui::CAttributeEditorComp, icomp::IRegistryElement> BaseClass;
	I_BEGIN_COMPONENT(CAttributeEditorComp)
		I_REGISTER_INTERFACE(imod::IObserver)
		I_ASSIGN(m_attributeSelectionObserverCompPtr, "AttributeSelectionObserver", "Attribute selection observer", false, "AttributeSelectionObserver")
	I_END_COMPONENT

	enum
	{
		Dependency = 0x1,
		MultipleDependency,
		Attribute,
		SelectableAttribute,
		MultipleAttribute
	};

	enum AttributeColumns
	{
		NameColumn = 0,
		ValueColumn = 1
	};

	enum
	{
		AttributeMining = Qt::UserRole + 1,
		AttributeId = Qt::UserRole + 2
	};

public:
	const icomp::IRegistryElement::AttributeInfo* GetRegistryAttribute(const QString& attributeId) const;
	const icomp::IAttributeStaticInfo* GetStaticAttributeInfo(const QString& attributeId) const;

	CAttributeEditorComp();

	// reimplemented (TGuiObserverWrap)
	virtual void OnGuiModelDetached();

	// reimplmented (imod::IModelEditor)
	virtual void UpdateEditor();
	virtual void UpdateModel() const;

protected slots:
	void OnItemSelected();

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

signals:
	void selectedDependencyInterface(const QString& interfaceId);
	void selectedPropertyType(const QString& propertyType);

private:
	friend class AttributeItemDelegate;

	class AttributeItemDelegate: public QItemDelegate
	{
	public:
		AttributeItemDelegate(CAttributeEditorComp& parent);
		
		// reimplemented (QItemDelegate)
		virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
		virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		virtual void setEditorData(QWidget* editor, const QModelIndex& index ) const;
		virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
		virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
	private:
		CAttributeEditorComp& m_parent;
	};

	typedef std::map<std::string, QString> AttributeTypesMap;
	AttributeTypesMap m_attributeTypesMap;

	I_REF(IAttributeSelectionObserver, m_attributeSelectionObserverCompPtr);
};


#endif // !CAttributeEditorComp_included

