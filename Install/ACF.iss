#define XERCESDIR GetEnv("XERCESDIR")
#define XALANDIR GetEnv("XALANDIR")
#define ZLIBDIR GetEnv("ZLIBDIR")
#define QTDIR GetEnv("QTDIR")

[Setup]
AppVerName=ACF $AcfVersion:0$
AppVersion=$AcfVersion:0$
AppPublisher=ImagingTools
AppPublisherURL=http://www.imagingtools.de/
AppName=ACF
OutputBaseFilename=SetupAcf$AcfVersion:0$

DefaultDirName={sd}\ImagingTools\ACF$AcfVersion:0$
WizardImageFile=..\..\Docs\Images\InstallerImage.bmp
WizardSmallImageFile=..\..\Docs\Images\Logo\Logo_128x128.bmp
SetupIconFile=..\..\Docs\Images\Logo\Logo.ico
AlwaysShowDirOnReadyPage=True
DefaultGroupName=ImagingTools\ACF
DisableProgramGroupPage=True
UninstallDisplayIcon=..\..\Docs\Images\Logo\Logo.ico
UninstallDisplayName=ACF
OutputDir=..\Setup
LicenseFile=..\..\GNU.txt
InfoAfterFile=..\InfoAfter.txt

[Files]
Source: ..\Temp\*; DestDir: {app}; Flags: recursesubdirs; Components: acfComp
Source: ..\Microsoft.VC80.CRT\*; DestDir: {app}\Bin\DebugVC8; Flags: recursesubdirs; Components: acfComp
Source: ..\Microsoft.VC80.CRT\*; DestDir: {app}\Bin\ReleaseVC8; Flags: recursesubdirs; Components: acfComp

Source: {#XERCESDIR}\Lib\*.dll; DestDir: {app}\ExtLib\Bin; Components: xercesComp
Source: {#XERCESDIR}\*; DestDir: {app}\ExtLib\Xerces; Flags: recursesubdirs; Excludes: *.dll; Components: xercesComp

Source: {#XALANDIR}\Lib\*.dll; DestDir: {app}\ExtLib\Bin; Components: xalanComp
Source: {#XALANDIR}\*; DestDir: {app}\ExtLib\Xalan; Flags: recursesubdirs; Excludes: *.dll; Components: xalanComp

Source: {#ZLIBDIR}\*; DestDir: {app}\ExtLib\Zlib; Flags: recursesubdirs; Excludes: *.dll; Components: zlibComp

Source: {#QTDIR}\bin\Qt*4.dll; DestDir: {app}\ExtLib\Bin; Components: qtComp

[Languages]
Name: en; MessagesFile: compiler:Default.isl
Name: de; MessagesFile: compiler:Languages\German.isl
Name: pl; MessagesFile: compiler:Languages\Polish.isl

[CustomMessages]
en.MinimalType=Minimal installation
de.MinimalType=Minimale Installation
pl.MinimalType=Instalacja minimalna

en.FullType=Full installation
de.FullType=Komplete Installation
pl.FullType=Instalacja kompletna

en.CustomType=Custom installation
de.CustomType=Ausgewählte Komponenten
pl.CustomType=Wybrane komponenty

en.QtBinaries=Qt binaries
de.QtBinaries=Qt binäre Datei
pl.QtBinaries=Pliki binarne Qt

[Types]
Name: minimalType; Description: {cm:MinimalType}
Name: fullType; Description: {cm:FullType}
Name: customType; Description: {cm:CustomType}; Flags: iscustom

[Components]
Name: acfComp; Description: ACF; Types: minimalType fullType customType
Name: xercesComp; Description: Xerces; Types: fullType customType
Name: xalanComp; Description: Xalan; Types: fullType customType
Name: zlibComp; Description: ZLib; Types: fullType customType
Name: qtComp; Description: {cm:QtBinaries}; Types: fullType customType

[Registry]
Root: HKLM; Subkey: SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\; ValueName: ACFDIR; ValueData: {app}; ValueType: string; Flags: uninsdeletevalue; Components: acfComp
Root: HKLM; Subkey: SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\; ValueName: XERCESDIR; ValueData: {app}\ExtLib\Xerces; ValueType: string; Flags: uninsdeletevalue; Components: xercesComp
Root: HKLM; Subkey: SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\; ValueName: XALANDIR; ValueData: {app}\ExtLib\Xalan; ValueType: string; Flags: uninsdeletevalue; Components: xalanComp
Root: HKLM; Subkey: SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\; ValueName: ZLIBDIR; ValueData: {app}\ExtLib\Zlib; ValueType: string; Flags: uninsdeletevalue; Components: zlibComp
