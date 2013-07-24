#ifndef iqtgui_CDoubleRangeSpinEdit_included
#define iqtgui_CDoubleRangeSpinEdit_included


// Local includes
#include "Generated/ui_CDoubleRangeSpinEdit.h"


namespace iqtgui
{


class CDoubleRangeSpinEdit: public QWidget
{
	Q_OBJECT
public:
	CDoubleRangeSpinEdit(QWidget *parent = NULL);

	// public interface
	void SetMinRange(double minFrom, double minTo);
	void SetMaxRange(double maxFrom, double maxTo);
	void SetRange(double from, double to);

	void SetDecimals(int count);
	void SetSingleStep(double step);
	void SetSuffixText(const QString& suffix);
	void SetMinLabelText(const QString& text);
	void SetMaxLabelText(const QString& text);

	void EnableMaxValue(bool enable = true);
	void EnableValdation(bool enable = true);

	void SetValues(double minValue, double maxValue);
	double GetMinValue() const;
	double GetMaxValue() const;

Q_SIGNALS:
	void ValuesChanged();

protected Q_SLOTS:
	void OnMinValueChanged(double);
	void OnMaxValueChanged(double);

protected:
	Ui::CDoubleRangeSpinEdit m_ui;

	bool m_maxValueEnabled;
	bool m_validateRangesEnabled;
};


} // namespace iqtgui


#endif // !iqtgui_CDoubleRangeSpinEdit_included
