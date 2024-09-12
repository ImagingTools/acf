#include <icmm/CSRgbColor.h>


// ACF includes
#include <icmm/CRgb.h>
#include <icmm/CTristimulusSpecification.h>

// ACF includes
#include <icmm/CRgbColorModel.h>


namespace icmm
{

CSRgbColor::CSRgbColor(const icmm::CRgb& Rgb)
	:CRgbColor(Rgb)
{
    CTristimulusSpecification spec(ObserverType::TwoDegree, AstmTableType::E308Table5, std::make_shared<CIlluminant>(StandardIlluminant::D65, "D65"));
 
    SetSpecification(spec);
}


} // namespace icmm


