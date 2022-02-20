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


#ifndef ifile_CTextFileLogComp_included
#define ifile_CTextFileLogComp_included


// Qt includes
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QMutex>

// ACF includes
#include <imod/IModel.h>
#include <imod/CSingleModelObserverBase.h>
#include <ifile/IFileNameParam.h>
#include <ilog/CStreamLogCompBase.h>


namespace ifile
{


/**
	Message container displaying messages as log list.

	\ingroup Logging
*/
class CTextFileLogComp: public ilog::CStreamLogCompBase
{
public:
	typedef ilog::CStreamLogCompBase BaseClass;

	I_BEGIN_COMPONENT(CTextFileLogComp);
		I_ASSIGN(m_fileNameCompPtr, "LogFile", "Name of the log file", true, "LogFile");
		I_ASSIGN_TO(m_fileNameModelCompPtr, m_fileNameCompPtr, true);
		I_ASSIGN(m_isAppendAttrPtr, "AppendToExisting", "Don't overwrite existing log file", true, false);
	I_END_COMPONENT;

	CTextFileLogComp();

protected:
	// reimplemented (CStreamLogCompBase)
	virtual void WriteText(const QString& text, istd::IInformationProvider::InformationCategory category);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	void OpenFileStream();
	void CloseFileStream();

private:
	I_REF(ifile::IFileNameParam, m_fileNameCompPtr);
	I_REF(imod::IModel, m_fileNameModelCompPtr);
	I_ATTR(bool, m_isAppendAttrPtr);

	QFile m_outputFile;
	QTextStream m_outputFileStream;

	class FilePathObserver: public imod::CSingleModelObserverBase
	{
	public:
		typedef imod::CSingleModelObserverBase BaseClass;

		explicit FilePathObserver(CTextFileLogComp& parent);

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	private:
		CTextFileLogComp& m_parent;
	};

	FilePathObserver m_filePathObserver;

	QMutex m_mutex;
	int m_lastDay;
};


} // namespace ifile


#endif // !ifile_CTextFileLogComp_included


