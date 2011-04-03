#include "iqt/CFileListProviderComp.h"


// Qt includes
#include <QStringList>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CFileList.h"


namespace iqt
{


// reimplemented (ibase::IFileListProvider)

istd::CStringList CFileListProviderComp::GetFileList() const
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
			istd::CStringList extensions;
			if (m_fileLoaderCompPtr->GetFileExtensions(extensions)){
				for (		istd::CStringList::const_iterator extIter = extensions.begin();
							extIter != extensions.end();
							++extIter){
					const istd::CString& extension = *extIter;

					filters << "*." + iqt::GetQString(extension);
				}
			}
		}
		else{
			int filtersCount = m_filtersAttrPtr.GetCount();
			for (int filterIndex = 0; filterIndex < filtersCount; ++filterIndex){
				filters << iqt::GetQString(m_filtersAttrPtr[filterIndex]);
			}
		}

		CFileList fileList;
		fileList.Create(
					iqt::GetQString(m_dirParamCompPtr->GetPath()),
					*m_minRecurDepthAttrPtr,
					*m_maxRecurDepthAttrPtr,
					filters);

		m_fileList = iqt::GetCStringList(fileList);
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
	if (m_dirParamModelCompPtr.IsValid() && m_dirParamModelCompPtr->IsAttached(this)){
		m_dirParamModelCompPtr->DetachObserver(this);
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace iqt


