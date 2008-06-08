#ifndef iqtsig_CSampleContainerView_included
#define iqtsig_CSampleContainerView_included


#include <QWidget>

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

	// reimplemented (QWidget)
	virtual void paintEvent(QPaintEvent* event);
};


} // namespace iqtsig


#endif // !iqtsig_CSampleContainerView_included


