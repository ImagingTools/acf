#include "iwidgets/CDoubleRangeSpinEdit.h"


namespace iwidgets
{


CDoubleRangeSpinEdit::CDoubleRangeSpinEdit(QWidget *parent): 
	QWidget(parent),
	m_maxValueEnabled(true),
	m_validateRangesEnabled(true)
{
	m_ui.setupUi(this);

	m_ui.ValueFromSpinBox->setKeyboardTracking(false);
	m_ui.ValueToSpinBox->setKeyboardTracking(false);

	connect(m_ui.ValueFromSpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnMinValueChanged(double)));
	connect(m_ui.ValueToSpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnMaxValueChanged(double)));
}


// public interface

void CDoubleRangeSpinEdit::SetMinRange(double minFrom, double minTo)
{
	if (m_validateRangesEnabled && minFrom > minTo){
		qSwap(minFrom, minTo);
	}

	m_ui.ValueFromSpinBox->setRange(minFrom, minTo);
}


void CDoubleRangeSpinEdit::SetMaxRange(double maxFrom, double maxTo)
{
	if (m_validateRangesEnabled && maxFrom > maxTo){
		qSwap(maxFrom, maxTo);
	}

	m_ui.ValueToSpinBox->setRange(maxFrom, maxTo);
}


void CDoubleRangeSpinEdit::SetRange(double from, double to)
{
	if (m_validateRangesEnabled && from > to){
		qSwap(from, to);
	}

	m_ui.ValueFromSpinBox->setRange(from, to);
	m_ui.ValueToSpinBox->setRange(from, to);
}


void CDoubleRangeSpinEdit::SetDecimals(int count)
{
	m_ui.ValueFromSpinBox->setDecimals(count);
	m_ui.ValueToSpinBox->setDecimals(count);
}


void CDoubleRangeSpinEdit::SetSingleStep(double step)
{
	m_ui.ValueFromSpinBox->setSingleStep(step);
	m_ui.ValueToSpinBox->setSingleStep(step);
}


void CDoubleRangeSpinEdit::SetSuffixText(const QString& suffix)
{
	m_ui.SuffixLabel1->setText(suffix);
	m_ui.SuffixLabel1->setVisible(!suffix.isEmpty());

	m_ui.SuffixLabel2->setText(suffix);
	m_ui.SuffixLabel2->setVisible(!suffix.isEmpty() && m_maxValueEnabled);
}


void CDoubleRangeSpinEdit::SetMinLabelText(const QString& text)
{
	m_ui.FromLabel->setText(text);
	m_ui.FromLabel->setVisible(!text.isEmpty());
}


void CDoubleRangeSpinEdit::SetMaxLabelText(const QString& text)
{
	m_ui.ToLabel->setText(text);
	m_ui.ToLabel->setVisible(!text.isEmpty() && m_maxValueEnabled);
}


void CDoubleRangeSpinEdit::EnableMaxValue(bool enable)
{
	if (m_maxValueEnabled != enable){
		m_maxValueEnabled = enable;

		m_ui.ToLabel->setVisible(!m_ui.ToLabel->text().isEmpty() && m_maxValueEnabled);
		m_ui.SuffixLabel2->setVisible(!m_ui.SuffixLabel2->text().isEmpty() && m_maxValueEnabled);
		m_ui.ValueToSpinBox->setVisible(m_maxValueEnabled);
	}
}


void CDoubleRangeSpinEdit::EnableValdation(bool enable)
{
	if (m_validateRangesEnabled != enable){
		m_validateRangesEnabled = enable;

		if (m_validateRangesEnabled){
			double minValue = GetMinValue();
			double maxValue = GetMaxValue();
			if (minValue > maxValue){
				SetValues(maxValue, minValue);
			}
		}
	}
}


void CDoubleRangeSpinEdit::SetValues(double minValue, double maxValue)
{
	if (m_validateRangesEnabled && minValue > maxValue){
		qSwap(maxValue, minValue);
	}

	m_ui.ValueFromSpinBox->setValue(minValue);
	m_ui.ValueToSpinBox->setValue(maxValue);
}


double CDoubleRangeSpinEdit::GetMinValue() const
{
	return m_ui.ValueFromSpinBox->value();
}


double CDoubleRangeSpinEdit::GetMaxValue() const
{
	return m_ui.ValueToSpinBox->value();
}


void CDoubleRangeSpinEdit::OnMinValueChanged(double val)
{
	if (m_validateRangesEnabled && val > GetMaxValue()){
		m_ui.ValueToSpinBox->setValue(val);
	}	

	Q_EMIT ValuesChanged();
}


void CDoubleRangeSpinEdit::OnMaxValueChanged(double val)
{
	if (m_validateRangesEnabled && val < GetMinValue()){
		m_ui.ValueFromSpinBox->setValue(val);
	}	

	Q_EMIT ValuesChanged();
}


} // namespace iwidgets
