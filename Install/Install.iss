#define QTDIR GetEnv("QTDIR")
#define EXTLIBDIR GetEnv("EXTLIBDIR")

[Setup]
AppVerName=$AcfProductName$ $AcfVersion:0$
AppVersion=$AcfVersion:0$
AppPublisher=$AcfCompanyName$
AppPublisherURL=http://www.imagingtools.de/
AppName=ACF
OutputBaseFilename=Setup_$AcfProductName$_v$AcfVersion:0$_$CompilerName$

DefaultDirName={pf}\ImagingTools\$AcfProductName$_v$AcfVersion:0$
WizardImageFile=..\WizardImageFile.bmp
WizardSmallImageFile=..\WizardSmallImageFile.bmp
SetupIconFile=..\..\Docs\Images\Logo\Logo.ico
AlwaysShowDirOnReadyPage=True
DefaultGroupName=ImagingTools\ACF
DisableProgramGroupPage=True
UninstallDisplayIcon=..\..\Docs\Images\Logo\Logo.ico
UninstallDisplayName=$AcfProductName$ $AcfVersion:0$
OutputDir=..\Setup
LicenseFile=$LicensePath$
VersionInfoDescription=$AcfProductName$ v. $AcfVersion:0$ ($AcfRawVersion:0$) by $AcfCompanyName$, compiled using $CompilerName$
AppCopyright=Copyright (C) 2007-2009 $AcfCompanyName$
VersionInfoVersion=$AcfVersion:0$

[Files]
Source: ..\Temp\*; DestDir: {app}; Flags: recursesubdirs; Components: acfComp
Source: ..\Temp\Bin\Info\*; DestDir: {app}\Bin\Debug$CompilerName$; Flags: recursesubdirs; Components: acfComp
Source: ..\Temp\Bin\Info\*; DestDir: {app}\Bin\Release$CompilerName$; Flags: recursesubdirs; Components: acfComp
Source: {#EXTLIBDIR}\Microsoft\Redist\Debug$CompilerName$\*; DestDir: {app}\Bin\Debug$CompilerName$; Flags: recursesubdirs; Components: acfComp
Source: {#EXTLIBDIR}\Microsoft\Redist\Release$CompilerName$\*; DestDir: {app}\Bin\Release$CompilerName$; Flags: recursesubdirs; Components: acfComp

Source: {#QTDIR}\bin\Qt*4.dll; DestDir: {app}\ExtLib\Bin; Components: qtComp
Source: {#QTDIR}\plugins\imageformats\*.dll; DestDir: {app}\ExtLib\Bin\imageformats; Components: qtComp
Source: {#QTDIR}\plugins\iconengines\*.dll; DestDir: {app}\ExtLib\Bin\iconengines; Components: qtComp

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

en.AddExtPath=Add external binaries path to the system PATH
de.AddExtPath=Pfad für externe Bibliotheken zum Systempfad hinzufügen
pl.AddExtPath=Dodaj sciezke zewnetrznych bibliotek do sciezki systemowej

en.AddAcfPath=Add ACF tools path to the system PATH
de.AddAcfPath=Pfad für ACF-Tools zum Systempfad hinzufügen
pl.AddAcfPath=Dodaj sciezke narzedzi ACF do sciezki systemowej

[Types]
Name: minimalType; Description: {cm:MinimalType}
Name: fullType; Description: {cm:FullType}
Name: customType; Description: {cm:CustomType}; Flags: iscustom

[Components]
Name: acfComp; Description: ACF; Types: minimalType fullType customType
Name: qtComp; Description: {cm:QtBinaries}; Types: fullType customType

[Icons]
Name: "{group}\Compositor Mini"; Filename: {app}\Bin\Release$CompilerName$\CompositorMini.exe; WorkingDir: {app}\Bin\Release$CompilerName$; Components: acfComp
Name: "{group}\API Documentation"; Filename: {app}\Docs\TechnicalDoc\index.html; Components: acfComp

[Tasks]
Name: AddExtPath; Description: {cm:AddExtPath}; Flags: checkedonce; Components: qtComp
Name: AddAcfPath; Description: {cm:AddAcfPath}; Flags: checkedonce; Components: acfComp

[Registry]
Root: HKCU; Subkey: "Environment"; ValueType: string; ValueName: "Path"; ValueData: "{olddata};{app}\ExtLib\Bin"; Flags: createvalueifdoesntexist; Tasks: AddExtPath
Root: HKCU; Subkey: "Environment"; ValueType: string; ValueName: "Path"; ValueData: "{olddata};{app}\Bin\Release$CompilerName$"; Flags: createvalueifdoesntexist; Tasks: AddAcfPath
Root: HKCU; Subkey: "Environment"; ValueName: ACFDIR; ValueData: {app}; ValueType: string; Flags: uninsdeletevalue; Components: acfComp

Root: HKLM; Subkey: "SOFTWARE\\Classes\\.arx"; ValueType: string; ValueData: arx_auto_file; Components: acfComp
Root: HKLM; Subkey: "SOFTWARE\\Classes\\arx_auto_file"; ValueData: "ACF Registry"; ValueType: string; Flags: uninsdeletevalue; Components: acfComp
Root: HKLM; Subkey: "SOFTWARE\\Classes\\arx_auto_file"; ValueName: EditFlags; ValueData: 0; ValueType: dword; Flags: uninsdeletevalue; Components: acfComp
Root: HKLM; Subkey: "SOFTWARE\\Classes\\arx_auto_file"; ValueName: BrowserFlags; ValueData: 8; ValueType: dword; Flags: uninsdeletevalue; Components: acfComp
Root: HKLM; Subkey: "SOFTWARE\\Classes\\arx_auto_file\\Run\\command"; ValueData: "{app}\\Bin\\Release$CompilerName$\\Acf.exe %22%251%22"; ValueType: string; Flags: uninsdeletevalue; Components: acfComp
