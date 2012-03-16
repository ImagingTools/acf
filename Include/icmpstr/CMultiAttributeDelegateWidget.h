#ifndef icmpstr_CMultiAttributeDelegateWidget_included
#define icmpstr_CMultiAttributeDelegateWidget_included


// Qt includes
#include <QtGui/QWidget>
#include <QtGui/QIcon>
#include <QtGui/QItemDelegate>


// ACF includes
#include "icmpstr/CElementSelectionInfoManagerBase.h"


namespace icmpstr
{


class CMultiAttributeDelegateWidget: public QWidget
{
    Q_OBJECT
public:
	typedef QWidget BaseClass;

public:
	CMultiAttributeDelegateWidget(
				QItemDelegate& itemDelegate,
				const CElementSelectionInfoManagerBase& elementSelectionInfoManager,
				QWidget* parentWidget,
				const std::string& attributeId,
				int attributeFlags);

	QString GetText() const;
	void SetText(const QString& text);

protected Q_SLOTS:
	void OnShowDialog();

private:
	QItemDelegate& m_itemDelegate;
	QLineEdit* m_textEditor;
	std::string m_attributeId;
	int m_attributeFlags;

	const CElementSelectionInfoManagerBase& m_elementSelectionInfoManager;
};


} // namespace icmpstr


#endif // !icmpstr_CMultiAttributeDelegateWidget_included

