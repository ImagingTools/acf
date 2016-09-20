#include <iview/CDrawBuffer.h>


namespace iview
{


void CDrawBuffer::Reset()
{
	m_drawContextPtr.Reset();

	BaseClass::operator=(QPixmap());
}


bool CDrawBuffer::IsCreated() const
{
	QSize bitmapSize = size();

	return !bitmapSize.isEmpty();
}


void CDrawBuffer::Create(const QPaintDevice& /*device*/, ibase::CSize size)
{
	m_drawContextPtr.Reset();

	BaseClass::operator=(QPixmap(iqt::GetQSize(size)));
}


void CDrawBuffer::CopyRectTo(const i2d::CRect& source, QPaintDevice& device, istd::CIndex2d devicePos) const
{
	Q_ASSERT(!hasAlpha());

	m_drawContextPtr.Reset();

	QPainter painter(&device);

	painter.drawPixmap(iqt::GetQPoint(devicePos), *this, iqt::GetQRect(source));
}


void CDrawBuffer::CopyRectTo(const i2d::CRect& source, QPainter& drawContext, istd::CIndex2d contextPos) const
{
	m_drawContextPtr.Reset();

	Q_ASSERT(!hasAlpha());

	drawContext.drawPixmap(iqt::GetQPoint(contextPos), *this, iqt::GetQRect(source));
}


// protected methods

void CDrawBuffer::EnsureDrawContext()
{
	if (!m_drawContextPtr.IsValid()){

		m_drawContextPtr.SetPtr(new QPainter(this));

		m_drawContextPtr->setRenderHint(QPainter::Antialiasing, true);
	}
}


} // namespace iview


