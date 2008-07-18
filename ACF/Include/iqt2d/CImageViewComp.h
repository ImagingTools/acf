#ifndef iqt2d_CImageViewComp_included
#define iqt2d_CImageViewComp_included


#include "iqt/TGuiObserverWrap.h"

#include "iqt2d/CImageShape.h"
#include "iqt2d/CSceneProviderComp.h"


namespace iqt2d
{


/**
	Show observed bitmap on the scene.
	This class extends standard scene provider to provide background bitmap object.
*/
class CImageViewComp:  public iqt::TGuiObserverWrap<CSceneProviderComp, CImageShape>
{
public:
	typedef iqt::TGuiObserverWrap<CSceneProviderComp, CImageShape> BaseClass;

	I_BEGIN_COMPONENT(CImageViewComp)
		I_REGISTER_INTERFACE(imod::IObserver)
	I_END_COMPONENT

	CImageViewComp();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
};


} // namespace iqt2d


#endif // !iqt2d_CImageViewComp_included


