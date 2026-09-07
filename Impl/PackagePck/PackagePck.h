// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>
#include <icomp/CXpcModel.h>

#include <ipackage/CPackagesLoaderComp.h>
#include <ipackage/CRegistriesManagerComp.h>
#include <ipackage/CRegistryCodeSaverComp.h>
#include <ipackage/CRegistryConsistInfoComp.h>


/**
	Component, packages and registries manipulation package.
*/
namespace PackagePck
{


typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<icomp::CRegistry>,
			icomp::IRegistry,
			iser::ISerializable,
			istd::IChangeable,
			imod::IModel> Registry;
typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<icomp::CXpcModel>,
			icomp::CXpcModel,
			iser::ISerializable,
			istd::IChangeable,
			imod::IModel> XpcModel;
typedef ipackage::CPackagesLoaderComp PackagesLoader;
typedef ipackage::CRegistriesManagerComp RegistriesManager;
typedef ipackage::CRegistryCodeSaverComp RegistryCodeSaver;
typedef ipackage::CRegistryConsistInfoComp RegistryConsistInfo;


} // namespace PackagePck


