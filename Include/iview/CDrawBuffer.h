#ifndef iview_CDrawBuffer_included
#define iview_CDrawBuffer_included


// Qt includes
#include <QtGui/QPixmap>
#include <QtGui/QPainter>


// ACF includes
#include "istd/TDelPtr.h"

#include "iqt/iqt.h"


namespace iview
{


class CDrawBuffer: public QPixmap
{
public:
	typedef QPixmap BaseClass;

	CDrawBuffer();

	/**
		Reset state of this bitmap.
	*/
	void Reset();
	
	/**
		Check, if bitmap was created.
	*/
	bool IsCreated() const;

	/**
		Create new buffer with the given size.
	*/
	void Create(const QPaintDevice& device, ibase::CSize size);

	/**
		Copy rectangle area to specified draw context.
		It is used for native-compatible template implementation in iview::TViewImpl.
	*/
	void CopyRectTo(const i2d::CRect& source, QPainter& drawContext, istd::CIndex2d contextPos) const;
	
	/**
		Copy rectangle area to specified native paint device.
		It is used for native-compatible template implementation in iview::TViewImpl.
	*/
	void CopyRectTo(const i2d::CRect& source, QPaintDevice& device, istd::CIndex2d devicePos) const;

	/**
		Get used painter for drawing operations.
	*/
	QPainter& GetDrawContext();

protected:
	void EnsureDrawContext();

private:
	mutable istd::TDelPtr<QPainter> m_drawContextPtr;
};


// inline methods

inline CDrawBuffer::CDrawBuffer()
{
}


inline QPainter& CDrawBuffer::GetDrawContext()
{
	EnsureDrawContext();

	I_ASSERT(m_drawContextPtr.IsValid());

	return *m_drawContextPtr;
}


} // namespace iview


#endif // !iview_CDrawBuffer_included

