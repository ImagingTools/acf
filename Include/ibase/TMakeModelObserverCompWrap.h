#ifndef ibase_TMakeModelObserverCompWrap_included
#define ibase_TMakeModelObserverCompWrap_included


// ACF includes
#include <icomp/TMakeComponentWrap.h>
#include <ibase/TModelObserverCompWrap.h>


namespace ibase
{


/**
	Cretion of wrapper for model observer components from non component classes.

	The model can be connected on component level during the initialization phase using component reference \c m_defaultModelCompPtr

	\ingroup DataModel
*/
template <	class Base,
			typename Interface1 = void,
			typename Interface2 = void,
			typename Interface3 = void,
			typename Interface4 = void,
			typename Interface5 = void,
			typename Interface6 = void,
			typename Interface7 = void,
			typename Interface8 = void,
			typename Interface9 = void,
			typename Interface10 = void>
class TMakeModelObserverCompWrap: public ibase::TModelObserverCompWrap< icomp::TMakeComponentWrap<
			Base,
			Interface1,
			Interface2,
			Interface3,
			Interface4,
			Interface5,
			Interface6,
			Interface7,
			Interface8,
			Interface9,
			Interface10> >
{
public:
	typedef ibase::TModelObserverCompWrap< icomp::TMakeComponentWrap<
				Base,
				Interface1,
				Interface2,
				Interface3,
				Interface4,
				Interface5,
				Interface6,
				Interface7,
				Interface8,
				Interface9,
				Interface10> > BaseClass;

	I_BEGIN_COMPONENT(TMakeModelObserverCompWrap);
	I_END_COMPONENT;
};


} // namespace ibase


#endif // !ibase_TMakeModelObserverCompWrap_included


