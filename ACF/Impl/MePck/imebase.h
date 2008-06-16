#ifndef imebase_included
#define imebase_included

#include <QList>
#include <QVector>

#include "isig/isig.h"
#include "iqt/iqt.h"

#include "medriver.h"
#include "medefines.h"
#include "metypes.h"
#include "meerror.h"



namespace imebase
{

	
/**
	Logical address of hardware endpoint.
*/
class CMeAddr
{
public:
	int device;
	int subdevice;
	int channel;
};


typedef QList<CMeAddr> CMeList;

typedef QVector<double> imebase_Buffer;


} // namespace imebase


#endif // !imebase_included
