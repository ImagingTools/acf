#define QTDIR GetEnv("QTDIR")
#define XERCESDIR GetEnv("XERCESDIR")
#define XALANDIR GetEnv("XALANDIR")
#define ZLIBDIR GetEnv("ZLIBDIR")

[Setup]
AppVerName=ACF-Test
AppVersion=0.6
AppPublisher=ImagingTools
AppPublisherURL=http://www.imagingtools.de/
AppName=ACF

DefaultDirName={pf}\ImagingTools\ACF
WizardImageFile=..\Docs\Images\InstallerImage.png
WizardSmallImageFile=..\Docs\Images\Logo\Logo_64x64.png
SetupIconFile=..\Docs\Images\Logo\Logo_64x64.bmp
AlwaysShowDirOnReadyPage=True
DefaultGroupName=ImagingTools\ACF
DisableProgramGroupPage=True
UninstallDisplayIcon=..\Docs\Images\Logo\Logo_64x64.png
UninstallDisplayName=ACF
OutputDir=.\Setup
LicenseFile=..\license.txt

[Files]
Source: .\Temp\*; DestDir: {app}; Components: acfComp

Source: ..\ACFDIR.envVariable; DestDir: {app}\ExtLib\Qt; DestName: QTDIR.envVariable; Components: qtComp

Source: {#QTDIR}\Bin\Qt*4.dll; DestDir: {app}\ExtLib\Bin; Components: qtComp
Source: {#QTDIR}\Lib\Qt*4.lib; DestDir: {app}\ExtLib\Qt\Lib; Components: qtComp
Source: {#QTDIR}\Include\*; DestDir: {app}\ExtLib\Qt\Include; Flags: recursesubdirs; Components: qtComp

Source: {#XERCESDIR}\Lib\*.dll; DestDir: {app}\ExtLib\Bin; Components: xercesComp
Source: {#XERCESDIR}\*; DestDir: {app}\ExtLib\Xerces; Flags: recursesubdirs; Excludes: *.dll; Components: xercesComp

Source: {#XALANDIR}\Lib\*.dll; DestDir: {app}\ExtLib\Bin; Components: xalanComp
Source: {#XALANDIR}\*; DestDir: {app}\ExtLib\Xalan; Flags: recursesubdirs; Excludes: *.dll; Components: xalanComp

Source: {#ZLIBDIR}\*; DestDir: {app}\ExtLib\Zlib; Flags: recursesubdirs; Excludes: *.dll; Components: zlibComp

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

[Types]
Name: minimalType; Description: {cm:MinimalType}
Name: fullType; Description: {cm:FullType}
Name: customType; Description: {cm:CustomType}

[Components]
Name: acfComp; Description: ACF; Types: fullType minimalType customType
Name: qtComp; Description: Qt; Types: fullType customType
Name: xercesComp; Description: Xerces; Types: fullType customType
Name: xalanComp; Description: Xalan; Types: fullType customType
Name: zlibComp; Description: ZLib; Types: fullType customType

[Registry]
Root: HKLM; Subkey: SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\; ValueName: ACFDIR; ValueData: {app}; ValueType: string; Components: acfComp
Root: HKLM; Subkey: SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\; ValueName: QTDIR; ValueData: {app}\ExtLib\Qt; ValueType: string; Components: qtComp
Root: HKLM; Subkey: SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\; ValueName: XERCESDIR; ValueData: {app}\ExtLib\Xerces; ValueType: string; Components: xercesComp
Root: HKLM; Subkey: SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\; ValueName: XALANDIR; ValueData: {app}\ExtLib\Xalan; ValueType: string; Components: xalanComp
Root: HKLM; Subkey: SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\; ValueName: ZLIBDIR; ValueData: {app}\ExtLib\Zlib; ValueType: string; Components: zlibComp

