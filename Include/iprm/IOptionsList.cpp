#include <iprm/IOptionsList.h>


namespace iprm
{


// gobal methods

int FindOptionIndexById(const QByteArray& id, const iprm::IOptionsList& list)
{
	if (id.isEmpty()){
		return -1;
	}

	int optionsCount = list.GetOptionsCount();
	for (int i = 0; i < optionsCount; ++i){
		if (id == list.GetOptionId(i)){
			return i;
		}
	}

	return -1;
}


} // namespace iprm


