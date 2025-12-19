#pragma once


// Qt includes
#include <QtGui/QTextTableCell>

// ACF includes
#include <ilog/IMessageContainer.h>
#include <ilog/CMessage.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <GeneratedFiles/iloggui/ui_CTextLogGuiComp.h>


namespace iloggui
{


class CTextLogGuiComp:
	public iqtgui::TDesignerGuiObserverCompBase<
				Ui::CTextLogGuiComp, ilog::IMessageContainer>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CTextLogGuiComp, ilog::IMessageContainer> BaseClass;

	I_BEGIN_COMPONENT(CTextLogGuiComp);
		I_ASSIGN_MULTI_0(m_ignoreIdsListAttrPtr, "IgnoreIdsList", "List if message IDs to be ignored", false);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiRetranslate() override;

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

	/**
		List of message IDs to be ignored.
	*/
	I_MULTIATTR(int, m_ignoreIdsListAttrPtr);
};


} // namespace iloggui


