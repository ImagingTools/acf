#include <icmm/icmm.h>


namespace icmm
{


bool IsCmyk(ColorantUsage usage)
{
	switch (usage){
	case icmm::CU_CYAN:
	case icmm::CU_DARK_CYAN:
	case icmm::CU_LIGHT_CYAN:
	case icmm::CU_MAGENTA:
	case icmm::CU_DARK_MAGENTA:
	case icmm::CU_LIGHT_MAGENTA:
	case icmm::CU_YELLOW:
	case icmm::CU_DARK_YELLOW:
	case icmm::CU_LIGHT_YELLOW:
	case icmm::CU_BLACK:
	case icmm::CU_LIGHT_BLACK:
	case icmm::CU_LIGHT_LIGHT_BLACK:
		return true;
	default:
		return false;
	}
}


} // namespace icmm


