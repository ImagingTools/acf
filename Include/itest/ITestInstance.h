/********************************************************************************
**
**	Copyright (C) 2007-2017 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the ACF Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.ilena.org or write info@imagingtools.de for further
** 	information about the ACF.
**
********************************************************************************/


#ifndef itest_ITestInstance_included
#define itest_ITestInstance_included

// Qt includes
#include <QtCore/QStringList>

// ACF includes
#include <istd/IPolymorphic.h>


namespace itest
{


/**
	Test instance interface.
*/
class ITestInstance: virtual public istd::IPolymorphic
{
public:
	// Runs the test instance and returns report about each sub-test
	// If sub-test report is empty, the test was successful
	virtual QStringList Run() = 0;
};


} // namespace itest


#endif // !itest_ITestInstance_included
