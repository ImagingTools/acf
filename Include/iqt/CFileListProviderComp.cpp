#include "iqt/CFileListProviderComp.h"


// Qt includes
#include <QStringList>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CFileList.h"


namespace iqt
{


// reimplemented (ibase::IFileListProvider)

QStringList CFileListProviderComp::GetFileList() const
{
	return m_fileList;
}


// protected methods

// reimplemented (imod::CSingleModelObserverBase)

void CFileListProviderComp::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	istd::CChangeNotifier notifier(this);

	m_fileList.clear();

	if (m_dirParamCompPtr.IsValid()){
		QStringList filters;

		if (m_fileLoaderCompPtr.IsValid()){
			QStringList extensions;
			if (m_fileLoaderCompPtr->GetFileExtensions(extensions)){
				for (		QStringList::const_iterator extIter = extensions.begin();
							extIter != extensions.end();
							++extIter){
					const QString& extension = *extIter;

					filters << "*." + extension;
				}
			}
		}
		else{
			int filtersCount = m_filtersAttrPtr.GetCount();
			for (int filterIndex = 0; filterIndex < filtersCount; ++filterIndex){
				filters << m_filtersAttrPtr[filterIndex];
			}
		}

		CFileList fileList;
		fileList.Create(
					m_dirParamCompPtr->GetPath(),
					*m_minRecurDepthAttrPtr,
					*m_maxRecurDepthAttrPtr,
					filters);

		m_fileList = (fileList);
	}
}


// reimplemented (icomp::CComponentBase)

void CFileListProviderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_dirParamModelCompPtr.IsValid()){
		m_dirParamModelCompPtr->AttachObserver(this);
	}
	else{
		OnUpdate(0, NULL);
	}
}


void CFileListProviderComp::OnComponentDestroyed()
{
	imod::CSingleModelObserverBase::EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace iqt


