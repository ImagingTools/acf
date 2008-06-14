#ifndef iqtsig_CSampleContainerView_included
#define iqtsig_CSampleContainerView_included


#include <QWidget>

#include "istd/CRange.h"

#include "imod/TSingleModelObserverBase.h"

#include "isig/ISamplesContainer.h"


namespace iqtsig
{


class CSampleContainerView: public QWidget, public imod::TSingleModelObserverBase<isig::ISamplesContainer>
{
public:
	typedef QWidget BaseClass;
	typedef imod::TSingleModelObserverBase<isig::ISamplesContainer> BaseClass2;

	CSampleContainerView(QWidget* parentPtr = NULL);

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (QWidget)
	virtual void paintEvent(QPaintEvent* event);

private:
	istd::CRange m_viewRange;
};


} // namespace iqtsig


#endif // !iqtsig_CSampleContainerView_included


