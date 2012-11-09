#include "CAcfStarter.h"


// STL includes
#include <iostream>

// Qt includes
#include <QtGui/QFileDialog> 
#include <QtGui/QMessageBox>
#include <QtCore/QProcess>
#include <QtCore/QTimer>

// ACF includes
#include "iser/CXmlFileReadArchive.h"
#include "icomp/CXpcModel.h"
#include "icomp/CRegistry.h"
#include "istd/CSystem.h"


CAcfStarter::CAcfStarter(/*int argc,*/ char**argv)
{
	setupUi(this);
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	timer = new QTimer(this);
	acfDir = istd::CSystem::GetNormalizedPath("$(ACFDIR)");

	connect(browseButton, SIGNAL(clicked()), this, SLOT(GetPath())); 
	connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(GetRegistryPath(QListWidgetItem*)));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(RunACF())); 
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));	
	connect(timer, SIGNAL(timeout()), this, SLOT(RunACF()));
	
	configFilePath = argv[1];
	if (InitApplicationRegistriesList()){
		SetProperty();

		if (applicationRegistriesList.size() == 1){
			activeRegistryFilePath = applicationRegistriesList[0];
		}
	}		
}


CAcfStarter::CAcfStarter()
{
	setupUi(this);
	timer = new QTimer(this);
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);	
	acfDir = istd::CSystem::GetNormalizedPath("$(ACFDIR)");
	
	connect(browseButton, SIGNAL(clicked()), this, SLOT(GetPath())); 
	connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
						this, SLOT(GetRegistryPath(QListWidgetItem*)));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(RunACF())); 
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));	
	connect(timer, SIGNAL(timeout()), this, SLOT(RunACF()));

}


void CAcfStarter::GetPath()
{

	QString configFilePathTemp = QFileDialog::getOpenFileName(
				this,
				"Choose a file to open",
				".",
				"Config File (*.xpc)");
	if (configFilePathTemp.isEmpty()){
		return;
	}
	else{	
		configFilePath = configFilePathTemp;
		applicationRegistriesList.clear();
		activeRegistryFilePath.clear();
		listWidget->clear();
		InitApplicationRegistriesList();
		SetProperty();
		if (applicationRegistriesList.size() == 1){
			activeRegistryFilePath = applicationRegistriesList[0];
			RunACF();
		}
	}
	
}


void CAcfStarter::GetRegistryPath(QListWidgetItem*item)
{	
	if (timer->isActive()){
		timer->stop();		
	}
	activeRegistryFilePath = item->text();	
	
}


void CAcfStarter::RunACF()
{
	QProcess*myProcess = new QProcess(this);
	QStringList arguments;
	QString pathAcf = "/Bin/$(ConfigurationName)/Acf.exe";

	pathAcf = acfDir + pathAcf;
	pathAcf = istd::CSystem::GetNormalizedPath(pathAcf);	
	std::cout << " Acf.exe path :" << std::endl  << pathAcf.toStdString() << std::endl;				

	if (activeRegistryFilePath.isEmpty() || configFilePath.isEmpty()){
		if (timer->isActive()){
			timer->stop();		
		}
		return;
	}


	arguments << activeRegistryFilePath << "-config" << configFilePath;	
	myProcess->start(pathAcf, arguments);

	if (timer->isActive()){
		timer->stop();		
	}

	showMinimized();
}


void CAcfStarter::RunACF(int msec)
{
	timer->start(msec);
}


//private methods

void CAcfStarter::SetProperty()
{
	textBrowser->setText(configFilePath);	
	
	if (applicationRegistriesList.isEmpty() && activeRegistryFilePath.isEmpty()){
		listWidget->setEnabled(false);
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
	else{
		listWidget->setEnabled(true);
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}	
	
	listWidget->addItems(applicationRegistriesList);		
}


bool CAcfStarter::InitApplicationRegistriesList()
{
	// Registry file(arx) in config file(xpc)
	iser::CXmlFileReadArchive archive(configFilePath);
	icomp::CXpcModel configurationData;
	if (!configurationData.Serialize(archive)){				
		QMessageBox::warning(
					this,
					"ACF Starter",								  
					"Problem archive config File",
					QMessageBox::Ok);
		return false;
	}
	else{	
		QString confPath = acfDir + "/Config";		
		QDir packageDir;
		QFileInfo configFileInfo = configFilePath;
		std::cout << " config file dir " << configFileInfo.absolutePath().toStdString() << std::endl;		

		packageDir.setCurrent(configFileInfo.absolutePath());		
		for (int i=0; i<configurationData.GetPackageDirsCount(); i++){
			packageDir.setPath(istd::CSystem::GetNormalizedPath(configurationData.GetPackageDir(i)));
			if (!packageDir.exists()){
				std::cout << i + 1 << " Package dir doesn't exist:" << std::endl  << packageDir.absolutePath().toStdString() << std::endl;		
				QMessageBox::warning(0, "ACF Starter",
								  "package dir not exist",
								  QMessageBox::Ok);		
				return false;
			}
			else{
				std::cout << i + 1 << " Package dir exist:" << std::endl  << packageDir.absolutePath().toStdString() << std::endl;	
			}
		}
	}

	if ((configurationData.GetRegistryFilesCount() > 0) && (configurationData.GetPackageDirsCount() > 0)){
		for (int i = 0; i < configurationData.GetRegistryFilesCount(); i++){		
			///IRegistry.h
			iser::CXmlFileReadArchive archiveRegistry(configurationData.GetRegistryFile(i));
			icomp::CRegistry registryData;
			if (!registryData.Serialize(archiveRegistry)){				
				QMessageBox::warning(this, "ACF Starter",								  
					"Problem archive registry File",
					QMessageBox::Ok);
				return false;
			}

			icomp::IRegistry::ExportedInterfacesMap::const_iterator mapIter;
			for (		mapIter = registryData.GetExportedInterfacesMap().begin();
						mapIter != registryData.GetExportedInterfacesMap().end();
						mapIter++){

					QString registryInterface  = mapIter.key().data();
					if (registryInterface == "ibase::IApplication"){
						applicationRegistriesList.append(configurationData.GetRegistryFile(i));
					}

			}	

		}

		if (applicationRegistriesList.isEmpty()){
			QMessageBox::warning(
						this,
						"ACF Starter",								  
						"Archive registry does not have a interface Application",
						QMessageBox::Ok);

			return false;
		}
		else{
			return true;
		}
	}
	else{
		QMessageBox::warning(
					this, "ACF Starter",								  
					"Problem with registry file or package dirs",
					QMessageBox::Ok);

		return false;
	}
}


