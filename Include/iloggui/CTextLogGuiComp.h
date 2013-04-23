#ifndef iloggui_CTextLogGuiComp_included
#define iloggui_CTextLogGuiComp_included


// Qt includes
#include <QtGui/QTextTableCell>


// ACF includes
#include "ilog/IMessageContainer.h"
#include "ilog/CMessage.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iloggui/Generated/ui_CTextLogGuiComp.h"


namespace iloggui
{


class CTextLogGuiComp:
	public iqtgui::TDesignerGuiObserverCompBase<Ui::CTextLogGuiComp, ilog::IMessageContainer>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CTextLogGuiComp, ilog::IMessageContainer> BaseClass;

	I_BEGIN_COMPONENT(CTextLogGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiRetranslate();

private Q_SLOTS:
	void DoFilter();

private:
	void UpdateFilters();
	void GenerateDocument(int severityFilter, const QString& sourceFilter, const QString& textFilter);
	void InsertImage(QTextTableCell cell, const QImage& image);
	void InsertText(QTextTableCell cell, const QString& text, const QTextTableCellFormat& cellFormat);
	QIcon GetCategoryIcon(int category) const;
	QImage GetCategoryImage(int category) const;
	QString GetCategoryText(int category) const;

	QTextTableFormat m_tableFormat;
	QTextTableCellFormat m_okCellFormat;
};


} // namespace iloggui


#endif // !iloggui_CTextLogGuiComp_included

